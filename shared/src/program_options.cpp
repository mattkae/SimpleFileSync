#include "program_options.hpp"
#include "logger.hpp"
#include <iostream>
#include <sstream>

namespace shared {
    ProgramOptions::ProgramOptions(std::string programName): mProgramName(programName) {}
    ProgramOptions::~ProgramOptions() {}

    ProgramOptions& ProgramOptions::addOption(std::string shortName, std::string longName, std::string desc, bool isOptional) {
        for (auto arg : mArguments) {
            if (arg.shortName == shortName) {
                logger_error("Cannot add more than one arg with same short name: %s", shortName.c_str());
                return *this;
            }
            if (arg.longName == longName) {
                logger_error("Cannot add more than one arg with long short name: %s", shortName.c_str());
                return *this;
            }
        }

        mArguments.push_back({ shortName, longName, desc, isOptional });
        return *this;
    }

    bool ProgramOptions::hasOption(std::string name) {
        return mValueMap.contains(name);
    }

    bool ProgramOptions::tryGetOption(std::string name, std::string& out) {
        if (!mValueMap.contains(name)) return false;
        out = mValueMap.at(name);
        return true;
    }

    std::string ProgramOptions::getHelp() {
        std::stringstream ss;

        ss << std::endl << "Usage: " << mProgramName << " [arguments]" << std::endl << std::endl;

        ss << "Arguments:" <<std::endl;
        for (auto arg : mArguments) {
            ss << "  -" << arg.shortName << "  --" << arg.longName << "\t" << arg.desc;
            if (arg.isOptional) {
                ss << " (Optional)";
            }
            ss << std::endl;
        }

        return ss.str();
    }

    inline bool isShortArg(const std::string& foundArg) {
        return foundArg.starts_with("-");
    }

    inline bool isLongArg(const std::string& foundArg) {
        return foundArg.starts_with("--");
    }

    void ProgramOptions::parse(int argc, char** argv) {
        mValueMap.clear();
        for (int r = 1; r < argc; r++) {
            ProgramOptionDefinition* foundDefinition = nullptr;
            const std::string& foundArg = argv[r];
            if (isLongArg(foundArg)) {
                for (auto& arg : mArguments) {
                    if (arg.longName == foundArg.substr(2)) {
                        foundDefinition = &arg;
                        break;
                    }
                }
            }
            else if (isShortArg(foundArg)) {
                for (auto& arg : mArguments) {
                    if (arg.shortName == foundArg.substr(1)) {
                        foundDefinition = &arg;
                        break;
                    }
                }
            }

            if (foundDefinition) {
                if (r + 1 < argc) {
                    const std::string nextArg = argv[r + 1];
                    if (isShortArg(nextArg)  || isLongArg(nextArg)) {
                        mValueMap.insert({foundDefinition->longName, ""});
                    }
                    else {
                        mValueMap.insert({foundDefinition->longName, nextArg});
                        r++;
                    }
                }
                else {
                    mValueMap.insert({foundDefinition->longName, ""});
                }
            }
        }

    }
}