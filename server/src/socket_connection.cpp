#include "socket_connection.hpp"
#include <openssl/ssl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <spdlog/spdlog.h>

namespace server {
    /* Base */
    BaseSocketConnection::~BaseSocketConnection() {}
    bool BaseSocketConnection::isClosed() { return mClosed; }
    void BaseSocketConnection::stop() { mClosed = true; }

    /* No ssl */
    NoSslSocketConnection::NoSslSocketConnection(int sockfd) {
        mSockfd = sockfd;
    }

    NoSslSocketConnection::~NoSslSocketConnection() {
        close(mSockfd);
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
            result.buffer[result.bytesRead] = '\0';
            stop();
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
            stop();
            spdlog::warn("Client not accepted");
            ERR_print_errors_fp(stderr);
            return false;
        }

        spdlog::info("New client accepted.");
        return true;
    }

    SslSocketConnection::~SslSocketConnection() {
        SSL_shutdown(mSsl);
        SSL_free(mSsl);
        close(mSockfd);
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
        result.connection = this;
        result.bytesRead = SSL_read(mSsl, &result.buffer, SocketBuffer::MAX_BUFF_SIZE - 1);
        if (result.bytesRead <= 0) {
            auto errorCode = result.bytesRead;
            result.bytesRead = 0;
            result.buffer[0] = '\0';
            result.connectionClosed = true;
            spdlog::error("Failed to read message from client: {0} {1}", errorCode, ERR_error_string(ERR_get_error(), NULL));
            return result;
        }
        else if (result.bytesRead == 0) {
            stop();
            return result;
        }

        result.buffer[result.bytesRead] = '\0';
        return result;
    }
}