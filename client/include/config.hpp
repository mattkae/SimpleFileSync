
#pragma once
#include <string>
#include "base_config.hpp"

namespace client {
	class Config: public shared::BaseConfig {
	public:
		Config() :shared::BaseConfig() { }
		Config(std::string mPath) : shared::BaseConfig(mPath) { }
		bool processToken(std::string key, std::string value) override;
		void writeTokens(std::ofstream& writer) override;
		std::string getDirectory();
		std::string getIp();
		int getPort();
		bool useSsl() { return mUseSsl; };
		int getUpdateInterval();
	private:
		std::string mConfigPath;
		
		std::string mIp = "127.0.0.1";
		int mPort = 3490;
		std::string mWatchDirectory = "./files";
		bool mUseSsl = false;
		int mUpdateIntervalSeconds = 10;
	};
};

