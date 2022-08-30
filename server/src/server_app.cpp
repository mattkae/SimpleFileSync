#include "server_app.hpp"
#include <algorithm>
#include <exception>
#include <filesystem>
#include <iterator>
#include "client_message.hpp"
#include "deserializer.hpp"
#include "event.hpp"
#include "logger.hpp"
#include "server_message.hpp"
#include "hash_calculator.hpp"
#include "server_socket.hpp"
#include "socket_buffer.hpp"
#include "socket_connection.hpp"
#include "environment.hpp"
#include "type.hpp"

namespace server {
    ServerApp::ServerApp():
        mConfig(shared::Environment::get().getConfigDirectory() + "server.conf"),
        mState(shared::Environment::get().getDataDirectory() + ".server_saved.data"),
        mLedger(shared::Environment::get().getDataDirectory() + ".server_events")
    {
		mState.load();

        mConfig.load();
        if (!std::filesystem::is_directory(mConfig.getDirectory()) || !std::filesystem::exists(mConfig.getDirectory())) {
            logger_info("Creating directory defined in the configuration: %s", mConfig.getDirectory().c_str());
            std::filesystem::create_directories(mConfig.getDirectory());
        }
    }

    ServerApp::~ServerApp() { }

    void ServerApp::_onData(SocketBuffer& buff) {
        shared::u64 bytesDeserialized = 0;
        const shared::u64 bytesRead = buff.bytesRead;
        while (bytesDeserialized < bytesRead) {
            shared::BinaryDeserializer clientSerializer({ &buff.buffer[bytesDeserialized], bytesRead, 0 });
            shared::ClientMessage incoming = clientSerializer.readObject<shared::ClientMessage>();
            bytesDeserialized += clientSerializer.getCursor();

            logger_info("bytes read=%lu / bytes deserialized=%lu", bytesRead, bytesDeserialized);

            switch (incoming.type) {
            case shared::ClientMessageType::RequestStartComm: {
                logger_info("Client has begun communication.");
                auto lastConfirmedClientHash = incoming.event.hash;
                
                // @NOTE: The last confirmed client hash is either in sync with the server or behind the server.
                shared::ServerMessage message;
                if (lastConfirmedClientHash == mState.getHash()) {
                    logger_info("Clients are in sync.");
                    message.type = shared::ServerMessageType::ResponseStartComm;
                }
                else {
                    logger_info("Client is not in sync.");
                    message.type = shared::ServerMessageType::ResponseStartComm;
                    auto hashList = mState.getHashList();
                    std::vector<shared::u64> hashesToSend;
                    if (lastConfirmedClientHash == 0) {
                        hashesToSend = hashList;
                    }
                    else {
                        auto it = std::find(hashList.begin(), hashList.end(), lastConfirmedClientHash);
                        if (it != hashList.end()) {
                            auto lastHashIndex = it - hashList.begin();
                            hashesToSend = std::vector<shared::u64>(hashList.begin() + lastHashIndex, hashList.end());
                        }
                    }
                    
                    std::vector<shared::Event> eventsToSend;
                    for (shared::u64 hash : hashesToSend) {
                        auto event = mLedger.retrieve(hash);
                        // @TODO: Cleanup unserialized nonsense
                        event.fullpath = mConfig.getDirectory() + "/" + event.path;
                        eventsToSend.push_back(event);
                    }

                    message.eventsForClient = eventsToSend;
                }

                mServerSerializer.reset();
                mServerSerializer.writeObject(message);
                buff.connection->writeData(mServerSerializer.getData(), mServerSerializer.getSize());
                break;
            }
            case shared::ClientMessageType::ChangeEvent:
                logger_info("Processing change event, hash=%lu", incoming.event.hash);
                processChangeEvent(incoming);
                break;
            case shared::ClientMessageType::RequestEndComm:
                logger_info("Client requesting termination of communication.");
                buff.connection->stop();
                break;
            default:
                logger_warning("Unknown request: %d", (int)incoming.type);
                break;
            }
        }
    }

    void ServerApp::run() {
        try {
            ServerSocketOptions options;
            options.onRead = [this](SocketBuffer& data) { return this->_onData(data); };
            options.port = mConfig.getPort();
            options.useSsl = mConfig.useSsl();
            options.sslOptions = mConfig.getSslOptions();
            ServerSocket mSocket(options);
            mSocket.run();
        } catch (std::exception& e) {
            logger_error("Exception while talking to client: %s", e.what());
        }
    }

    bool ServerApp::processChangeEvent(shared::ClientMessage& incoming) {
        auto data = incoming;
        auto hash = data.event.hash;
        
        auto expectedNextHash = shared::getHash(mState.getHash(), data.event);
        if (expectedNextHash != hash) {
            logger_error("Hashes do not add up, expected=%lu, received=%lu", expectedNextHash, hash);;
            return false;
        }

        logger_info("New hash accepted, hash=%lu", hash);
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
