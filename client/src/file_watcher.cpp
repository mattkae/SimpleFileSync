
#include "file_watcher.hpp"

namespace client {

	void start(FileWatcher* fw) {
		fw->begin();
	}

	FileWatcher::FileWatcher(FileWatchFunc callback, std::string directory) {
		mDirectory = directory;
		mCallback = callback;
		mThread = std::thread(start, this);
		delay = std::chrono::milliseconds(5000);
	}

	FileWatcher::~FileWatcher() {
		end();
	}

	void FileWatcher::begin() {
		mIsRunning = true;

		for(auto &file : std::filesystem::recursive_directory_iterator(mDirectory)) {
		    mPaths[file.path().string()] = std::filesystem::last_write_time(file);
		}

		while (mIsRunning) {
			std::this_thread::sleep_for(delay);

			auto it = mPaths.begin();
			while (it != mPaths.end()) {
				if (!std::filesystem::exists(it->first)) {
					mCallback();
					it = mPaths.erase(it);
				}
				else {
					it++;
				}
			}
 
			// Check if a file was created or modified
			for(auto &file : std::filesystem::recursive_directory_iterator(mDirectory)) {
				auto lastWriteTime = std::filesystem::last_write_time(file);
	 
				if(!mPaths.contains(file.path())) {
					mPaths[file.path().string()] = lastWriteTime;
					mCallback();
				} else {
					if(mPaths[file.path().string()] != lastWriteTime) {
						mPaths[file.path().string()] = lastWriteTime;
						mCallback();
					}
				}
			}
		}
	}

	void FileWatcher::end() {
		mIsRunning = false;
		mThread.join();
	}
};
