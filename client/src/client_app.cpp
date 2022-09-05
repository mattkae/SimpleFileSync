#include <cstddef>
#include <filesystem>
#include <iterator>
#include <string>
#include "client_app.hpp"
#include "client_message.hpp"
#include "config.hpp"
#include "event.hpp"
#include "event_ledger.hpp"
#include "file_watcher.hpp"
#include "serializer.hpp"
#include "server_message.hpp"
#include "hash_calculator.hpp"
#include "client_socket.hpp"
#include "environment.hpp"
#include "logger.hpp"


namespace client {
	ClientApp::ClientApp(const ClientOptions& opts): 
		mConfig(shared::Environment::get().getConfigDirectory() + "client.conf"),
		mAppData(shared::Environment::get().getDataDirectory() + ".client_saved.data", opts.blankSlate),
		mLedger(shared::Environment::get().getDataDirectory() + ".client_events", opts.blankSlate)
	{
		mAppData.load();
		mConfig.load();
		auto bso = shared::BinarySerializerOptions();
		mClientSerializer = shared::BinarySerializer(bso);
		mFw = client::FileWatcher([this](const std::vector<shared::Event>& eventList) {
			try {
				this->onDirectoryChange(eventList);
			}
			catch (const std::exception& e) {
				logger_error("Failed to connect to socket: %s", e.what());
			}
		}, mConfig.getDirectory(), mConfig.getUpdateInterval());
	}

	ClientApp::~ClientApp() {
	}

	void ClientApp::onDirectoryChange(const std::vector<shared::Event>& eventList) {
		logger_info("Processing next client update...");
		std::string host = mConfig.getIp();
		int port = mConfig.getPort();
		logger_info("Making connection to %s:%d", host.c_str(), port);

		ClientSocket socket({ host, static_cast<uint_least16_t>(port), mConfig.useSsl() });

		// Begin communicaton with the server: Write exactly the amount of data that we need and expect the 
		// start communication in response
		shared::ClientMessage startMessage;
		startMessage.type = shared::ClientMessageType::RequestStartComm;
		startMessage.event.hash = mAppData.getHash();
		mClientSerializer.reset();
		mClientSerializer.writeObject(startMessage);
		socket.write(mClientSerializer.getData(), mClientSerializer.getSize());
		logger_info("Client is currently at hash: %lu", startMessage.event.hash);

		// Receiving a start communicaton message from the server
		auto socketRead = socket.read();
		shared::BinaryDeserializer mServerSerializer({ socketRead.data, static_cast<shared::u64>(socketRead.len), 0 });
		shared::ServerMessage response = mServerSerializer.readObject<shared::ServerMessage>();

		switch (response.type) {
			case shared::ServerMessageType::ResponseStartComm:
				if (response.eventsForClient.size()) {
					logger_info("Client is behind by %d events.", response.eventsForClient.size());
					for (auto event : response.eventsForClient) {
						shared::executeEvent(event, mConfig.getDirectory());
						addNewEvent(event);
					}
					logger_info("Client caught up.");
				}
				else {
					logger_info("Client already caught up.");
				}
				break;
			default:
				logger_error("Invalid initial response from server: type=%d", (int)response.type);
				return;
		}

		// Start sending the events
		for (auto event : eventList) {
			shared::ClientMessage fileUpdateMsg;
			fileUpdateMsg.type = shared::ClientMessageType::ChangeEvent;
			event.hash = shared::getHash(mAppData.getHash(), event);
			fileUpdateMsg.event = event;
			switch (event.type) {
			case shared::EventType::Created: {
				logger_info("Created file: %s", event.path.c_str());
				break;
			}
			case shared::EventType::Modified: {
				logger_info("Modified file: %s", event.path.c_str());
				break;
			}
			case shared::EventType::Deleted: {
				logger_info("Deleted file: %s", event.path.c_str());
				break;
			}
			default:
				logger_error("Unknown event type, ignoring: %d", (int)event.type);
				break;
			}

			addNewEvent(event);

			mClientSerializer.reset();
			mClientSerializer.writeObject(fileUpdateMsg);
			socket.write(mClientSerializer.getData(), mClientSerializer.getSize());
		}

		// Tell the server that we won't be sending data anymore and close the connection
		mClientSerializer.reset();
		shared::ClientMessage endCommMsg;
		endCommMsg.type = shared::ClientMessageType::RequestEndComm;
		mClientSerializer.writeObject(endCommMsg);
		socket.write(mClientSerializer.getData(), mClientSerializer.getSize());
		return;
	}

	void ClientApp::addNewEvent(shared::Event& event) {
		mAppData.addHash(event.hash);
		mAppData.write();
		mLedger.record(event);
	}
};
