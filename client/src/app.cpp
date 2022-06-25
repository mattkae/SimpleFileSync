#include "app.hpp"
#include "client_message.hpp"
#include "config.hpp"
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <filesystem>
#include <string>
#include "file_watcher.hpp"
#include "serializer.hpp"
#include "server_message.hpp"
#include "hash_calculator.hpp"

namespace client {
	App::App() {
		mConfig = client::Config("../data/config.conf");
		auto bso = shared::BinarySerializerOptions();
		mClientSerializer = shared::BinarySerializer<shared::ClientMessage>(bso);
		mFw = client::FileWatcher([this](std::vector<shared::Event> eventList) {
			this->onDirectoryChange(eventList);
		}, mConfig.getDirectory());
	}

	App::~App() {
	}

	void App::onDirectoryChange(std::vector<shared::Event> eventList) {
		std::cout << "Processing next client update..." << std::endl;
		client::Config globalConfig("../data/config.conf");
		boost::asio::io_service ios;
		std::string host = globalConfig.getIp();
		int port = globalConfig.getPort();
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port);
		boost::asio::ip::tcp::socket socket(ios);
		socket.connect(endpoint);

		// Begin communicaton with the server: Write exactly the amount of data that we need and expect the 
		// start communication in response
		shared::ClientMessageData startCommData;
		startCommData.type = shared::ClientMessageType::RequestStartComm;
		startCommData.numberOfMessages = eventList.size();
		shared::ClientMessage startComm(startCommData);
		startComm.serialize(&mClientSerializer);
		boost::system::error_code error;
		socket.wait(socket.wait_write);
		socket.write_some(
			boost::asio::buffer(
				mClientSerializer.getData(), 
				mClientSerializer.getSize()
			),  error);

		// Receiving a start communicaton message from the server
		socket.wait(socket.wait_read);
		size_t len = socket.read_some(boost::asio::buffer(mResponseBuffer, 1024), error);
		shared::BinaryDeserializer<shared::ServerMessage> mServerSerializer({ mResponseBuffer, len, 0 });
		shared::ServerMessage response;
		mServerSerializer.deserialize(response);

		if (response.getData().type != shared::ServerMessageType::ResponseStartComm) {
			std::cerr << "Invalid ResponseStartComm message" << std::endl;
			socket.close();
			return;
		}

		std::cout << "ResponseStartComm: Hash - " << response.getData().hash << ", Version - " << response.getData().version << std::endl;
		
		// Start sending the events
		for (auto event : eventList) {
			shared::ClientMessageData fileUpdateData;
			fileUpdateData.type = shared::ClientMessageType::ChangeEvent;
			fileUpdateData.event = event;
			fileUpdateData.hash = shared::getHash(0, event);
			switch (event.type) {
			case shared::EventType::Created: {
				std::cout << "Created file: " << event.path << std::endl;
				break;
			}
			case shared::EventType::Modified: {
				std::cout << "Modified file: " << event.path << std::endl;
				break;
			}
			case shared::EventType::Deleted: {
				std::cout << "Deleted file: " << event.path << std::endl;
				break;
			}
			default:
				std::cerr << "Unknown event type, ignoring: " << (int)event.type << std::endl;
				break;
			}

			std::cout << "Hash: " << fileUpdateData.hash << std::endl;

			shared::ClientMessage fileUpdate(fileUpdateData);

			socket.wait(socket.wait_write);
			std::string message = "";
			boost::array<char, 128> buf;
			std::copy(message.begin(), message.end(), buf.begin());
			boost::system::error_code error;
			socket.write_some(boost::asio::buffer(buf, message.size()), error);
		}

		// Tell the server that we won't be sending data anymore and close the connection
		mClientSerializer.reset();
		shared::ClientMessageData endCommData;
		endCommData.type = shared::ClientMessageType::RequestEndComm;
		shared::ClientMessage endComm(endCommData);
		endComm.serialize(&mClientSerializer);
		socket.wait(socket.wait_write);
		socket.write_some(
			boost::asio::buffer(
				mClientSerializer.getData(), 
				mClientSerializer.getSize()
			),  error);

		std::cout << "Closing connection." << std::endl;
		socket.close();	
		return;
	}

};
