#pragma once
#include <string>
#include <chrono>
#include <functional>
#include <thread>
#include <filesystem>
#include <unordered_map>

namespace client {
	enum class FileChangeType {
		None = 0,
		Created,
		Modified,
		Deleted
	};

	struct FileChangeEvent {
		FileChangeType type;
		std::string filePath;
	};

	typedef std::function<void(FileChangeEvent)> FileWatchFunc;

	class FileWatcher {
	public:
		FileWatcher() { }
		FileWatcher(FileWatchFunc callback, std::string directory);
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
