#include "client_app.hpp"
#include <filesystem>
#include <iostream>
#include <exception>
#include "program_options.hpp"
#include "logger.hpp"

int main(int argc, char** argv) {
	Logger::initialize({});
	client::ClientOptions opts;
	try {
		shared::ProgramOptions po("simplefilesync_c");
		po.addOption("c", "clean", "Erase all saved data and start from a blank slate.")
			.addOption("h", "help", "Print help message");

		po.parse(argc, argv);
		std::string blankSlateString;
		if (po.hasOption("help")) {
			std::cout << po.getHelp();
			return 0;
		}

		if (po.tryGetOption("clean", blankSlateString)) {
			opts.blankSlate = blankSlateString == "true";
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


