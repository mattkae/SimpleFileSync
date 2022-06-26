#pragma once
#include "config.hpp"
#include "serializer.hpp"
#include "deserializer.hpp"
#include "client_message.hpp"
#include "file_watcher.hpp"
#include "server_message.hpp"
#include "event.hpp"
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
		
		size_t getCurrentHash();
		
	private:
		void onDirectoryChange(std::vector<shared::Event> eventList);
		shared::Byte mResponseBuffer[DEFAULT_BUF_SIZE];
		
		client::Config mConfig;
		shared::BinarySerializer<shared::ClientMessage> mClientSerializer;
		client::FileWatcher mFw;

		int mVersion = -1;
		size_t mHash = 0;
	};
};
