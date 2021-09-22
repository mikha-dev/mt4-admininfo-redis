#include "stdafx.h"
#include "types.hpp"
#include "groups_manager.hpp"

void GroupsManager::OnGroupAdd(const IMTConGroup* group) {
	jGroup o = jGroup::fromGroup(*group);

	string t(KEY_GRP_ADD);
	//t.append(symbol);

	redis->Send(t, o.AsJSON());
} 

void GroupsManager::OnGroupUpdate(const IMTConGroup* group) {
	jGroup o = jGroup::fromGroup(*group);

	string t(KEY_GRP_UPD);
	//t.append(symbol);

	redis->Send(t, o.AsJSON());
}

void GroupsManager::OnGroupDelete(const IMTConGroup* group) {
	jGroup o = jGroup::fromGroup(*group);

	string t(KEY_GRP_RM);
	t.append(o.name);

	redis->Send(t, o.AsJSON());
}