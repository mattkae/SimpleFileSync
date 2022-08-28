
#include "file_watcher.hpp"

namespace client {

	void start(FileWatcher* fw) {
		fw->begin();
	}

	FileWatcher::FileWatcher(FileWatchFunc callback, std::string directory, shared::i32 updateIntervalSeconds) {
		mDirectory = directory;
		mCallback = callback;
		delay = std::chrono::milliseconds(updateIntervalSeconds * 1000);
		begin();
	}

	FileWatcher::~FileWatcher() {
		end();
	}

	void FileWatcher::begin() {
		mIsRunning = true;

		for(auto &file : std::filesystem::recursive_directory_iterator(mDirectory)) {
		    mPaths[file.path().string()] = std::filesystem::last_write_time(file);
		}

		std::vector<shared::Event> eventList;
		while (mIsRunning) {
			eventList.clear();

			auto it = mPaths.begin();
			while (it != mPaths.end()) {
				if (!std::filesystem::exists(it->first)) {
					const std::filesystem::path filePath = it->first;
					shared::Event event;
					event.type = shared::EventType::Deleted;
					event.fullpath = filePath;
					event.path = filePath.filename();
					eventList.push_back(event);
					it = mPaths.erase(it);
				}
				else {
					it++;
				}
			}
 
			// Check if a file was created or modified
			for(auto &file : std::filesystem::recursive_directory_iterator(mDirectory)) {
				auto lastWriteTime = std::filesystem::last_write_time(file);
				auto pathname = file.path().filename();
	 
				if(!mPaths.contains(file.path())) {
					mPaths[file.path()] = lastWriteTime;
					shared::Event event;
					event.type = shared::EventType::Created;
					event.fullpath = file.path();
					event.path = pathname;
					event.timeModifiedUtcMs = file.last_write_time().time_since_epoch().count();
					eventList.push_back(event);
				} else {
					if(mPaths[file.path()] != lastWriteTime) {
						mPaths[file.path()] = lastWriteTime;
						shared::Event event;
						event.type = shared::EventType::Modified;
						event.fullpath = file.path();
						event.path = pathname;
						event.timeModifiedUtcMs = file.last_write_time().time_since_epoch().count();
						eventList.push_back(event);
					}
				}
			}

			mCallback(eventList);

			std::this_thread::sleep_for(delay);
		}
	}

	void FileWatcher::end() {
		mIsRunning = false;
	}
};
