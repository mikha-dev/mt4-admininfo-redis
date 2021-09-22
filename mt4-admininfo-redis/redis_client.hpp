#pragma once

#include <cpp_redis/cpp_redis>
#include <cpp_redis/misc/macro.hpp>

using namespace std;

class RedisClient
{
public:
	RedisClient(bool show_debug_) {	
		show_debug = show_debug_;
	}
	void Connect(const string &server, size_t port );
	void Disconnect();
	void Send(const string &channel, const string &json);
	void CacheSet(const string key, const string value, bool commit = true);
	void CacheClear(const string key);

private:

	cpp_redis::client client_;
	bool show_debug;
};
