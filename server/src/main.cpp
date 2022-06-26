#include "config.hpp"
#include <filesystem>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iterator>
#include "server_message.hpp"
#include "client_message.hpp"
#include "serializer.hpp"
#include "deserializer.hpp"
#include "save_area.hpp"

int main() {
	server::Config globalConfig(shared::getSaveAreaPath("server.conf"));
	globalConfig.load();
	if (!std::filesystem::is_directory(globalConfig.getDirectory()) || !std::filesystem::exists(globalConfig.getDirectory())) {
		std::cout << "Creating directory defined in the configuration: " << globalConfig.getDirectory() << std::endl;
		std::filesystem::create_directories(globalConfig.getDirectory());
	}

	shared::BinarySerializer<shared::ServerMessage> serverSerializer;
	
	try {
		boost::asio::io_context io_context;
		boost::asio::ip::tcp::acceptor acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 13));
		for (;;) {
			boost::asio::ip::tcp::socket socket(io_context);
			acceptor.accept(socket);

			std::cout << "Client connected." << std::endl;
			//socket.set_option(boost::asio::detail::socket_option::integer<SOL_SOCKET, SO_RCVTIMEO>{ 200 });

			const size_t MAX_BUFF_SIZE = 1024;
			boost::array<shared::Byte, MAX_BUFF_SIZE> buf;
			while (socket.is_open()) {
				buf.empty();
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

				shared::ServerMessageData data;
				switch (incoming.getData().type) {
				case shared::ClientMessageType::RequestStartComm:
					data.type = shared::ServerMessageType::ResponseStartComm;
					std::cout << "Client is requesting data." << std::endl;
					break;
				case shared::ClientMessageType::ChangeEvent:
					std::cout << "Processing change event." << std::endl;
					std::cout << "Hash: " << incoming.getData().hash << std::endl;
					break;
				case shared::ClientMessageType::RequestEndComm:
					std::cout << "Client requesting termination of communication." << std::endl;
					socket.close();
					break;
				default:
					std::cout << "Unknown request: " << (int)incoming.getData().type << std::endl;
					break;
				}
		    	
				shared::ServerMessage response(data);
				boost::system::error_code ignored_error;
				serverSerializer.reset();
				serverSerializer.serialize(response);
				boost::asio::write(socket, boost::asio::buffer(serverSerializer.getData(), serverSerializer.getSize()), ignored_error);
			}

			std::cout << "Closing client." << std::endl;
			socket.close();
		}
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
    
	return 0;
}
