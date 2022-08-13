#include "client_socket.hpp"
#include "spdlog/common.h"
#include "spdlog/spdlog.h"
#include <cstring>
#include <iostream>
#include <netdb.h>
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
    }

    ClientSocket::~ClientSocket() {
        this->_close();
    }

    void ClientSocket::write(shared::byte* data, size_t size) {
        if (send(mSockfd, data, size, 0) == -1) {
            spdlog::error("Failed to send data.");
        }
    }

    ClientReadResult ClientSocket::read() {
        ClientReadResult result;
        return result;
    }

    void ClientSocket::_close() {
        close(mSockfd);
    }
}