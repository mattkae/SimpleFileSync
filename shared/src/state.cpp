#include "state.hpp"
#include "base_config.hpp"

namespace shared {
    State::State(): BaseConfig() { }

    State::State(std::string mPath): BaseConfig(mPath) { }

    bool State::processToken(std::string key, std::string value) {
        if (key == "hash") {
            hash = std::stoi(value);
            return true;
        }
        else if (key == "version") {
            version = std::stoi(value);
        }

        return false;
    }

    void State::writeTokens(std::ofstream& writer) {
        writeToken(writer, "hash", hash);
        writeToken(writer, "version", version);
    }
}