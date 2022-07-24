#include "event.hpp"
#include "save_area.hpp"
#include "server_message.hpp"
#include "util.hpp"
#include <string>
#include <string_view>
#include <iostream>
#include <fstream>
#include <spdlog/spdlog.h>

namespace shared {

    // @TODO: Duplicated from client_message.hpp
    void writeFile(BinarySerializer<Event> * serializer, Event& event) {
		// @TODO Investigate efficiency as files get larger
	    constexpr auto read_size = std::size_t(4096);
		auto stream = std::ifstream(event.fullpath);
		stream.exceptions(std::ios_base::badbit);
    
		auto out = std::string();
		auto buf = std::string(read_size, '\0');
		while (stream.read(& buf[0], read_size)) {
			out.append(buf, 0, stream.gcount());
		}
		out.append(buf, 0, stream.gcount());

		serializer->writeString(out);
	}

    void Event::serialize(BinarySerializer<Event>* serializer) {
        serializer->write<size_t>(hash);
        serializer->write<int>(enumToUnderlying(type));
        serializer->write(timeModifiedUtcMs);
        serializer->writeString(path);

        if (type == shared::EventType::Created || type == shared::EventType::Modified) {
            writeFile(serializer, *this);
        }
    }

    void Event::deserialize(BinaryDeserializer<Event>* serializer){
        // @TODO: Write the Event deserializer here
    }

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