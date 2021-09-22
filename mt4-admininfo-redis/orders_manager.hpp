#pragma once
#include "types.hpp"
#include "redis_client.hpp"

using namespace std;

class OrdersManager : public IMTOrderSink
{
public:
	OrdersManager(RedisClient *redis_) {
		redis = redis_;
	}

	void      OnOrderAdd(const IMTOrder* /*order*/);
	void      OnOrderUpdate(const IMTOrder* /*order*/);
	void      OnOrderDelete(const IMTOrder* /*order*/);

private:
	RedisClient *redis;
};