#include "config.hpp"
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>
#include <fstream>


namespace client {
	const std::string DIRECTORY = "directory";
	const std::string PORT = "port";
	const std::string IP = "ip";

	Config::Config(std::string configPath) {
		mConfigPath = configPath;
		load();
	}

	Config::~Config() { }

	bool Config::load() {
		std::string line;
		std::ifstream reader(mConfigPath);
		if (!reader) {
			// TODO: log error
			return false;
		}

		int lineNumber = 0;
		while (!reader.eof()) {
			std::getline(reader, line);
			lineNumber++;
			
			std::string key;
			std::string value;
			
		    std::istringstream iss(line);
			std::vector<std::string> results(std::istream_iterator<std::string>{iss},
											 std::istream_iterator<std::string>());

			if (results.size() != 2) {
				std::cerr << "Unknown config option (" << mConfigPath << " #" << lineNumber << ")" << std::endl;
				continue;
			}

			key = results[0];
			value = results[1];

			if (key == DIRECTORY) {
				mWatchDirectory = value;
			}
			else if (key == IP) {
				mIp = value;
			} else if (key == PORT) {
				mPort = stoi(value);
			}
			else {
				std::cerr << "Unknown key option (" << mConfigPath << " #" << lineNumber << ")" << std::endl;
			}				 	
		}

		reader.close();

		std::cout << "Configuration:" << std::endl;
		std::cout << "\tWatch directory: " << mWatchDirectory << std::endl;
		std::cout << "\tIP: " << mIp << std::endl;
		std::cout << "\tPort: " << mPort << std::endl;
		return true;
	}

	std::string Config::getDirectory() { return mWatchDirectory; };
	std::string Config::getIp() { return mIp; }
	int Config::getPort() { return mPort; }
};
