#pragma once
#include <string>

namespace shared {
    class Environment {
    public:
        static Environment& get();
        const std::string& getConfigDirectory();
        const std::string& getDataDirectory();
    private:
        Environment();
        std::string mConfigDirectory;
        std::string mDataDirectory;
    };
}