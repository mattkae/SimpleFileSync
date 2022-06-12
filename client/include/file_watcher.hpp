#pragma once
#include <string>
#include <chrono>
#include <functional>
#include <thread>
#include <filesystem>
#include <unordered_map>

namespace client {
	typedef std::function<void()> FileWatchFunc;

	class FileWatcher {
	public:
		FileWatcher(FileWatchFunc callback, std::string directory);
		~FileWatcher();
		void begin();
		void end();
	private:
		std::string mDirectory;
		FileWatchFunc mCallback;
		std::thread mThread;
		bool mIsRunning = false;
		std::chrono::duration<int, std::milli> delay;
		std::unordered_map<std::string, std::filesystem::file_time_type> mPaths;
	};
}
