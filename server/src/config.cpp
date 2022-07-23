#include "config.hpp"
#include <string>

namespace server {
	const std::string DIRECTORY = "directory";
	const std::string MAX_REVISIONS = "maxRevisions";
	const std::string PORT = "port";
	bool Config::processToken(std::string key, std::string value) {
		if (key == DIRECTORY) {
			mWatchDirectory = value;
			return true;
		}
		else if (key == MAX_REVISIONS) {
			mMaxRevisions = std::stoi(value);
			return true;
		}
		else if (key == PORT) {
			mPort = std::stoi(value);
			return true;
		}
		else {
			return false;
		}	
	}

	std::string Config::getDirectory() { return mWatchDirectory; }
	int Config::getPort() { return mPort; }
};
