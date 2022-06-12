
#pragma once
#include <string>
namespace client {
	class Config {
	public:
		Config(std::string configPath);
		~Config();

		bool load();
		std::string getDirectory();
		std::string getIp();
		int getPort();
	private:
		std::string mConfigPath;
		
		std::string mIp;
		int mPort;
		std::string mIdentifier;
		std::string mWatchDirectory;
	};
};

