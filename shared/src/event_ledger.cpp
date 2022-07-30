#include "event_ledger.hpp"
#include "deserializer.hpp"
#include <spdlog/spdlog.h>
#include <string>
#include <zlib.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

namespace shared {
    EventLedger::EventLedger(std::string directory) {
        mDirectory = directory;

        if (!std::filesystem::is_directory(mDirectory) || !std::filesystem::exists(mDirectory)) {
            spdlog::info("Creating directory for the ledger: {0}", mDirectory);
            std::filesystem::create_directories(mDirectory);
        }
    }

    std::string EventLedger::mGetEventPath(size_t hash) {
        std::stringstream ss;
        ss << mDirectory << "/" << hash << ".zip";
        return ss.str();
    }

    void EventLedger::record(Event& event) {
        BinarySerializer serializer;
        serializer.writeObject(event);

        unsigned long nDataSize = serializer.getSize();

        spdlog::info("Writing event to ledger: {0}", event.hash);
        spdlog::info("Initial size: {0}", nDataSize);
        
        //auto savedFileName = getSaveAreaPath(mDirectory.c_str());
        // If the directory does not exist, create it
        // Create a new file in the directory with the hash name
        // Write the event to the file and zip the file
        unsigned long nCompressedDataSize = nDataSize;
        unsigned char * pCompressedData = new unsigned char[nCompressedDataSize];
        
        int nResult = compress2(pCompressedData, &nCompressedDataSize, serializer.getData(), nDataSize, 9);
        spdlog::info("Uncompressed size event {0}", nDataSize);
        if (nResult == Z_OK) {
            spdlog::info("Compressed event size: {0}", nCompressedDataSize);
            auto path = mGetEventPath(event.hash);
            std::ofstream wf(path, std::ios::out | std::ios::binary);
            if(!wf) {
                spdlog::error("Unable to write event to path: {0}", path);
            }
            else {
                wf << pCompressedData;
                wf.close();
                spdlog::info("Wrote event to path: {0}", path);
            }
        }

        delete [] pCompressedData;
    }

    Event EventLedger::retrieve(size_t hash) {
        auto path = mGetEventPath(hash);
        std::ifstream reader(path, std::ios::binary);

        if (!reader) {
            spdlog::error("Unable to read event: {0}", hash);
            return Event();
        }

        reader.seekg(0, std::ios::end);
        std::streampos compressedSize = reader.tellg();
        reader.seekg(0, std::ios::beg);

        std::vector<Byte> compressedData(compressedSize);
        reader.read((char*) &compressedData[0], compressedSize);

        size_t uncompressedSize;
        std::vector<Byte> uncompressedData(compressedSize);
        auto nResult = uncompress(&uncompressedData[0], &uncompressedSize, &compressedData[0], compressedSize);
        Event e;
		if (nResult == Z_OK) {
            BinaryDeserializer deserializer({ &uncompressedData[0], uncompressedSize, 0 });
            e = deserializer.readObject<Event>();
		}
        else {
            spdlog::error("Failed to decompress event: {0}", hash);
        }

        return e;
    }
}