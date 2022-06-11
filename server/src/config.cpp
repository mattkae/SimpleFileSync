#include "config.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>

const std::string DIRECTORY = "directory";
const std::string MAX_REVISIONS = "maxRevisions";

namespace server {
	Config::Config(std::string configPath) {
		mConfigPath = configPath;
		load();
	}

	Config::~Config() {
		
	}

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
			else if (key == MAX_REVISIONS) {
				mMaxRevisions = std::stoi(value);
			}
			else {
				std::cerr << "Unknown key option (" << mConfigPath << " #" << lineNumber << ")" << std::endl;
			}				 	
		}

		reader.close();

		std::cout << "Watching directory " << mWatchDirectory << " with a maximum of " << mMaxRevisions << " revision(s)." << std::endl;

		return true;
	}
};
