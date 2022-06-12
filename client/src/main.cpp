#include "config.hpp"
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <filesystem>
#include "file_watcher.hpp"

void onDirectoryChange();

int main() {
	client::Config globalConfig("../data/config.conf");
	if (!std::filesystem::is_directory(globalConfig.getDirectory()) || !std::filesystem::exists(globalConfig.getDirectory())) {
		std::filesystem::create_directory(globalConfig.getDirectory());
		std::cout << "Creating directory defined in the configuration...";
	}

	client::FileWatcher fw(onDirectoryChange, globalConfig.getDirectory());

	boost::asio::io_service ios;
	std::string host = globalConfig.getIp();
	int port = globalConfig.getPort();
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

void onDirectoryChange() {
	std::cout << "Directory changed." << std::endl;
	return;
}
