#include "state.hpp"
#include "base_config.hpp"
#include <cstddef>
#include <sstream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>

namespace shared {
    State::State(): BaseConfig() { }

    State::State(std::string mPath): BaseConfig(mPath) {
        // @TODO: This is temporary. We are purposefully starting from a blank State slate everytime
        // std::ofstream ofs;
        // ofs.open(mPath, std::ofstream::out | std::ofstream::trunc);
        // ofs.close();
    }

    size_t State::getHash() { return mHashList.size() ? mHashList.back() : 0; }

    const std::vector<size_t> State::getHashList() { return mHashList; }

    void State::addHash(size_t hash, std::vector<char> data) {
        mHashList.push_back(hash);
    }

    const std::string SPACE_DELIMITER = " ";
    bool State::processToken(std::string key, std::string value) {
        if (key == "hashes") {
            std::vector<std::string> words;
            size_t pos = 0;
            while ((pos = value.find(SPACE_DELIMITER)) != std::string::npos) {
                words.push_back(value.substr(0, pos));
                value.erase(0, pos + SPACE_DELIMITER.length());
            }
            
            for (const auto &str : words) {
                mHashList.push_back(std::stoi(str));
            }

            return true;
        }

        return false;
    }

    void State::writeTokens(std::ofstream& writer) {
        std::ostringstream oss;

        if (!mHashList.empty()) {
            std::copy(mHashList.begin(), mHashList.end() - 1, std::ostream_iterator<size_t>(oss, " "));
            oss << mHashList.back();
        }

        writeToken(writer, "hashes", oss.str());
    }
}