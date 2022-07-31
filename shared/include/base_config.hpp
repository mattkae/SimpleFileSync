#pragma once
#include <string>
#include <iostream>
#include <fstream>

namespace shared {
    class BaseConfig {
    public:
        BaseConfig();
        BaseConfig(std::string configPath);

        virtual bool processToken(std::string key, std::string value) = 0;
        virtual void writeTokens(std::ofstream& writer) = 0;

        bool load();
        void write();

    protected:
        template<typename T>
        void writeToken(std::ofstream& writer, std::string key, T value) {
            writer << key << " " << value << std::endl;
        }

    private:
        std::string mConfigPath;
    };
}