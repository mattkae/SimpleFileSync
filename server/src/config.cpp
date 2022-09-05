#include "config.hpp"
#include <string>

namespace server {
	const std::string DIRECTORY = "directory";
	const std::string PORT = "port";
	const std::string USE_SSL = "useSsl";

	const std::string SSL_PUBLIC_KEY_FILE = "ssl_publicKey";
	const std::string SSL_PRIVATE_KEY_FILE = "ssl_privateKey";

	bool Config::processToken(std::string key, std::string value) {
		if (key == DIRECTORY) {
			mWatchDirectory = value;
			return true;
		}
		else if (key == PORT) {
			mPort = std::stoi(value);
			return true;
		}
		else if (key == USE_SSL) {
			mUseSsl = value == "true";
			return true;
		}
		else if (key == SSL_PUBLIC_KEY_FILE) {
			mSslOptions.certChainFile = value;
			return true;
		}
		else if (key == SSL_PRIVATE_KEY_FILE) {
			mSslOptions.privateKeyFile = value;
			return true;
		}
		else {
			return false;
		}
	}

	void Config::writeTokens(std::ofstream& writer) {
		writeToken(writer, DIRECTORY, mWatchDirectory);
		writeToken(writer, PORT, mPort);
	}

	std::string Config::getDirectory() { return mWatchDirectory; }
	int Config::getPort() { return mPort; }
};
