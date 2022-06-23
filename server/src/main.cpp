#include "config.hpp"
#include <filesystem>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "server_message.hpp"
#include "client_message.hpp"
#include "serializer.hpp"
#include "deserializer.hpp"

int main() {
	server::Config globalConfig("../data/config.conf");
	if (!std::filesystem::is_directory(globalConfig.getDirectory()) || !std::filesystem::exists(globalConfig.getDirectory())) {
		std::filesystem::create_directory(globalConfig.getDirectory());
		std::cout << "Creating directory defined in the configuration...";
	}

	shared::BinarySerializer<shared::ServerMessage> serverSerializer;
	
	try {
		boost::asio::io_context io_context;
		boost::asio::ip::tcp::acceptor acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 13));
		for (;;) {
			boost::asio::ip::tcp::socket socket(io_context);
			acceptor.accept(socket);

			std::cout << "Client connected." << std::endl;

			// Let's find out what it is that thery want

		    boost::array<shared::Byte, 1024> buf;
			boost::system::error_code error;

			size_t len = socket.read_some(boost::asio::buffer(buf), error);
			shared::BinaryDeserializer<shared::ClientMessage> clientSerializer({ buf.data(), len });
			shared::ClientMessage incoming = clientSerializer.deserialize();

			std::cout << "Message length: " << len << std::endl;

			shared::ServerMessageData data;
			switch (incoming.getData().type) {
			case shared::ClientMessageType::RequestStatus:
				data.type = shared::ServerMessageType::ResponseStatus;
				std::cout << "Client is requesting data." << std::endl;
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
			std::cout << "Closing client." << std::endl;

			socket.close();
		}
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
    
	return 0;
}
