#include "config.hpp"
#include "serializer.hpp"
#include "server_message.hpp"
#include "client_message.hpp"
#include "state.hpp"
#include "event_ledger.hpp"

namespace server {
    struct SocketConnection;

    class ServerApp {
    public:
        ServerApp();
        ~ServerApp();

        void run();
        bool processChangeEvent(shared::ClientMessage& incoming);

    private:
        Config mConfig;
        shared::BinarySerializer mServerSerializer;
        shared::State mState;
        shared::EventLedger mLedger;

        size_t _onData(SocketConnection& data);
    };
}