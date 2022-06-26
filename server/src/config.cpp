#include "config.hpp"

namespace server {
	const std::string DIRECTORY = "directory";
	const std::string MAX_REVISIONS = "maxRevisions";
	bool Config::processToken(std::string key, std::string value) {
		if (key == DIRECTORY) {
			mWatchDirectory = value;
			return true;
		}
		else if (key == MAX_REVISIONS) {
			mMaxRevisions = std::stoi(value);
			return true;
		}
		else {
			return false;
		}	
	}

	std::string Config::getDirectory() { return mWatchDirectory; }
};
