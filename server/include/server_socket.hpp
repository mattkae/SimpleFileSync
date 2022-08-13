#pragma once
#include <ctype.h>
#include <string>
#include <functional>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "type.hpp"


namespace server {
    struct SocketBuffer;
    struct SslOptions {
        std::string certChainFile;
        std::string privateKeyFile;
        std::string tmpDhFile;
    };

    struct ServerSocketOptions {
        int port = 5555;
        bool useSsl = false;
        std::function<void(SocketBuffer&)> onRead;

        SslOptions sslOptions;
    };

    class ServerSocket {
    public:
        ServerSocket(const ServerSocketOptions& opts);
        ~ServerSocket();
        void stop();
        void run();

    private:
        SSL_CTX* _getSslContext();

        std::function<void((SocketBuffer&))> mOnRead;
        bool mIsRunning = false;
        int mPort = 0;
        bool mUseSsl = false;
        SslOptions mSslOptions;
    };
}

