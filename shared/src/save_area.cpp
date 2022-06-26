#include "save_area.hpp"
#include <cstdlib>

namespace shared {
    std::string getSaveAreaPath(const char* filename) {
        std:: string saveArea = getenv("HOME");
	    saveArea += "/.simplefilesync/" + std::string(filename);
        return saveArea;
    }
}