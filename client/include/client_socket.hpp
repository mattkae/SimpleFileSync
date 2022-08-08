#pragma once
#include <cstdint>
#include <string>
#include <ctype.h>
#include "type.hpp"
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

namespace client {
    typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> SslSocket;

    struct ClientSocketOptions {
        std::string host;
        uint_least16_t port;
    };

    struct ClientReadResult {
        static const size_t BUFFER_SIZE = 1024;
        shared::byte data[BUFFER_SIZE];
        size_t len = 0;
    };

    class ClientSocket {
    public:
        ClientSocket(const ClientSocketOptions& options);
        ~ClientSocket();
        void write(shared::byte* data, size_t size);
        ClientReadResult read();
    private:
        void _close();
        SslSocket mSocket;
    };
}

