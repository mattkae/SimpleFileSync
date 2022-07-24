#pragma once
#include <string>
#include "event.hpp"

namespace shared {
    class EventLedger {
    public:
        EventLedger(std::string directory);

        void record(const Event& event);
        Event retrieve(size_t hash);

    private:
        std::string mDirectory;
};
}