#include "app.hpp"
#include <algorithm>
#include <exception>
#include <filesystem>
#include <iostream>
#include <boost/array.hpp>
#include <iterator>
#include "client_message.hpp"
#include "deserializer.hpp"
#include "event.hpp"
#include "save_area.hpp"
#include "server_message.hpp"
#include "hash_calculator.hpp"
#include "spdlog/common.h"
#include <spdlog/spdlog.h>
#include <unistd.h>

namespace server {
    App::App():
        mState(shared::getSaveAreaPath(".server_saved.data")),
        mLedger(shared::getSaveAreaPath(".server_events"))
    {
		mState.load();

        mConfig = server::Config(shared::getSaveAreaPath("server.conf"));
        mConfig.load();
        if (!std::filesystem::is_directory(mConfig.getDirectory()) || !std::filesystem::exists(mConfig.getDirectory())) {
            spdlog::info("Creating directory defined in the configuration: {0}", mConfig.getDirectory());
            std::filesystem::create_directories(mConfig.getDirectory());
        }
    }

    App::~App() { mIsRunning = false; }

    void App::run() {
        mIsRunning = true;
        try {
            boost::asio::io_context io_context;
		    boost::asio::ip::tcp::acceptor acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), mConfig.getPort()));
            while (mIsRunning) {
                boost::asio::ip::tcp::socket socket(io_context);
                acceptor.accept(socket);

                spdlog::info("Client connected.");
                //socket.set_option(boost::asio::detail::socket_option::integer<SOL_SOCKET, SO_RCVTIMEO>{ 200 });

                const size_t MAX_BUFF_SIZE = 1024;
                boost::array<shared::byte, MAX_BUFF_SIZE> buf;
                while (socket.is_open()) {
                    boost::system::error_code error;
                    socket.wait(socket.wait_read);
                    size_t bytesRead = socket.read_some(boost::asio::buffer(buf), error);
                    size_t bytesDeserialized = 0;

                    while (bytesRead != bytesDeserialized) {
                        shared::BinaryDeserializer clientSerializer({ &buf.data()[bytesDeserialized], bytesRead });
                        shared::ClientMessage incoming = clientSerializer.readObject<shared::ClientMessage>();
                        bytesDeserialized += clientSerializer.getCursor();

                        spdlog::info("bytes read={0} / bytes deserialized={1}", bytesRead, bytesDeserialized);

                        switch (incoming.type) {
                        case shared::ClientMessageType::RequestStartComm: {
                            spdlog::info("Client has begun communication.");
                            auto lastConfirmedClientHash = incoming.event.hash;
                            
                            // @NOTE: The last confirmed client hash is either in sync with the server or behind the server.
                            shared::ServerMessage message;
                            if (lastConfirmedClientHash == mState.getHash()) {
                                spdlog::info("Clients are in sync.");
                                message.type = shared::ServerMessageType::ResponseStartComm;
                            }
                            else {
                                spdlog::info("Client is not in sync.");
                                message.type = shared::ServerMessageType::ResponseStartComm;
                                auto hashList = mState.getHashList();
                                std::vector<size_t> hashesToSend;
                                if (lastConfirmedClientHash == 0) {
                                    hashesToSend = hashList;
                                }
                                else {
                                    auto it = std::find(hashList.begin(), hashList.end(), lastConfirmedClientHash);
                                    if (it != hashList.end()) {
                                        auto lastHashIndex = it - hashList.begin();
                                        hashesToSend = std::vector<size_t>(hashList.begin() + lastHashIndex, hashList.end());
                                    }
                                }
                                
                                std::vector<shared::Event> eventsToSend;
                                for (size_t hash : hashesToSend) {
                                    auto event = mLedger.retrieve(hash);
                                    // @TODO: Cleanup unserialized nonsense
                                    event.fullpath = mConfig.getDirectory() + "/" + event.path;
                                    eventsToSend.push_back(event);
                                }

                                message.eventsForClient = eventsToSend;
                            }

                            boost::system::error_code ignored_error;
                            mServerSerializer.reset();
                            mServerSerializer.writeObject(message);
                            boost::asio::write(socket, boost::asio::buffer(mServerSerializer.getData(), mServerSerializer.getSize()), ignored_error);
                            break;
                        }
                        case shared::ClientMessageType::ChangeEvent:
                            spdlog::info("Processing change event, hash={0}", incoming.event.hash);
                            processChangeEvent(incoming);
                            break;
                        case shared::ClientMessageType::RequestEndComm:
                            spdlog::info("Client requesting termination of communication.");
                            socket.close();
                            break;
                        default:
                            spdlog::warn("Unknown request: {0}", (int)incoming.type);
                            break;
                        }
                    }
                }

                spdlog::info("Client closed.");
            }
        } catch (std::exception& e) {
            spdlog::error("Exception while talking to client: {0}", e.what());
        }
    }

    bool App::processChangeEvent(shared::ClientMessage& incoming) {
        auto data = incoming;
        auto hash = data.event.hash;
        
        auto expectedNextHash = shared::getHash(mState.getHash(), data.event);
        if (expectedNextHash != hash) {
            spdlog::error("Hashes do not add up, expected={0}, received={1}", expectedNextHash, hash);;
            return false;
        }

        spdlog::info("New hash accepted, hash={0}", hash);
        mState.addHash(hash);
        mState.write();

        // @TODO: Cleanup unserialized nonsense
        data.event.fullpath = mConfig.getDirectory() + "/" + data.event.path;
        shared::executeEvent(data.event, mConfig.getDirectory());
        data.event.hash = hash;
        mLedger.record(data.event);

        return true;
    }
}
