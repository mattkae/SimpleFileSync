#include "server_app.hpp"
#include "spdlog/common.h"
#include <spdlog/spdlog.h>
int main() {
	spdlog::set_level(spdlog::level::trace);
	server::ServerApp app;
	app.run();
	return 0;
}
