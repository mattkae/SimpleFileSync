#pragma once
#include <cstdint>
#include <string>
#include <ctype.h>
#include "type.hpp"
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace client {
    struct ClientSocketOptions {
        std::string host;
        uint_least16_t port = 0;
        bool useSsl = false;
    };

    struct ClientReadResult {
        static const size_t BUFFER_SIZE = 1024;
        shared::byte data[BUFFER_SIZE];
        int len = 0;
    };

    class ClientSocket {
    public:
        ClientSocket(const ClientSocketOptions& options);
        ~ClientSocket();
        void write(shared::byte* data, size_t size);
        ClientReadResult read();
    private:
        int mSockfd;
        void _close();

        bool mUseSsl = false;
        SSL_CTX* mSslCtx = NULL;
        SSL* mSsl = NULL;
    };
}

