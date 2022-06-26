#include "app.hpp"
#include "app_data.hpp"
#include <filesystem>
#include "save_area.hpp"

int main() {
	client::AppData appData(shared::getSaveAreaPath("client_saved.data"));
	appData.load();
	client::App app;
	return 0;
}


