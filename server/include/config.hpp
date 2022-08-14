#pragma once
#include "base_config.hpp"
#include "server_socket.hpp"
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
		bool useSsl() { return mUseSsl; };
		SslOptions getSslOptions() { return mSslOptions; };
	private:
		std::string mConfigPath;

		std::string mWatchDirectory;
		int mMaxRevisions;
		int mPort;
		bool mUseSsl;
		SslOptions mSslOptions;
	};
};
