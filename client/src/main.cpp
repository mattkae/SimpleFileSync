#include "client_app.hpp"
#include <filesystem>
#include "save_area.hpp"
#include <boost/program_options.hpp>
#include <iostream>
#include <exception>

namespace po = boost::program_options;

int main(int argc, char** argv) {
	client::ClientOptions opts;
	try {
		po::options_description desc("Allowed options");
		desc.add_options()
			("help", "produce help message")
			("clean_slate", po::value<bool>(), "Start from a blank slate, erasing all saved data on the client side.");

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		if (vm.count("help")) {
			std::cout << desc << "\n";
			return 0;
		}

		if (vm.count("clean_slate")) {
			opts.blankSlate = vm["clean_slate"].as<bool>();
		}
	}
	catch(std::exception& e) {
        std::cerr << "Exception while parsing startup variables: " << e.what() << "\n";
        return 1;
    }
    catch(...) {
        std::cerr << "Exception of unknown type at startup.\n";
		return 1;
	}

	client::ClientApp app(opts);
	return 0;
}


