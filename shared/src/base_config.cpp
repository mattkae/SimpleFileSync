#include "base_config.hpp"
#include "logger.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <filesystem>

namespace shared {
    BaseConfig::BaseConfig() { }
    
    BaseConfig::BaseConfig(std::string configPath) {
        mConfigPath = configPath;

        try {
            auto path = std::filesystem::path(configPath);
            std::string parentPath = path.parent_path();
            if (!std::filesystem::exists(parentPath)) {
                std::filesystem::create_directories(parentPath);
            }

            if (!std::filesystem::exists(configPath)) {
                std::ofstream ofs(configPath);
                ofs.close();
            }
        }
        catch(std::filesystem::filesystem_error const& ex) {
            logger_error("Failed to initialize config from path %s because %s", configPath.c_str(), ex.what());
        }
    }


    bool BaseConfig::load() {
        std::string line;
        std::ifstream reader(mConfigPath);
        if (!reader) {
            logger_error("Failed to load config");
            return false;
        }

        int lineNumber = 0;
        while (!reader.eof()) {
            std::getline(reader, line);
            lineNumber++;
            
            std::string key = "";
            std::string value = "";
            
            std::istringstream iss(line);
            std::vector<std::string> results(std::istream_iterator<std::string>{iss},
                                            std::istream_iterator<std::string>());

            if (results.size() != 2) {
                continue;
            }

            key = results[0];
            value = results[1];

            if (!processToken(key, value)) {
                logger_warning("Unknown key option, config=%s, line=%d, key=%s, value=%s", mConfigPath.c_str(), lineNumber, key.c_str(), value.c_str());
            }			 	
        }

        reader.close();
        return true;
    }

    void BaseConfig::write() {
        std::ofstream writer(mConfigPath);
        writeTokens(writer);
        writer.close();
    }
}