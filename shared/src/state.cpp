#include "state.hpp"
#include "base_config.hpp"

namespace shared {
    State::State(): BaseConfig() { }

    State::State(std::string mPath): BaseConfig(mPath) {
        // TODO: This is temporary. We are purposefully starting from a blank State slate everytime
        std::ofstream ofs;
        ofs.open(mPath, std::ofstream::out | std::ofstream::trunc);
        ofs.close();
    }

    bool State::processToken(std::string key, std::string value) {
        if (key == "hash") {
            hash = std::stoi(value);
            std::cout << "Reading hash: " << hash << std::endl;
            return true;
        }
        else if (key == "version") {
            version = std::stoi(value);
            std::cout << "Reading version: " << version << std::endl;
            return true;
        }

        return false;
    }

    void State::writeTokens(std::ofstream& writer) {
        writeToken(writer, "hash", hash);
        writeToken(writer, "version", version);
    }
}