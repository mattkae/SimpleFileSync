#include "config.hpp"


namespace client {
	const std::string DIRECTORY = "directory";
	const std::string PORT = "port";
	const std::string IP = "ip";

	bool Config::processToken(std::string key, std::string value) {
		if (key == DIRECTORY) {
			mWatchDirectory = value;
			return true;
		}
		else if (key == IP) {
			mIp = value;
			return true;
		} else if (key == PORT) {
			mPort = stoi(value);
			return true;
		}
		else {
			return false;
		}
	}

	std::string Config::getDirectory() { return mWatchDirectory; };
	std::string Config::getIp() { return mIp; }
	int Config::getPort() { return mPort; }
};
