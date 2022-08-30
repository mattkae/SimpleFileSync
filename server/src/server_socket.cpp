#include "server_socket.hpp"
#include <netdb.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "logger.hpp"
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
            logger_error("Unable to get address info for port %s. Error: %s", port.c_str(), gai_strerror(rv));
            return;
        }

        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol); // @TODO: Select the right thing
        if (sockfd < 0) {
            logger_error("Unable to open server socket.");
            return;
        }

        if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
            logger_error("Unable to bind server socket.");
            return;
        }

        freeaddrinfo(res);
        listen(sockfd, BACKLOG);
        SSL_CTX* ctx = NULL;
        if (mUseSsl) {
            ctx = _getSslContext();
            if (ctx == NULL) {
                logger_error("Unable to start server without valid SSL context.");
                return;
            }

            logger_info("Using SSL.");
        }
        else {
            logger_info("Not using SSL.");
        }
        
        while (mIsRunning) {
            try {
                sockaddr_storage clientAddr;
                socklen_t addrSize = sizeof clientAddr;
                int clientfd = accept(sockfd, (sockaddr*)&clientAddr, &addrSize);
                if (clientfd < 0) {
                    logger_info("Unable to accept client connection.");
                    continue;
                }

                BaseSocketConnection* conn;
                if (mUseSsl) {
                    auto sslConn = new SslSocketConnection(clientfd, ctx);
                    bool couldConnect = sslConn->connect();
                    conn = sslConn;
                    if (!couldConnect) {
                        logger_error("Failed to accept client as SSL.");
                    }
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
                logger_error("Exception while talking to client: %s", e.what());
            }
        }

        close(sockfd);
        if (ctx) SSL_CTX_free(ctx);
    }

    SSL_CTX* ServerSocket::_getSslContext() {
        // This post usefully explained the keys that are in play here:
        // https://superuser.com/questions/620121/what-is-the-difference-between-a-certificate-and-a-key-with-respect-to-ssl
        const SSL_METHOD *method;
        SSL_CTX *ctx;

        method = TLS_server_method();

        ctx = SSL_CTX_new(method);
        if (!ctx) {
            logger_error("Unable to create SSL context");
            ERR_print_errors_fp(stderr);
            return NULL;
        }

        if (SSL_CTX_use_certificate_file(ctx, mSslOptions.certChainFile.c_str(), SSL_FILETYPE_PEM) <= 0) {
            logger_error("Unable to create use certificate file.");
            ERR_print_errors_fp(stderr);
            return NULL;
        }

        if (SSL_CTX_use_PrivateKey_file(ctx, mSslOptions.privateKeyFile.c_str(), SSL_FILETYPE_PEM) <= 0 ) {
            logger_error("Unable to create use key file.");
            ERR_print_errors_fp(stderr);
            return NULL;
        }

        return ctx;
    }

    void ServerSocket::stop() {
        mIsRunning = false;
    }
}