#include "event_ledger.hpp"
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

    void EventLedger::record(const Event& event) {
        BinarySerializer<Event> serializer;
        serializer.serialize(event);

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
            std::stringstream ss;
            ss << mDirectory << "/" << event.hash << ".zip";
            std::ofstream wf(ss.str(), std::ios::out | std::ios::binary);
            if(!wf) {
                spdlog::error("Unable to write event to path: {0}", ss.str());
            }
            else {
                wf << pCompressedData;
                wf.close();
                spdlog::info("Wrote event to path: {0}", ss.str());
            }
        }

        delete [] pCompressedData;
    }

    Event EventLedger::retrieve(size_t hash) {
        return Event();
    }
}