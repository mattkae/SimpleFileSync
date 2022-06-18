#pragma once
#include "config.hpp"
#include "serializer.hpp"
#include "message.hpp"
#include "file_watcher.hpp"
#include <string>

namespace client {
	class App{
	public:
		App();
		~App();

		int getCurrentVersion();
		void bumpCurrentVersion();
		
		std::string getCurrentHash();
		void calculateNextHash();
		
	private:
		void mOnDirectoryChange(client::FileChangeEvent event);
		
		client::Config mConfig;
		shared::BinarySerializer<shared::ClientMessage> mSerializer;
		client::FileWatcher mFw;

		int mVersion = -1;
		std::string mHash = "";
	};
};
