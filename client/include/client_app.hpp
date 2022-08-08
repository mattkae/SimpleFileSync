#pragma once
#include "config.hpp"
#include "serializer.hpp"
#include "deserializer.hpp"
#include "client_message.hpp"
#include "file_watcher.hpp"
#include "server_message.hpp"
#include "event_ledger.hpp"
#include "event.hpp"
#include "state.hpp"
#include <cstddef>
#include <string>

namespace client {
	struct ClientOptions {
		bool blankSlate = false;
	};

	class ClientApp{
	public:
		ClientApp(const ClientOptions& opts);
		~ClientApp();

		int getCurrentVersion();
		void bumpCurrentVersion();
		
	private:
		void onDirectoryChange(std::vector<shared::Event> eventList);
		void addNewEvent(shared::Event& event);
		
		client::Config mConfig;
		shared::BinarySerializer mClientSerializer;
		client::FileWatcher mFw;

		shared::State mAppData;
		shared::EventLedger mLedger;
	};
};
