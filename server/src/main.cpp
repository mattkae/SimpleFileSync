#include "logger.hpp"
#include "server_app.hpp"

int main() {
	Logger::initialize({});
	server::ServerApp app;
	app.run();
	return 0;
}
