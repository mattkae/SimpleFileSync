#pragma once
#include <string>
#include <map>
#include <vector>


namespace shared {
    struct ProgramOptionDefinition {
        std::string shortName;
        std::string longName;
        std::string desc;
        bool isOptional;
    };

    class ProgramOptions {
    public:
        ProgramOptions(std::string programName);
        ~ProgramOptions();

        ProgramOptions& addOption(std::string shortName, std::string longName, std::string desc, bool isOptional = true);
        bool hasOption(std::string name);
        bool tryGetOption(std::string name, std::string& out);
        std::string getHelp();
        void parse(int argc, char** argv);

    private:
        std::string mProgramName;
        std::vector<ProgramOptionDefinition> mArguments;
        std::map<std::string, std::string> mValueMap;
    };
}
