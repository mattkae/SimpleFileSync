#include "client_socket.hpp"
#include "spdlog/common.h"
#include "spdlog/spdlog.h"
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <openssl/ossl_typ.h>
#include <openssl/ssl.h>
#include <string>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

namespace client {
    ClientSocket::ClientSocket(const ClientSocketOptions& options) {
        addrinfo hints;
        addrinfo* serverInfo;

        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;

        int rv;
        if ((rv = getaddrinfo(options.host.c_str(), std::to_string(options.port).c_str(), &hints, &serverInfo)) != 0) {
            spdlog::error("Unable to getaddrinfo to server: {0}:{1} because: {2}", options.host, options.port, gai_strerror(rv));
            return;
        }


        // Try to connect to at least one connection
        addrinfo* p;
        for (p = serverInfo; p != NULL; p = p->ai_next) {
            if ((mSockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
                continue;
            }

            if (connect(mSockfd, p->ai_addr, p->ai_addrlen) == -1) {
                close(mSockfd);
                continue;
            }

            break;
        }

        if (p == NULL) {
            spdlog::error("Unable to connect to server: {0}:{1}", options.host, options.port);
            return;
        }

        freeaddrinfo(serverInfo);

        mUseSsl = options.useSsl;
        if (mUseSsl) {
            spdlog::info("Using SSL client.");
            auto method = TLS_client_method();
            mSslCtx = SSL_CTX_new(method);
            if (mSslCtx == NULL) {
                spdlog::error("Failed to get SSL context.");
                ERR_print_errors_fp(stderr);
                return;
            }

            mSsl = SSL_new(mSslCtx);
            SSL_set_fd(mSsl, mSockfd);
            if ( SSL_connect(mSsl) == -1 ) {
                spdlog::error("Failed to connect to server with SSL.");
                ERR_print_errors_fp(stderr);
                return;
            }
        }
    }

    ClientSocket::~ClientSocket() {
        this->_close();
    }

    void ClientSocket::write(shared::byte* data, size_t size) {
        if (mUseSsl) {
            int readResult;
            if ((readResult = SSL_write(mSsl, data, size)) <= 0) {
                auto error = SSL_get_error(mSsl, readResult);
                spdlog::error("Failed to write message to server: {0}", error);
            }
        }
        else {
            if (send(mSockfd, data, size, 0) == -1) {
                spdlog::error("Failed to send data.");
            }
        }
    }

    ClientReadResult ClientSocket::read() {
        ClientReadResult result;
        if (mUseSsl) {
            result.len = SSL_read(mSsl, &result.data, result.BUFFER_SIZE - 1);
        }
        else {
            result.len = recv(mSockfd, &result.data, result.BUFFER_SIZE - 1, 0);
        }
        spdlog::info("Bytes read from server: {0}", result.len);
        if (result.len == -1) {
            spdlog::error("Failed to read message from client.");
            return result;
        }
        else if (result.len == 0) {
            this->_close();
        }
        return result;
    }

    void ClientSocket::_close() {
        SSL_free(mSsl);
        close(mSockfd);
        SSL_CTX_free(mSslCtx);
    }
}