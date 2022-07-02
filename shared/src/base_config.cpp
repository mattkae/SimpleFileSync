#include "base_config.hpp"
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
            std::cout
                << "what():  " << ex.what() << '\n'
                << "path1(): " << ex.path1() << '\n'
                << "path2(): " << ex.path2() << '\n'
                << "code().value():    " << ex.code().value() << '\n'
                << "code().message():  " << ex.code().message() << '\n'
                << "code().category(): " << ex.code().category().name() << '\n';
        }
    }


    bool BaseConfig::load() {
        std::cout << "Loading configuration from path: " << mConfigPath << std::endl;
        std::string line;
        std::ifstream reader(mConfigPath);
        if (!reader) {
            // @TODO: log error
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
                continue;
            }

            key = results[0];
            value = results[1];

            if (!processToken(key, value)) {
                //std::cerr << "Unknown key option (" << mConfigPath << " #" << lineNumber << ")" << std::endl;
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