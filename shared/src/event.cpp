#include "event.hpp"
#include "server_message.hpp"
#include "logger.hpp"
#include "type.hpp"
#include <string>
#include <string_view>
#include <iostream>
#include <fstream>
#include <sstream>

namespace shared {
    void writeFile(BinarySerializer& serializer, Event& event) {
		// @Performance Investigate efficiency as files get larger
	    constexpr auto read_size = std::size_t(4096);
		auto stream = std::ifstream(event.fullpath);
		stream.exceptions(std::ios_base::badbit);
    
		auto out = std::string();
		auto buf = std::string(read_size, '\0');
		while (stream.read(& buf[0], read_size)) {
			out.append(buf, 0, stream.gcount());
		}
		out.append(buf, 0, stream.gcount());

		serializer.writeString(out);
	}

    void Event::serialize(BinarySerializer& serializer) {
        serializer.write(hash);
        serializer.write(getEnumType(type));
        serializer.write(timeModifiedUtcMs);
        serializer.writeString(path);

        if (type == shared::EventType::Created || type == shared::EventType::Modified) {
            writeFile(serializer, *this);
        }
    }

    void Event::deserialize(BinaryDeserializer& serializer){
        hash = serializer.readu64();
        type = (shared::EventType)serializer.readi16();
        timeModifiedUtcMs = serializer.readu64();
        path = serializer.readString();

        if (type == shared::EventType::Created || type == shared::EventType::Modified) {
            content = serializer.readString();
        }
    }
    
    std::string Event::toString() const {
        std::stringstream ss;
        ss << "Event {\n";
        ss << "\t hash=" << hash << "\n";
        ss << "\t type=" << (int)type << "\n";
        ss << "\t timeModifiedUtcMs=" << timeModifiedUtcMs << "\n";
        ss << "\t path=" << path << "\n";
        ss << "\t content=" << content << "\n";
        ss << "}";
        return ss.str();
    }

    bool executeEvent(const Event& event, const std::string&& mDirectory) {
        std::string path = mDirectory + "/" + event.path;
        switch (event.type) {
            case shared::EventType::Created:
            case shared::EventType::Modified: {
                std::ofstream fs(path); 
                if(!fs) {
                    logger_error("Failed to open file at path=%s", path.c_str());
                    return false;
                }

                fs << event.content;
                fs.close();
                return true;
            }
            case shared::EventType::Deleted: {
                const int deleteResult = remove(path.c_str());
                if (deleteResult >= 0) {
                    logger_error("Failed to delete file at path=%s", path.c_str());
                    return false;
                }

                return true;
            }
            default:
                logger_error("Unsupported change type, type=%d", (int)event.type);
                return false;
        }
    }
}