#include "app.hpp"
#include "client_message.hpp"
#include "config.hpp"
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <cstddef>
#include <iostream>
#include <filesystem>
#include <iterator>
#include <string>
#include "file_watcher.hpp"
#include "save_area.hpp"
#include "serializer.hpp"
#include "server_message.hpp"
#include "hash_calculator.hpp"

namespace client {
	App::App() {
		mAppData = shared::State(shared::getSaveAreaPath("client_saved.data"));
		mAppData.load();

		mConfig = client::Config(shared::getSaveAreaPath("client.conf"));
		mConfig.load();
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
		client::Config globalConfig(shared::getSaveAreaPath("client.conf"));
		globalConfig.load();
		boost::asio::io_service ios;
		std::string host = globalConfig.getIp();
		int port = globalConfig.getPort();
		std::cout << "Making connection to " << host << ":" << port << std::endl;
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port);
		boost::asio::ip::tcp::socket socket(ios);
		socket.connect(endpoint);

		// Begin communicaton with the server: Write exactly the amount of data that we need and expect the 
		// start communication in response
		shared::ClientMessageData startCommData;
		startCommData.type = shared::ClientMessageType::RequestStartComm;
		startCommData.numberOfMessages = eventList.size();
		shared::ClientMessage startComm(startCommData);
		mClientSerializer.reset();
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
			fileUpdateData.hash = shared::getHash(mAppData.hash, event);
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

			std::cout << "Writing new hash to disk: " << fileUpdateData.hash << "..." << std::endl;
			mAppData.hash = fileUpdateData.hash;
			mAppData.write();
			std::cout << "Hash written to disk." << std::endl;

			shared::ClientMessage fileUpdate(fileUpdateData);
			socket.wait(socket.wait_write);
			mClientSerializer.reset();
			fileUpdate.serialize(&mClientSerializer);
			socket.wait(socket.wait_write);
			socket.write_some(
				boost::asio::buffer(
					mClientSerializer.getData(), 
					mClientSerializer.getSize()
				),  error);
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

		std::cout << "Closing connection." << std::endl << std::endl;
		socket.close();	
		return;
	}

};
