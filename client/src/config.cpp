#include "config.hpp"


namespace client {
	const std::string DIRECTORY = "directory";
	const std::string PORT = "port";
	const std::string IP = "ip";
	const std::string USE_SSL = "useSsl";
	const std::string UPDATE_INTERVAL_SECONDS = "updateIntervalSeconds";

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
		else if (key == USE_SSL) {
			mUseSsl = value == "true";
			return true;
		}
		else if (key == UPDATE_INTERVAL_SECONDS) {
			mUpdateIntervalSeconds = stoi(value);
			return true;
		}
		else {
			return false;
		}
	}

	void Config::writeTokens(std::ofstream& writer) {
		writeToken(writer, DIRECTORY, mWatchDirectory);
		writeToken(writer, IP, mIp);
		writeToken(writer, PORT, mPort);
	}

	std::string Config::getDirectory() { return mWatchDirectory; };
	std::string Config::getIp() { return mIp; }
	int Config::getPort() { return mPort; }
	int Config::getUpdateInterval() { return mUpdateIntervalSeconds; }
};
