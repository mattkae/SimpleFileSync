#pragma once

#include "base_config.hpp"
#include <cstddef>
#include <string>

namespace shared {
    class State : public BaseConfig {
    public:
        State();
        State(std::string);
        bool processToken(std::string key, std::string value) override;
        void writeTokens(std::ofstream& writer) override;
    
        size_t hash = 0;
        size_t version = 0;
    private:
        std::string mAppDataFile;
    };
}