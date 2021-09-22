#pragma once

#include "types.hpp"
#include "redis_client.hpp"

using namespace std;

class ApiManager {
public:
	ApiManager() {}
	~ApiManager() {
		
	}

	bool Start(const string server, const int login, const string password, const string redis_host, int redis_port, bool show_debug);
	void Stop();
	static void __stdcall PumpingFunc(int code, int type, void * data, void *param);
	static void FillCacheMargins();
	static void FillCacheSymbols();
	static void FillCacheGroups();

private:
	void connectManagers();
	void fillCache();
	

private:
	static CManagerInterface *man;
	static CManagerInterface *pump;
	CManagerFactory  *factory;

	string mt4_server;
	string mt4_password;
	int mt4_login;

	static RedisClient *redis_client;
};

