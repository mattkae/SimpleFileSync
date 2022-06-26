#pragma once

#include "base_config.hpp"
#include <cstddef>
#include <string>

namespace client {
    class AppData : public shared::BaseConfig {
    public:
        AppData();
        AppData(std::string);
        bool processToken(std::string key, std::string value) override;
        void writeTokens(std::ofstream& writer) override;
    
        size_t hash = 0;
    private:
        std::string mAppDataFile;
    };
}