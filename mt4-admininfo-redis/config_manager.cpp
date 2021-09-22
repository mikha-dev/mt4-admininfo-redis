#include "stdafx.h"
#include "config_manager.hpp"

boost::property_tree::ptree ConfigManager::_pt;

std::string ConfigManager::Get(const string &key) {
	return _pt.get<string>(key);
}

int ConfigManager::Get(const std::string &key, int def) {
	return _pt.get<int>(key);
}

bool ConfigManager::Load(const char *file) {
	try {

		std::ifstream ifs(file);

		boost::property_tree::read_json(ifs, _pt);

		ifs.close();
	}
	catch (const std::exception &ex) {
		std::cout << ex.what() << std::endl;
		return false;
	}

	return true;
}

