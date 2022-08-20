#include "environment.hpp"
#include <cstdlib>
#include <sstream>

namespace shared {
    inline std::string getConfigDirectory() {
        std::stringstream ss;
        auto xdgConfigHome = getenv("XDG_CONFIG_HOME");
        if (xdgConfigHome) {
            ss << xdgConfigHome << "/";
        }
        else {
            auto homeDirectory = getenv("HOME");
            if (homeDirectory) {
                ss << homeDirectory << "/.config/";
            }
            else {
                ss << "~/.config/";
            }
        }
        ss << "simplefilesync/";
        return ss.str();
    }

    inline std::string getDataDirectory() {
        std::stringstream ss;
        auto xdgDataHome = getenv("XDG_DATA_HOME");
        if (xdgDataHome) {
            ss << xdgDataHome << "/";
        }
        else {
            auto homeDirectory = getenv("HOME");
            if (homeDirectory) {
                ss << homeDirectory << "/.local/share/";
            }
            else {
                ss << "~/.local/share/";
            }
        }
        ss << "simplefilesync/";
        return ss.str();
    }

    Environment::Environment() {
        mConfigDirectory = getConfigDirectory();
        mDataDirectory = getDataDirectory();
    }

    const std::string& Environment::getConfigDirectory() {
        return mConfigDirectory;
    }

    const std::string& Environment::getDataDirectory() {
        return mDataDirectory;
    }

    Environment* gEnv = nullptr;
    Environment& Environment::get() {
        if (!gEnv) gEnv = new Environment();
        return *gEnv;
    }
}