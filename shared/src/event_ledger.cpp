#include "event_ledger.hpp"
#include "deserializer.hpp"
#include "type.hpp"
#include <spdlog/spdlog.h>
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>

namespace shared {
    EventLedger::EventLedger(std::string directory, bool cleanSlate) {
        mDirectory = directory;

        if (!std::filesystem::is_directory(mDirectory) || !std::filesystem::exists(mDirectory)) {
            spdlog::info("Creating directory for the ledger: {0}", mDirectory);
            std::filesystem::create_directories(mDirectory);
        }

        if (cleanSlate) {
            for (const auto& entry : std::filesystem::directory_iterator(mDirectory)) 
                std::filesystem::remove_all(entry.path());
        }
    }

    std::string EventLedger::mGetEventPath(size_t hash) {
        std::stringstream ss;
        ss << mDirectory << "/" << hash << ".dat";
        return ss.str();
    }

    void EventLedger::record(Event& event) {
        BinarySerializer serializer;
        serializer.writeObject(event);

        auto path = mGetEventPath(event.hash);
        std::ofstream wf(path, std::ios::out | std::ios::binary);
        if(!wf) {
            spdlog::error("Unable to write event to path: {0}", path);
        }
        else {
            wf.write((char*) serializer.getData(), serializer.getSize());
            wf.close();
        }
    }

    Event EventLedger::retrieve(size_t hash) {
        auto path = mGetEventPath(hash);
        FILE *f = fopen(path.c_str(), "rb+");
        if (!f) {
            spdlog::error("Unable to read event: {0}", hash);
            return Event();
        }

        fseek(f, 0L, SEEK_END);
        long filesize = ftell(f); // get file size
        fseek(f, 0L ,SEEK_SET); //go back to the beginning
        byte* buffer = new byte[filesize]; // allocate the read buf
        fread(buffer, 1, filesize, f);
        fclose(f);

        BinaryDeserializer deserializer({ &buffer[0], static_cast<u64>(filesize), 0 });
        Event e = deserializer.readObject<Event>();
        e.hash = hash;
        delete[] buffer;
        return e;
    }
}