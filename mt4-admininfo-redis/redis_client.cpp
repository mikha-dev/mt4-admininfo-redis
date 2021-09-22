#include "stdafx.h"
#include "redis_client.hpp"


const std::string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	localtime_s(&tstruct, &now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return buf;
}

void RedisClient::Connect(const string &server, size_t port) {

	winsock_initializer winsock_init;

	cpp_redis::active_logger = unique_ptr<cpp_redis::logger>(new cpp_redis::logger);
	client_.connect(server, port,
		[](const string &host, size_t port, cpp_redis::connect_state status) {
		if (status == cpp_redis::connect_state::dropped) {
			loginfo << " redis disconnected from " << host << ":" << port << endl;
			cout << "redis disconnected from " << host << ":" << port << endl;
		}

		if (status == cpp_redis::connect_state::start) {
			loginfo << "redis connected to " << host << ":" << port << endl;
			cout << "redis connected to " << host << ":" << port << endl;
		}
	}, 10000, 1000, 1000);
}

void RedisClient::Disconnect() {
	client_.disconnect();
}

void RedisClient::Send(const string &channel, const string &json) {
	if(show_debug)
		cout << "channel " << channel <<  " >> " << json << endl;

	try {

		if (!client_.is_connected())
			return;
			
		client_.publish(channel, json, [](cpp_redis::reply &r) {
			//		cout << "reply: " << r.as_string() << endl;

			if (r.is_error()) {
				logerror << "redis publish error " << r.error() << endl;
				cout << "redis publish error " << r.error() << endl;
			}

		});

		client_.commit();
	} 
	catch (const cpp_redis::redis_error &e) {
		logerror << "redis publish error: " << e.what() << endl;
		if (show_debug)
			cout << "redis publish error: " << e.what() << endl;
	}
}

void RedisClient::CacheSet(const string key, const string value, bool commit) {
	if (!client_.is_connected())
		return;
	client_.set(key, value);
	if (commit)
		client_.commit();
}

void RedisClient::CacheClear(const string key) {
	if (!client_.is_connected())
		return;

	vector<string> v;
	v.push_back(key);
	client_.del(v);

	client_.commit();
}
