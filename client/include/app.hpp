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
	const int DEFAULT_BUF_SIZE = 256;

	class App{
	public:
		App();
		~App();

		int getCurrentVersion();
		void bumpCurrentVersion();
		
	private:
		void onDirectoryChange(std::vector<shared::Event> eventList);
		shared::Byte mResponseBuffer[DEFAULT_BUF_SIZE];
		
		client::Config mConfig;
		shared::BinarySerializer mClientSerializer;
		client::FileWatcher mFw;

		shared::State mAppData;
		shared::EventLedger mLedger;
	};
};
