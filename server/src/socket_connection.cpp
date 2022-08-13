#include "socket_connection.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <spdlog/spdlog.h>

namespace server {
    /* Base */
    BaseSocketConnection::~BaseSocketConnection() {}
    bool BaseSocketConnection::isClosed() { return mClosed; }

    /* No ssl */
    NoSslSocketConnection::NoSslSocketConnection(int sockfd) {
        mSockfd = sockfd;
    }

    NoSslSocketConnection::~NoSslSocketConnection() {
        destroy();
    }

    void NoSslSocketConnection::destroy() {
        close(mSockfd);
        mClosed = true;
        spdlog::info("Client closed.");
    }

    void NoSslSocketConnection::writeData(shared::byte* data, size_t size) {
        if (send(mSockfd, data, size, 0) == 0) {
            spdlog::error("Failed to write message to client.");
        }
        else {
            spdlog::info("Message sent to client.");
        }
    }

    SocketBuffer NoSslSocketConnection::readData() {
        SocketBuffer result;
        result.connection = this;
        result.bytesRead = 0;
        result.bytesRead = recv(mSockfd, &result.buffer, SocketBuffer::MAX_BUFF_SIZE - 1, 0);
        if (result.bytesRead == -1) {
            result.connectionClosed = true;
            spdlog::error("Failed to read message from client.");
            return result;
        }
        else if (result.bytesRead == 0) {
            mClosed = true;
            result.buffer[result.bytesRead] = '\0';
            return result;
        }

        spdlog::info("Bytes read: {0}", result.bytesRead);
        result.buffer[result.bytesRead] = '\0';
        return result;
    }

    /* SSL */
    SslSocketConnection::SslSocketConnection(int sockfd, SSL_CTX* ctx) {
        mSockfd = sockfd;
        mCtx = ctx;
    }

    bool SslSocketConnection::connect() {
        mSsl = SSL_new(mCtx);
        SSL_set_fd(mSsl, mSockfd);

        if (SSL_accept(mSsl) <= 0) {
            spdlog::warn("Client not accepted");
            ERR_print_errors_fp(stderr);
            return false;
        }

        return true;
    }

    SslSocketConnection::~SslSocketConnection() {
        destroy();
    }

    void SslSocketConnection::destroy() {
        SSL_shutdown(mSsl);
        SSL_free(mSsl);
        close(mSockfd);
        mClosed = true;
        spdlog::info("Client closed.");
    }

    void SslSocketConnection::writeData(shared::byte* data, size_t size) {
        if (SSL_write(mSsl, data, size) == 0) {
            spdlog::error("Failed to write message to client.");
        }
        else {
            spdlog::info("Message sent to client.");
        }
    }

    SocketBuffer SslSocketConnection::readData() {
        SocketBuffer result;
        result.bytesRead = recv(mSockfd, &result.buffer, SocketBuffer::MAX_BUFF_SIZE - 1, 0);
        if (result.bytesRead == -1) {
            result.connectionClosed = true;
            spdlog::error("Failed to read message from client.");
            return result;
        }
        else if (result.bytesRead == 0) {
            mClosed = true;
            return result;
        }

        result.buffer[result.bytesRead] = '\0';
        return result;
    }
}