#include "server_socket.hpp"
#include <netdb.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <spdlog/spdlog.h>
#include "socket_connection.hpp"

namespace server {
    ServerSocket::ServerSocket(const ServerSocketOptions& opts) {
        mOnRead = opts.onRead;
        mPort = opts.port;
        mUseSsl = opts.useSsl;
        if (mUseSsl) {
            mSslOptions = opts.sslOptions;
        }
    }

    ServerSocket::~ServerSocket() {
        
    }

    #define BACKLOG 20
    #define MAXDATASIZE 1024

    void ServerSocket::run() {
        mIsRunning = true;
        addrinfo hints = {};
        memset(&hints, 0, sizeof hints);
        addrinfo* res = nullptr;
        int sockfd = 
        hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
        hints.ai_socktype = SOCK_STREAM; // TCP
        hints.ai_flags = AI_PASSIVE;

        std::string port = std::to_string(mPort);
        int rv;
        if ((rv = getaddrinfo(NULL, port.c_str(), &hints, &res)) != 0) {
            spdlog::error("Unable to get address info for port {0}. Error: {1}", port, gai_strerror(rv));
            return;
        }

        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol); // @TODO: Select the right thing
        if (sockfd < 0) {
            spdlog::error("Unable to open server socket.");
            return;
        }

        if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
            spdlog::error("Unable to bind server socket.");
            return;
        }

        freeaddrinfo(res);
        listen(sockfd, BACKLOG);
        SSL_CTX* ctx = NULL;
        if (mUseSsl) {
            ctx = _getSslContext();
            if (ctx == NULL) {
                spdlog::error("Unable to start server without valid SSL context.");
                return;
            }
        }
        
        while (mIsRunning) {
            try {
                sockaddr_storage clientAddr;
                socklen_t addrSize = sizeof clientAddr;
                int clientfd = accept(sockfd, (sockaddr*)&clientAddr, &addrSize);
                if (clientfd < 0) {
                    spdlog::error("Unable to accept client connection.");
                    continue;
                }

                BaseSocketConnection* conn;
                if (mUseSsl) {
                    conn = new SslSocketConnection(clientfd, ctx);
                }
                else {
                    conn = new NoSslSocketConnection(clientfd);
                }

                while (!conn->isClosed()) {
                    SocketBuffer readResult = conn->readData();
                    mOnRead(readResult);
                }

                delete conn;
            } catch (std::exception& e) {
                spdlog::error("Exception while talking to client: {0}", e.what());
            }
        }

        close(sockfd);
        if (ctx) SSL_CTX_free(ctx);
    }

    SSL_CTX* ServerSocket::_getSslContext() {
         const SSL_METHOD *method;
        SSL_CTX *ctx;

        method = TLS_server_method();

        ctx = SSL_CTX_new(method);
        if (!ctx) {
            spdlog::error("Unable to create SSL context");
            ERR_print_errors_fp(stderr);
            return NULL;
        }

        if (SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM) <= 0) {
            spdlog::error("Unable to create use certificate file.");
            ERR_print_errors_fp(stderr);
            return NULL;
        }

        if (SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0 ) {
            spdlog::error("Unable to create use key file.");
            ERR_print_errors_fp(stderr);
            return NULL;
        }

        return ctx;
    }

    void ServerSocket::stop() {
        mIsRunning = false;
    }
}