#pragma once
#include "types.hpp"
#include "redis_client.hpp"

using namespace std;

class PositionsManager : public IMTPositionSink
{
public:
	PositionsManager(RedisClient *redis_, IMTManagerAPI *man_) {
		redis = redis_;
		man = man_;
	}

	void      OnPositionAdd(const IMTPosition* /*position*/);
	void      OnPositionUpdate(const IMTPosition* /*position*/);
	void      OnPositionDelete(const IMTPosition* /*position*/);
	void      RefreshCache(int login);
	void      ClearCache(int login);

private:
	RedisClient *redis;
	IMTManagerAPI *man;
};