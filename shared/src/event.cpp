#include "event.hpp"
#include "save_area.hpp"
#include "server_message.hpp"
#include "util.hpp"
#include <string>
#include <string_view>
#include <iostream>
#include <fstream>
#include <spdlog/spdlog.h>
#include <zlib.h>

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

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

    bool recordEvent(const Event& event, size_t hash) {
        BinarySerializer<Event> serializer;
        serializer.serialize(event);

        unsigned long nDataSize = serializer.getSize();

        printf("Initial size: %d\n", nDataSize);
        
        //auto savedFileName = getSaveAreaPath(mDirectory.c_str());
        // If the directory does not exist, create it
        // Create a new file in the directory with the hash name
        // Write the event to the file and zip the file
        unsigned long nCompressedDataSize = nDataSize;
        unsigned char * pCompressedData = new unsigned char[nCompressedDataSize];
        
        int nResult = compress2(pCompressedData, &nCompressedDataSize, serializer.getData(), nDataSize, 9);

        if (nResult == Z_OK)
        {
            printf("Compressed size: %d\n", nCompressedDataSize);

            unsigned char * pUncompressedData = new unsigned char[nDataSize];
            nResult = uncompress(pUncompressedData, &nDataSize, pCompressedData, nCompressedDataSize);
            if (nResult == Z_OK)
            {
                printf("Uncompressed size: %d\n", nDataSize);
                if (memcmp(pUncompressedData, serializer.getData(), nDataSize) == 0)
                    printf("Great Success\n");
            }
            delete [] pUncompressedData;
        }

        delete [] pCompressedData;
    }

    Event retrieveEvent(size_t hash) {
        
    }
}