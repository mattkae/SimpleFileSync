#include "config.hpp"
#include <boost/asio.hpp>
#include "serializer.hpp"
#include "server_message.hpp"
#include "client_message.hpp"
#include "state.hpp"
#include "event_ledger.hpp"

namespace server {
    class App {
    public:
        App();
        ~App();

        void run();
        bool processChangeEvent(shared::ClientMessage& incoming);
        std::string getPassword();

    private:
        bool mIsRunning = false;
        Config mConfig;
        shared::BinarySerializer mServerSerializer;
        shared::State mState;
        shared::EventLedger mLedger;
    };
}