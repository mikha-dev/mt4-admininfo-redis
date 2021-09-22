#pragma once
#include "types.hpp"
#include "redis_client.hpp"

using namespace std;

class GroupsManager : public IMTConGroupSink
{
public:
	GroupsManager(RedisClient *redis_) {
		redis = redis_;
	}

	void      OnGroupAdd(const IMTConGroup* /*group*/);
	void      OnGroupUpdate(const IMTConGroup* /*group*/);
	void      OnGroupDelete(const IMTConGroup* /*group*/);

private:
	RedisClient *redis;
};