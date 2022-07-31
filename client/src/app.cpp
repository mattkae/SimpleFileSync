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
	App::App(const AppOptions& opts): 
		mLedger(shared::getSaveAreaPath(".client_events"), opts.blankSlate),
		mAppData(shared::getSaveAreaPath(".client_saved.data"), opts.blankSlate),
		mConfig(shared::getSaveAreaPath("client.conf"))
	{
		mAppData.load();
		mConfig.load();
		auto bso = shared::BinarySerializerOptions();
		mClientSerializer = shared::BinarySerializer(bso);
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
		shared::ClientMessage startMessage;
		startMessage.type = shared::ClientMessageType::RequestStartComm;
		startMessage.event.hash = mAppData.getHash();
		mClientSerializer.reset();
		mClientSerializer.writeObject(startMessage);
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
		shared::BinaryDeserializer mServerSerializer({ mResponseBuffer, len, 0 });
		shared::ServerMessage response = mServerSerializer.readObject<shared::ServerMessage>();

		switch (response.type) {
			case shared::ServerMessageType::ResponseStartComm:
				if (response.eventsForClient.size()) {
					spdlog::info("Client is behind by {0} events.", response.eventsForClient.size());
					for (auto event : response.eventsForClient) {
						shared::executeEvent(event, mConfig.getDirectory());
						addNewEvent(event);
					}
					spdlog::info("Client caught up.");
				}
				break;
			default:
				spdlog::error("Invalid initial response from server: type={0}", (int)response.type);
				socket.close();
				return;
		}

		// Start sending the events
		for (auto event : eventList) {
			shared::ClientMessage fileUpdateMsg;
			fileUpdateMsg.type = shared::ClientMessageType::ChangeEvent;
			fileUpdateMsg.event = event;
			fileUpdateMsg.event.hash = shared::getHash(mAppData.getHash(), event);
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

			addNewEvent(event);

			socket.wait(socket.wait_write);
			mClientSerializer.reset();
			mClientSerializer.writeObject(fileUpdateMsg);
			socket.wait(socket.wait_write);
			socket.write_some(
				boost::asio::buffer(
					mClientSerializer.getData(), 
					mClientSerializer.getSize()
				),  error);
		}

		// Tell the server that we won't be sending data anymore and close the connection
		mClientSerializer.reset();
		shared::ClientMessage endCommMsg;
		endCommMsg.type = shared::ClientMessageType::RequestEndComm;
		mClientSerializer.writeObject(endCommMsg);
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

	void App::addNewEvent(shared::Event& event) {
		mAppData.addHash(event.hash);
		mAppData.write();
		mLedger.record(event);
	}
};
