#include "event.hpp"
#include <string>
#include <string_view>
#include <iostream>
#include <fstream>

namespace shared {
    bool executeEvent(const Event& event, const std::string&& mDirectory) {
        std::string path = mDirectory + "/" + event.path;
        std::cout << "Processing event on path: " << path << std::endl;
        switch (event.type) {
            case shared::EventType::Created:
            case shared::EventType::Modified: {
                std::ofstream fs(path); 
                if(!fs) {
                    std::cerr << "Failed to open file: " << path << std::endl;
                    return false;
                }

                fs << event.content;
                fs.close();
                return true;
            }
            case shared::EventType::Deleted: {
                const int deleteResult = remove(path.c_str());
                if (deleteResult >= 0) {
                    std::cerr << "Failed to delete file: " << path << std::endl;
                    return false;
                }

                return true;
            }
            default:
                std::cerr << "Unsupported event type: " << (int)event.type << std::endl;
                return false;
        }
    }
}