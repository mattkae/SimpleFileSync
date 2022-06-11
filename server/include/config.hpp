#pragma once
#include <string>
namespace server {
	class Config {
	public:
		Config(std::string configPath);
		~Config();

		bool load();
		void beginWatch();
		void endWatch();
		std::string getDirectory();
	private:
		std::string mConfigPath;

		std::string mWatchDirectory;
		int mMaxRevisions;
	};
};
