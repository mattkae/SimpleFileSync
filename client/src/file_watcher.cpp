
#include "file_watcher.hpp"

namespace client {

	void start(FileWatcher* fw) {
		fw->begin();
	}

	FileWatcher::FileWatcher(FileWatchFunc callback, std::string directory) {
		mDirectory = directory;
		mCallback = callback;
		delay = std::chrono::milliseconds(5000);
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
			std::this_thread::sleep_for(delay);

			auto it = mPaths.begin();
			while (it != mPaths.end()) {
				if (!std::filesystem::exists(it->first)) {
					const std::filesystem::path filePath = it->first;
					shared::Event event;
					event.type = shared::EventType::Deleted;
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
					event.path = pathname;
					event.timeModifiedUtcMs = file.last_write_time().time_since_epoch().count();
					eventList.push_back(event);
				} else {
					if(mPaths[file.path()] != lastWriteTime) {
						mPaths[file.path()] = lastWriteTime;
						shared::Event event;
						event.type = shared::EventType::Modified;
						event.path = pathname;
						event.timeModifiedUtcMs = file.last_write_time().time_since_epoch().count();
						eventList.push_back(event);
					}
				}
			}

			mCallback(eventList);
		}
	}

	void FileWatcher::end() {
		mIsRunning = false;
	}
};
