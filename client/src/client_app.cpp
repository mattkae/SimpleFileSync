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
#include "save_area.hpp"
#include "serializer.hpp"
#include "server_message.hpp"
#include "hash_calculator.hpp"
#include <spdlog/spdlog.h>
#include "client_socket.hpp"


namespace client {
	ClientApp::ClientApp(const ClientOptions& opts): 
		mConfig(shared::getSaveAreaPath("client.conf")),
		mAppData(shared::getSaveAreaPath(".client_saved.data"), opts.blankSlate),
		mLedger(shared::getSaveAreaPath(".client_events"), opts.blankSlate)
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
				spdlog::error("Failed to connect to socket: {0}", e.what());
			}
		}, mConfig.getDirectory());
	}

	ClientApp::~ClientApp() {
	}

	void ClientApp::onDirectoryChange(const std::vector<shared::Event>& eventList) {
		spdlog::info("Processing next client update...");
		client::Config globalConfig(shared::getSaveAreaPath("client.conf"));
		globalConfig.load();
		std::string host = globalConfig.getIp();
		int port = globalConfig.getPort();
		spdlog::info("Making connection to {0}:{1}", host, port);

		ClientSocket socket({ host, static_cast<uint_least16_t>(port), globalConfig.useSsl() });

		// Begin communicaton with the server: Write exactly the amount of data that we need and expect the 
		// start communication in response
		shared::ClientMessage startMessage;
		startMessage.type = shared::ClientMessageType::RequestStartComm;
		startMessage.event.hash = mAppData.getHash();
		mClientSerializer.reset();
		mClientSerializer.writeObject(startMessage);
		socket.write(mClientSerializer.getData(), mClientSerializer.getSize());

		// Receiving a start communicaton message from the server
		auto socketRead = socket.read();
		shared::BinaryDeserializer mServerSerializer({ socketRead.data, static_cast<size_t>(socketRead.len), 0 });
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
				else {
					spdlog::info("Client already caught up.");
				}
				break;
			default:
				spdlog::error("Invalid initial response from server: type={0}", (int)response.type);
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
