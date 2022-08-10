#include "server_socket.hpp"
#include <boost/asio.hpp> 
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/ssl.hpp> 
#include <boost/array.hpp>
#include <spdlog/spdlog.h>

namespace server {

    using boost::asio::ip::tcp;
    typedef boost::asio::ssl::stream<tcp::socket> SslSocket;

    ServerSocket::ServerSocket(const ServerSocketOptions& opts) :
        mIoContext(), 
        mAcceptor(mIoContext, tcp::endpoint(tcp::v4(), opts.port)),
        mSslContext(boost::asio::ssl::context::sslv23)
    {

        if (opts.useSsl) {
            // Creating a self-signed certificate: https://stackoverflow.com/questions/10175812/how-to-generate-a-self-signed-ssl-certificate-using-openssl/10176685#10176685
            mSslContext.use_certificate_chain_file("./server.crt");
            mSslContext.use_private_key_file("./server.key", boost::asio::ssl::context::pem);
            mSslContext.use_tmp_dh_file("./dh512.pem");
            //mSslContext.set_password_callback(std::bind(&App::getPassword, this));
        }

        mOnRead = opts.onRead;
    }

    ServerSocket::~ServerSocket() {
        
    }

    void ServerSocket::run() {
        mIsRunning = true;
        while (mIsRunning) {
            try {
                tcp::socket socket(mIoContext);
                mAcceptor.accept(socket);

                SslSocket sslSocket(std::move(socket), mSslContext);
                try {
                    sslSocket.handshake(boost::asio::ssl::stream_base::server);
                }
                catch (const boost::system::system_error& error) {
                    spdlog::error("Could not complete handshake with client: {0}", error.what());
                    continue;
                }

                spdlog::info("Client connected.");

                SocketConnection connection(&sslSocket);
                while (sslSocket.lowest_layer().is_open()) {
                    boost::system::error_code error;
                    sslSocket.lowest_layer().wait(sslSocket.lowest_layer().wait_read);
                    connection.bytesRead = sslSocket.read_some(boost::asio::buffer(connection.buffer), error);
                    connection.bytesDeserialized = 0;

                    while (connection.bytesRead != connection.bytesDeserialized) {
                        connection.bytesDeserialized += mOnRead(connection);
                    }
                }

                spdlog::info("Client closed.");
            } catch (std::exception& e) {
                spdlog::error("Exception while talking to client: {0}", e.what());
            }
        }
    }

    void ServerSocket::stop() {
        mIsRunning = false;
    }

    SocketConnection::SocketConnection(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>* stream) {
        mStream = stream;
    }

    void SocketConnection::close() {
        mStream->lowest_layer().close();
    }

    void SocketConnection::write(shared::byte* data, size_t size) {
        // @TODO: Check error
        boost::system::error_code ignored_error;
        mStream->write_some( boost::asio::buffer(data, size), ignored_error);
    }

}