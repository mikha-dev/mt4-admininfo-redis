#include "stdafx.h"

#include "config_manager.hpp"
#include "api_manager.hpp"
#include "usr_interrupt_handler.hpp"

using namespace cfx;
using namespace std;

int wmain(int argc, wchar_t *argv[]) {
	InterruptHandler::hookSIGINT();
	ApiManager manager;

	try {

		ConfigManager::Load("config.json");

		string host = ConfigManager::Get("mt4.host");
		int login = ConfigManager::Get("mt4.login", 0);
		string password = ConfigManager::Get("mt4.password");
		string redis_host = ConfigManager::Get("redis.host");
		int redis_port = ConfigManager::Get("redis.port", 6379);
		bool show_debug = ConfigManager::Get("show_debug", 0);

		logging::register_simple_formatter_factory<logging::trivial::severity_level, char>("Severity");
		ifstream file("logger.ini");
		logging::init_from_stream(file);
		logging::add_common_attributes();

		//cout << "MetaTrader5 ticks trying to start for: " << host << endl;
		loginfo << "MetaTrader4 admininfo trying to start at: " << host << endl;

		manager.Start(host, login, password, redis_host, redis_port, show_debug);

		// << "MetaTrader5 ticks started, manager " << login << ":" << password << endl;
		loginfo << "MetaTrader4 admininfo is started, manager: " << login << ";" << endl;

		InterruptHandler::waitForUserInterrupt();

		manager.Stop();
		loginfo << "MetaTrader4 admininfo shutdown" << endl;
	}
	catch (exception & e) {
		manager.Stop();
		logerror << e.what() << endl;
	}
	catch (...) {

	}

	return 0;
}
