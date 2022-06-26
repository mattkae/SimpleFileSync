#include "app_data.hpp"
#include "base_config.hpp"
#include <string>

namespace client {
    AppData::AppData(): shared::BaseConfig() { }

    AppData::AppData(std::string mPath): shared::BaseConfig(mPath) { }

    bool AppData::processToken(std::string key, std::string value) {
        if (key == "hash") {
            hash = std::stoi(value);
            return true;
        }

        return false;
    }

    void AppData::writeTokens(std::ofstream& writer) {
        writeToken(writer, "hash", hash);
    }
}