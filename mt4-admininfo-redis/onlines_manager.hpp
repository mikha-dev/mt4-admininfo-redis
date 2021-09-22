#pragma once
#include "types.hpp"
#include "redis_client.hpp"
#include "positions_manager.hpp"

using namespace std;

class OnlinesManager : public IMTUserSink
{
public:
	OnlinesManager(RedisClient *redis_, IMTManagerAPI *man_, PositionsManager *pos_man_) {
		redis = redis_;
		man = man_;
		pos_man = pos_man_;
	}

	void      OnUserLogin(LPCWSTR /*ip*/, const IMTUser* /*user*/, const UINT /*type*/);
	void      OnUserLogout(LPCWSTR /*ip*/, const IMTUser* /*user*/, const UINT /*type*/);
	void refreshCache();

private:
	RedisClient *redis;
	IMTManagerAPI *man;
	PositionsManager *pos_man;
};