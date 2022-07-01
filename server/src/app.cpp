#include "app.hpp"
#include <filesystem>
#include <iostream>
#include <boost/array.hpp>
#include <iterator>
#include "client_message.hpp"
#include "deserializer.hpp"
#include "save_area.hpp"
#include "server_message.hpp"
#include "hash_calculator.hpp"

namespace server {
    App::App() {
        mState = shared::State(shared::getSaveAreaPath("server_saved.data"));
		mState.load();

        mConfig = server::Config(shared::getSaveAreaPath("server.conf"));
        mConfig.load();
        if (!std::filesystem::is_directory(mConfig.getDirectory()) || !std::filesystem::exists(mConfig.getDirectory())) {
            std::cout << "Creating directory defined in the configuration: " << mConfig.getDirectory() << std::endl;
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

                std::cout << "Client connected." << std::endl;
                //socket.set_option(boost::asio::detail::socket_option::integer<SOL_SOCKET, SO_RCVTIMEO>{ 200 });

                const size_t MAX_BUFF_SIZE = 1024;
                boost::array<shared::Byte, MAX_BUFF_SIZE> buf;
                while (socket.is_open()) {
                    boost::system::error_code error;

                    socket.wait(socket.wait_read);
                    size_t bytesRead = socket.read_some(boost::asio::buffer(buf), error);
                    if (bytesRead == 0) {
                        continue;
                    }

                    shared::BinaryDeserializer<shared::ClientMessage> clientSerializer({ buf.data(), bytesRead });
                    shared::ClientMessage incoming;
                    size_t bytesDeserialized = clientSerializer.deserialize(incoming);

                    std::cout << "bytes read: " << bytesRead << " / bytes deserialized: " << bytesDeserialized << std::endl;

                    switch (incoming.getData().type) {
                    case shared::ClientMessageType::RequestStartComm: {
                        shared::ServerMessageData data;
                        data.type = shared::ServerMessageType::ResponseStartComm;
                        data.hash =  mState.hash;
                        shared::ServerMessage response(data);
                        boost::system::error_code ignored_error;
                        mServerSerializer.reset();
                        mServerSerializer.serialize(response);
                        boost::asio::write(socket, boost::asio::buffer(mServerSerializer.getData(), mServerSerializer.getSize()), ignored_error);
                        std::cout << "Client is requesting data." << std::endl;
                        break;
                    }
                    case shared::ClientMessageType::ChangeEvent:
                        std::cout << "Processing change event." << std::endl;
                        std::cout << "Hash: " << incoming.getData().hash << std::endl;
                        processChangeEvent(incoming);
                        break;
                    case shared::ClientMessageType::RequestEndComm:
                        std::cout << "Client requesting termination of communication." << std::endl;
                        socket.close();
                        break;
                    default:
                        std::cout << "Unknown request: " << (int)incoming.getData().type << std::endl;
                        break;
                    }
                }

                std::cout << "Closing client." << std::endl << std::endl;
                socket.close();
            }
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    bool App::processChangeEvent(shared::ClientMessage& incoming) {
       auto data = incoming.getData();
       auto hash = data.hash;
       
       auto expectedNextHash = shared::getHash(mState.hash, data.event);
       if (expectedNextHash != hash) {
           std::cerr << "Error: Hashes do not add up! We need to resolve. Expected: " << expectedNextHash << ", Received: " << hash << std::endl;
           return false;
       }

       std::cout << "New hash accepted: " << hash << std::endl;
       mState.hash = hash;
       mState.write();
       return true;
    }
}