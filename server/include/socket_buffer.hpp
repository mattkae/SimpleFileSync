#pragma once

namespace server {
    class BaseSocketConnection;

    struct SocketBuffer {
        static const int MAX_BUFF_SIZE = 1024;
        bool connectionClosed = false;
        int bytesRead = 0;
        unsigned char buffer[SocketBuffer::MAX_BUFF_SIZE];
        BaseSocketConnection* connection;
    };
}