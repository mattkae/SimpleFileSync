#include "server_app.hpp"
#include <algorithm>
#include <exception>
#include <filesystem>
#include <iterator>
#include "client_message.hpp"
#include "deserializer.hpp"
#include "event.hpp"
#include "save_area.hpp"
#include "server_message.hpp"
#include "hash_calculator.hpp"
#include "server_socket.hpp"
#include "server_socket.hpp"
#include "spdlog/common.h"
#include <spdlog/spdlog.h>

namespace server {
    ServerApp::ServerApp():
        mConfig(shared::getSaveAreaPath("server.conf")),
        mState(shared::getSaveAreaPath(".server_saved.data")),
        mLedger(shared::getSaveAreaPath(".server_events"))
    {
		mState.load();

        mConfig.load();
        if (!std::filesystem::is_directory(mConfig.getDirectory()) || !std::filesystem::exists(mConfig.getDirectory())) {
            spdlog::info("Creating directory defined in the configuration: {0}", mConfig.getDirectory());
            std::filesystem::create_directories(mConfig.getDirectory());
        }
    }

    ServerApp::~ServerApp() { }

    size_t ServerApp::_onData(SocketConnection& conn) {
        shared::BinaryDeserializer clientSerializer({ &conn.buffer[conn.bytesDeserialized], conn.bytesRead });
        shared::ClientMessage incoming = clientSerializer.readObject<shared::ClientMessage>();

        spdlog::info("bytes read={0} / bytes deserialized={1}", conn.bytesRead, conn.bytesDeserialized);

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

            mServerSerializer.reset();
            mServerSerializer.writeObject(message);
            conn.write(mServerSerializer.getData(), mServerSerializer.getSize());
            break;
        }
        case shared::ClientMessageType::ChangeEvent:
            spdlog::info("Processing change event, hash={0}", incoming.event.hash);
            processChangeEvent(incoming);
            break;
        case shared::ClientMessageType::RequestEndComm:
            spdlog::info("Client requesting termination of communication.");
            conn.doClose();
            break;
        default:
            spdlog::warn("Unknown request: {0}", (int)incoming.type);
            break;
        }

        return clientSerializer.getCursor();
    }

    void ServerApp::run() {
        try {
            ServerSocketOptions options;
            options.onRead = [this](SocketConnection& data) { return this->_onData(data); };
            options.port = mConfig.getPort();
            options.useSsl = true;
            ServerSocket mSocket(options);
            mSocket.run();
        } catch (std::exception& e) {
            spdlog::error("Exception while talking to client: {0}", e.what());
        }
    }

    bool ServerApp::processChangeEvent(shared::ClientMessage& incoming) {
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
