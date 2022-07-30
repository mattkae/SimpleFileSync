#pragma once
#include <string>
#include "event.hpp"

namespace shared {
    class EventLedger {
    public:
        EventLedger(std::string directory, bool cleanSlate = false);

        void record(Event& event);
        Event retrieve(size_t hash);

    private:
        std::string mDirectory;
        std::string mGetEventPath(size_t hash);
};
}