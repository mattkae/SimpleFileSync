#include "event.hpp"
#include <string>
#include <string_view>
#include <iostream>
#include <fstream>
#include <spdlog/spdlog.h>

namespace shared {
    bool executeEvent(const Event& event, const std::string&& mDirectory) {
        std::string path = mDirectory + "/" + event.path;
        switch (event.type) {
            case shared::EventType::Created:
            case shared::EventType::Modified: {
                std::ofstream fs(path); 
                if(!fs) {
                    spdlog::error("Failed to open file at path={0}", path);
                    return false;
                }

                fs << event.content;
                fs.close();
                return true;
            }
            case shared::EventType::Deleted: {
                const int deleteResult = remove(path.c_str());
                if (deleteResult >= 0) {
                    spdlog::error("Failed to delete file at path={0}", path);
                    return false;
                }

                return true;
            }
            default:
                spdlog::error("Unsupported change type, type={0}", (int)event.type);
                return false;
        }
    }
}