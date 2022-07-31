#pragma once
#include "base_config.hpp"
#include <string>
namespace server {
	class Config: public shared::BaseConfig {
	public:
		Config() :shared::BaseConfig() { }
		Config(std::string mPath) : shared::BaseConfig(mPath) { }
		bool processToken(std::string key, std::string value) override;
		void writeTokens(std::ofstream& writer) override;
		std::string getDirectory();
		int getPort();
	private:
		std::string mConfigPath;

		std::string mWatchDirectory;
		int mMaxRevisions;
		int mPort;
	};
};
