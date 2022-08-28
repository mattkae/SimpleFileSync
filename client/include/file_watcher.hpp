#pragma once
#include <string>
#include <chrono>
#include <functional>
#include <thread>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include "event.hpp"
#include "type.hpp"

namespace client {
	typedef std::function<void(std::vector<shared::Event>)> FileWatchFunc;

	class FileWatcher {
	public:
		FileWatcher() { }
		FileWatcher(FileWatchFunc callback, std::string directory, shared::i32 updateIntervalSeconds);
		~FileWatcher();
		void begin();
		void end();
	private:
		std::string mDirectory;
		FileWatchFunc mCallback;
		bool mIsRunning = false;
		std::chrono::duration<int, std::milli> delay;
		std::unordered_map<std::string, std::filesystem::file_time_type> mPaths;
	};
}
