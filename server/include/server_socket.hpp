#pragma once
#include <ctype.h>
#include <string>
#include <functional>
#include <boost/asio.hpp> 
#include <boost/asio/ssl.hpp>
#include <boost/array.hpp>
#include "type.hpp"


namespace server {
    struct SslOptions {
        std::string certChainFile;
        std::string privateKeyFile;
        std::string tmpDhFile;
    };

    class SocketConnection {
    public:
        SocketConnection(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>* stream);
        static const size_t MAX_BUFF_SIZE = 1024;
        size_t bytesDeserialized = 0;
        size_t bytesRead = 0;
        boost::array<shared::byte, SocketConnection::MAX_BUFF_SIZE> buffer;

        void write(shared::byte* data, size_t size);
        void close();

    private:
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket>* mStream;
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
        boost::asio::io_context mIoContext;
        boost::asio::ip::tcp::acceptor mAcceptor;
        boost::asio::ssl::context mSslContext;
    };
}

