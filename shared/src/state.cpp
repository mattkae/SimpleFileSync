#include "state.hpp"
#include "base_config.hpp"
#include <cstddef>
#include <sstream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

namespace shared {
    State::State(): BaseConfig() { }

    State::State(std::string mPath, bool eraseData): BaseConfig(mPath) {
        if (eraseData) {
            std::ofstream ofs;
            ofs.open(mPath, std::ofstream::out | std::ofstream::trunc);
            ofs.close();
        }
    }

    size_t State::getHash() { return mHashList.size() ? mHashList.back() : 0; }

    const std::vector<size_t> State::getHashList() { return mHashList; }

    void State::addHash(size_t hash) {
        mHashList.push_back(hash);
    }

    const std::string SPACE_DELIMITER = " ";
    const std::string NEWLINE_DELIMITER = "\n";
    bool State::processToken(std::string key, std::string value) {
        mHashList.clear();
        if (key == "hashes") {
            std::vector<std::string> words;
            std::istringstream iss(value);
            std::copy(std::istream_iterator<std::string>(iss),
                std::istream_iterator<std::string>(),
                std::back_inserter(words));
                
            
            for (const std::string &str : words) {
                mHashList.push_back(std::stoul(str));
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