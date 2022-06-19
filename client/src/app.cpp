#include "app.hpp"
#include "config.hpp"
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <filesystem>
#include "file_watcher.hpp"
#include "serializer.hpp"

namespace client {
	App::App() {
		mConfig = client::Config("../data/config.conf");
		auto bso = shared::BinarySerializerOptions();
		mSerializer = shared::BinarySerializer<shared::ClientMessage>(bso);
		mFw = client::FileWatcher([this](client::FileChangeEvent event) {
			this->mOnDirectoryChange(event);
		}, mConfig.getDirectory());
	}

	App::~App() {
	}

	void App::mOnDirectoryChange(client::FileChangeEvent event) {
		client::Config globalConfig("../data/config.conf");
		std::cout << "Processing change event." << std::endl;
	
		boost::asio::io_service ios;
		std::string host = globalConfig.getIp();
		int port = globalConfig.getPort();
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port);
		boost::asio::ip::tcp::socket socket(ios);
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


		socket.connect(endpoint);

		std::string message = "";
		boost::array<char, 128> buf;
		std::copy(message.begin(), message.end(), buf.begin());
		boost::system::error_code error;
		socket.write_some(boost::asio::buffer(buf, message.size()), error);
		socket.close();

		return;
	}

};
