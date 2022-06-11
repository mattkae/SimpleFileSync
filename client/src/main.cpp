
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iostream>

int main() {
	boost::asio::io_service ios;
	std::string host = "127.0.0.1";
	int port = 13;
	std::string message = "Matthew";
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port);

	boost::asio::ip::tcp::socket socket(ios);

	socket.connect(endpoint);

	boost::array<char, 128> buf;
	std::copy(message.begin(),message.end(),buf.begin());
	boost::system::error_code error;
	socket.write_some(boost::asio::buffer(buf, message.size()), error);

	

	socket.close();
	return 0;
}
