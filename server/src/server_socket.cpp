#include "server_socket.hpp"
#include <netdb.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <spdlog/spdlog.h>

namespace server {
    ServerSocket::ServerSocket(const ServerSocketOptions& opts) {
        mOnRead = opts.onRead;
        mPort = opts.port;
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
        while (mIsRunning) {
            try {
                sockaddr_storage clientAddr;
                socklen_t addrSize = sizeof clientAddr;
                int clientfd = accept(sockfd, (sockaddr*)&clientAddr, &addrSize);
                if (clientfd < 0) {
                    spdlog::error("Unable to accept clinet connection.");
                    continue;
                }

                SocketConnection conn(clientfd);

                while (true) {
                    int bytesRead = recv(clientfd, &conn.buffer, SocketConnection::MAX_BUFF_SIZE - 1, 0);
                    if (bytesRead == -1) {
                        spdlog::error("Failed to read message from client.");
                        continue;
                    }
                    else if (bytesRead == 0) {
                        break; // Client has closed the connection
                    }

                    conn.buffer[bytesRead] = '\0';
                    conn.bytesRead += bytesRead;
                    conn.bytesDeserialized += mOnRead(conn);
                }
            } catch (std::exception& e) {
                spdlog::error("Exception while talking to client: {0}", e.what());
            }
        }
    }

    void ServerSocket::stop() {
        mIsRunning = false;
    }

    SocketConnection::SocketConnection(int sockfd) {
        mSockfd = sockfd;
    }

    SocketConnection::~SocketConnection() {
        doClose();
    }

    void SocketConnection::doClose() {
        close(mSockfd);
        spdlog::info("Client closed.");
    }

    void SocketConnection::write(shared::byte* data, size_t size) {
        if (send(mSockfd, data, size, 0) == 0) {
            spdlog::error("Failed to write message to client.");
        }
        else {
            spdlog::info("Message sent to client.");
        }
    }

}