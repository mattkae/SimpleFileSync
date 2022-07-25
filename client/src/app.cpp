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
#include "event.hpp"
#include "event_ledger.hpp"
#include "file_watcher.hpp"
#include "save_area.hpp"
#include "serializer.hpp"
#include "server_message.hpp"
#include "hash_calculator.hpp"
#include <spdlog/spdlog.h>

namespace client {
	App::App(): 
		mLedger(shared::getSaveAreaPath(".client_events")),
		mAppData(shared::getSaveAreaPath("client_saved.data")),
		mConfig(shared::getSaveAreaPath("client.conf"))
	{
		mAppData.load();
		mConfig.load();
		auto bso = shared::BinarySerializerOptions();
		mClientSerializer = shared::BinarySerializer<shared::ClientMessage>(bso);
		mFw = client::FileWatcher([this](std::vector<shared::Event> eventList) {
			try {
				this->onDirectoryChange(eventList);
			}
			catch (boost::wrapexcept<boost::system::system_error> e) {
				spdlog::error("Failed to connect to socket: {0}", e.what());
			}
		}, mConfig.getDirectory());
	}

	App::~App() {
	}

	void App::onDirectoryChange(std::vector<shared::Event> eventList) {
		spdlog::info("Processing next client update...");
		client::Config globalConfig(shared::getSaveAreaPath("client.conf"));
		globalConfig.load();
		boost::asio::io_service ios;
		std::string host = globalConfig.getIp();
		int port = globalConfig.getPort();
		spdlog::info("Making connection to {0}: {1}", host, port);
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port);
		boost::asio::ip::tcp::socket socket(ios);
		socket.connect(endpoint);

		// Begin communicaton with the server: Write exactly the amount of data that we need and expect the 
		// start communication in response
		shared::ClientMessageData startCommData;
		startCommData.type = shared::ClientMessageType::RequestStartComm;
		startCommData.hash = mAppData.getHash();
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

		switch (response.getData().type) {
			case shared::ServerMessageType::ResponseStartComm:
				break;
			default:
				spdlog::error("Invalid initial response from server: type={0}", (int)response.getData().type);
				socket.close();
				return;
		}

		// Start sending the events
		for (auto event : eventList) {
			shared::ClientMessageData fileUpdateData;
			fileUpdateData.type = shared::ClientMessageType::ChangeEvent;
			fileUpdateData.event = event;
			fileUpdateData.hash = shared::getHash(mAppData.getHash(), event);
			event.hash = fileUpdateData.hash;
			switch (event.type) {
			case shared::EventType::Created: {
				spdlog::info("Created file: {0}", event.path);
				break;
			}
			case shared::EventType::Modified: {
				spdlog::info("Modified file: {0}", event.path);
				break;
			}
			case shared::EventType::Deleted: {
				spdlog::info("Deleted file: {0}", event.path);
				break;
			}
			default:
				spdlog::error("Unknown event type, ignoring: {0}", (int)event.type);
				break;
			}

			spdlog::info("Writing new hash to disk: {0} ...", fileUpdateData.hash);
			mAppData.addHash(fileUpdateData.hash);
			mAppData.write();
			spdlog::info("Has written to disk.");
			mLedger.record(event);

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

		spdlog::info("Closing connection.");
		socket.close();	
		return;
	}

};
