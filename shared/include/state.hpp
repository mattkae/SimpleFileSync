#pragma once

#include "base_config.hpp"
#include <cstddef>
#include <string>
#include <vector>

namespace shared {
    struct HashData {
        size_t hash = 0;
    };

    class State : public BaseConfig {
    public:
        State();
        State(std::string, bool eraseData = false);
        bool processToken(std::string key, std::string value) override;
        void writeTokens(std::ofstream& writer) override;
        size_t getHash();
        void addHash(size_t hash);
        const std::vector<size_t> getHashList();
    
    private:
        std::vector<size_t> mHashList;
        std::string mAppDataFile;
    };
}