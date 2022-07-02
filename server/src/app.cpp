#include "app.hpp"
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

namespace server {
    App::App() {
        mState = shared::State(shared::getSaveAreaPath("server_saved.data"));
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
		    boost::asio::ip::tcp::acceptor acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 13));
            while (mIsRunning) {
                boost::asio::ip::tcp::socket socket(io_context);
                acceptor.accept(socket);

                spdlog::info("Client connected.");
                //socket.set_option(boost::asio::detail::socket_option::integer<SOL_SOCKET, SO_RCVTIMEO>{ 200 });

                const size_t MAX_BUFF_SIZE = 1024;
                boost::array<shared::Byte, MAX_BUFF_SIZE> buf;
                while (socket.is_open()) {
                    boost::system::error_code error;
                    socket.wait(socket.wait_read);
                    size_t bytesRead = socket.read_some(boost::asio::buffer(buf), error);
                    size_t bytesDeserialized = 0;

                    while (bytesRead != bytesDeserialized) {
                        shared::BinaryDeserializer<shared::ClientMessage> clientSerializer({ &buf.data()[bytesDeserialized], bytesRead });
                        shared::ClientMessage incoming;
                        bytesDeserialized += clientSerializer.deserialize(incoming);

                        spdlog::info("bytes read={0} / bytes deserialized={1}", bytesRead, bytesDeserialized);

                        switch (incoming.getData().type) {
                        case shared::ClientMessageType::RequestStartComm: {
                            spdlog::info("Client is requesting data.");
                            shared::ServerMessageData data;
                            data.type = shared::ServerMessageType::ResponseStartComm;
                            data.hash =  mState.hash;
                            shared::ServerMessage response(data);
                            boost::system::error_code ignored_error;
                            mServerSerializer.reset();
                            mServerSerializer.serialize(response);
                            boost::asio::write(socket, boost::asio::buffer(mServerSerializer.getData(), mServerSerializer.getSize()), ignored_error);
                            break;
                        }
                        case shared::ClientMessageType::ChangeEvent:
                            spdlog::info("Processing change event, hash={0}", incoming.getData().hash);
                            processChangeEvent(incoming);
                            break;
                        case shared::ClientMessageType::RequestEndComm:
                            spdlog::info("Client requesting termination of communication.");
                            socket.close();
                            break;
                        default:
                            spdlog::warn("Unknown request: {0}", (int)incoming.getData().type);
                            break;
                        }
                    }
                }

                spdlog::info("Closing client.");
                socket.close();
            }
        } catch (std::exception& e) {
            spdlog::error("Exception while talking to client: {0}", e.what());
        }
    }

    bool App::processChangeEvent(shared::ClientMessage& incoming) {
        auto data = incoming.getData();
        auto hash = data.hash;
        
        auto expectedNextHash = shared::getHash(mState.hash, data.event);
        if (expectedNextHash != hash) {
            spdlog::error("Hashes do not add up, expected={0}, received={1}", expectedNextHash, hash);;
            return false;
        }

        spdlog::info("New hash accepeted, hash={0}", hash);
        mState.hash = hash;
        mState.write();

        shared::executeEvent(data.event, mConfig.getDirectory());

        return true;
    }
}