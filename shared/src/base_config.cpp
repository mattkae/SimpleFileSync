#include "base_config.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <filesystem>
#include <spdlog/spdlog.h>

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
            spdlog::error("Failed to initialize config: {0}", ex.what());
        }
    }


    bool BaseConfig::load() {
        std::string line;
        std::ifstream reader(mConfigPath);
        if (!reader) {
            spdlog::error("Failed to load config");
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
                spdlog::warn("Unknown key option, config={0}, line={1} key={2}, value={3}", mConfigPath, lineNumber, key, value);
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