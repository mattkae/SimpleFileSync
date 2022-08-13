#pragma once
#include <ctype.h>
#include <string>
#include <functional>
#include "type.hpp"


namespace server {
    struct SslOptions {
        std::string certChainFile;
        std::string privateKeyFile;
        std::string tmpDhFile;
    };

    class SocketConnection {
    public:
        SocketConnection(int sockfd);
        ~SocketConnection();
        static const size_t MAX_BUFF_SIZE = 1024;
        size_t bytesDeserialized = 0;
        size_t bytesRead = 0;
        unsigned char buffer[SocketConnection::MAX_BUFF_SIZE];

        void write(shared::byte* data, size_t size);
        void doClose();
    private:
        int mSockfd;
    };

    struct ServerSocketOptions {
        int port = 5555;
        bool useSsl = false;
        std::function<size_t(SocketConnection&)> onRead;

        SslOptions sslOptions;
    };

    class ServerSocket {
    public:
        ServerSocket(const ServerSocketOptions& opts);
        ~ServerSocket();
        void run();
        void stop();

    private:
        std::function<size_t((SocketConnection&))> mOnRead;
        bool mIsRunning = false;
        int mPort = 0;
    };
}

