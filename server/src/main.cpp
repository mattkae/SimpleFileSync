#include "config.hpp"
#include <filesystem>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>

int main() {
	server::Config globalConfig("../data/config.conf");
	if (!std::filesystem::is_directory(globalConfig.getDirectory()) || !std::filesystem::exists(globalConfig.getDirectory())) {
		std::filesystem::create_directory(globalConfig.getDirectory());
		std::cout << "Creating directory defined in the configuration...";
	}

	try {
		boost::asio::io_context io_context;
		boost::asio::ip::tcp::acceptor acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 13));
		for (;;) {
			boost::asio::ip::tcp::socket socket(io_context);
			acceptor.accept(socket);

			std::cout << "Client connected." << std::endl;

		    boost::array<char, 128> buf;
			boost::system::error_code error;

			size_t len = socket.read_some(boost::asio::buffer(buf), error);
			std::cout << buf.data() << std::endl;
	  
			std::string message = "Hello, " + std::string(buf.data());
			boost::system::error_code ignored_error;
			boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
			std::cout << "Closing client." << std::endl;
			
			

		}
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
    
	return 0;
}
