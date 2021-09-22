#pragma once
#include "types.hpp"
#include "redis_client.hpp"

using namespace std;

class DealsManager : public IMTDealSink
{
public:
	DealsManager(RedisClient *redis_) {
		redis = redis_;
	}

	void      OnDealAdd(const IMTDeal* /*position*/);
	void      OnDealUpdate(const IMTDeal* /*position*/);
	void      OnDealDelete(const IMTDeal* /*position*/);

private:
	RedisClient *redis;
};