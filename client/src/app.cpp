#include "app.hpp"
#include "client_message.hpp"
#include "config.hpp"
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <filesystem>
#include "file_watcher.hpp"
#include "serializer.hpp"
#include "server_message.hpp"

namespace client {
	App::App() {
		mConfig = client::Config("../data/config.conf");
		auto bso = shared::BinarySerializerOptions();
		mClientSerializer = shared::BinarySerializer<shared::ClientMessage>(bso);
		mFw = client::FileWatcher([this](std::vector<client::FileChangeEvent> eventList) {
			this->onDirectoryChange(eventList);
		}, mConfig.getDirectory());
	}

	App::~App() {
	}

	void App::onDirectoryChange(std::vector<client::FileChangeEvent> eventList) {
		std::cout << "Processing next client update..." << std::endl;
		client::Config globalConfig("../data/config.conf");
		boost::asio::io_service ios;
		std::string host = globalConfig.getIp();
		int port = globalConfig.getPort();
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port);
		boost::asio::ip::tcp::socket socket(ios);
		socket.connect(endpoint);

		shared::ClientMessageData statusRequestData;
		statusRequestData.type = shared::ClientMessageType::RequestStatus;
		shared::ClientMessage statusRequest(statusRequestData);
		statusRequest.serialize(&mClientSerializer);
		boost::system::error_code error;
		socket.write_some(
			boost::asio::buffer(
				mClientSerializer.getData(), 
				mClientSerializer.getSize()
			),  error);

		size_t len = socket.read_some(boost::asio::buffer(mResponseBuffer, 1024), error);
		std::cout << "Received " << len << " bytes of data." << std::endl;
		shared::BinaryDeserializer<shared::ServerMessage> mServerSerializer({ mResponseBuffer, len });
		auto response = mServerSerializer.deserialize();

		if (response.getData().type != shared::ServerMessageType::ResponseStatus) {
			std::cerr << "Invalid ResponseStatus message" << std::endl;
			return;
		}

		std::cout << "Response Status: Hash - " << response.getData().hash << ", Version - " << response.getData().version << std::endl;
		
		for (auto event : eventList) {
			switch (event.type) {
			case client::FileChangeType::Created: {
				std::cout << "Created file: " << event.filePath << std::endl;
				break;
			}
			case client::FileChangeType::Modified: {
				std::cout << "Modified file: " << event.filePath << std::endl;
				break;
			}
			case client::FileChangeType::Deleted: {
				std::cout << "Deleted file: " << event.filePath << std::endl;
				break;
			}
			default:
				std::cerr << "Unkwown event type, ignoring: " << (int)event.type << std::endl;
				break;
			}

			std::string message = "";
			boost::array<char, 128> buf;
			std::copy(message.begin(), message.end(), buf.begin());
			boost::system::error_code error;
			socket.write_some(boost::asio::buffer(buf, message.size()), error);
			socket.close();
		}
	

		return;
	}

};
