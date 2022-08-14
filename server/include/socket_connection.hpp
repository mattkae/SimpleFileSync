#pragma once
#include "type.hpp"
#include <stddef.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "socket_buffer.hpp"

namespace server {
    class BaseSocketConnection {
    public:
        virtual ~BaseSocketConnection();
        virtual SocketBuffer readData() = 0;
        virtual void writeData(shared::byte* data, size_t size) = 0;
        bool isClosed();
        void stop();

    protected:
        bool mClosed = false;
    };

    class NoSslSocketConnection: public BaseSocketConnection {
    public:
        NoSslSocketConnection(int sockfd);
        ~NoSslSocketConnection() override;

        SocketBuffer readData() override;
        void writeData(shared::byte* data, size_t size) override;
    private:
        int mSockfd;
    };

    class SslSocketConnection: public BaseSocketConnection {
    public:
        SslSocketConnection(int sockfd, SSL_CTX* ctx);
        ~SslSocketConnection() override;
        SocketBuffer readData() override;
        bool connect();
        void writeData(shared::byte* data, size_t size) override;
    private:
        int mSockfd;
        SSL* mSsl;
        SSL_CTX* mCtx;
    };
}
