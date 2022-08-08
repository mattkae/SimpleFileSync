#include "client_socket.hpp"
#include <boost/array.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/basic_endpoint.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/bind.hpp>
#include <iostream>

using boost::asio::ip::tcp;

namespace client {
    bool verify_certificate(bool preverified, boost::asio::ssl::verify_context& ctx) {
        // The verify callback can be used to check whether the certificate that is
        // being presented is valid for the peer. For example, RFC 2818 describes
        // the steps involved in doing this for HTTPS. Consult the OpenSSL
        // documentation for more details. Note that the callback is called once
        // for each certificate in the certificate chain, starting from the root
        // certificate authority.

        // In this example we will simply print the certificate's subject name.
        char subject_name[256];
        X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
        X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
        std::cout << "Verifying " << subject_name << "\n";

        return preverified;
    }

    SslSocket _getSocket(const ClientSocketOptions& options) {
        boost::asio::io_service ios;
        boost::asio::ssl::context sslContext(boost::asio::ssl::context::sslv23);
        sslContext.load_verify_file("/home/matthewk/Projects/SimpleFileSync/server/build/server.crt");
        sslContext.set_verify_mode(boost::asio::ssl::verify_peer);
        SslSocket socket = SslSocket(ios, sslContext);
        tcp::resolver resolver(ios);
        boost::asio::connect(socket.lowest_layer(), resolver.resolve({boost::asio::ip::address::from_string(options.host), boost::asio::ip::port_type(options.port)}));
        socket.set_verify_callback(boost::bind(&verify_certificate, _1, _2));
        socket.handshake(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>::client);
        return socket;
    }

    ClientSocket::ClientSocket(const ClientSocketOptions& options): mSocket(_getSocket(options)) {
    }

    ClientSocket::~ClientSocket() {
        this->_close();
    }

    void ClientSocket::write(shared::byte* data, size_t size) {
        // @TODO: Check error
        boost::system::error_code error;
        mSocket.write_some(
			boost::asio::buffer(
				data, 
				size
			),  error);
    }

    ClientReadResult ClientSocket::read() {
        // @TODO: Check error
        boost::system::error_code error;
        ClientReadResult result;
        result.len = mSocket.read_some(boost::asio::buffer(result.data, result.BUFFER_SIZE), error);
        return result;
    }

    void ClientSocket::_close() {
        mSocket.lowest_layer().close();
    }
}