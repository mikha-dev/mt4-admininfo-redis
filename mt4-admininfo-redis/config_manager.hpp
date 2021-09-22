#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace std;

class ConfigManager
{
public:
	static bool Load(const char *file_name);
	static string Get(const string &key);
	static int Get(const string &key, int def );
private:
	static boost::property_tree::ptree _pt;
};