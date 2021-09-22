#include "stdafx.h"
#include "types.hpp"
#include "positions_manager.hpp"

void PositionsManager::OnPositionAdd(const IMTPosition* position) {
	jPosition o = jPosition::fromPosition(*position);

	string t(KEY_POS_ADD);
	//t.append(symbol);

	redis->Send(t, o.AsJSON());

	RefreshCache(position->Login());
} 

void PositionsManager::OnPositionUpdate(const IMTPosition* position) {
	jPosition o = jPosition::fromPosition(*position);

	string t(KEY_POS_UPD);
	//t.append(symbol);

	redis->Send(t, o.AsJSON());

	RefreshCache(position->Login());
}

void PositionsManager::OnPositionDelete(const IMTPosition* position) {
	jPosition o = jPosition::fromPosition(*position);

	string t(KEY_POS_RM);
	t.append(std::to_string(o.account));

	redis->Send(t, o.AsJSON());

	RefreshCache(position->Login());
}

void PositionsManager::RefreshCache(int login) {
	IMTPositionArray* positions;
	vector<string> arr;

	positions = man->PositionCreateArray();

	MTAPIRES res = man->PositionGet(login, positions);

	if (res == MT_RET_OK) {
		for (UINT i = 0; i < positions->Total(); i++) {
			IMTPosition *pos = positions->Next(i);

			jPosition o = jPosition::fromPosition(*pos);

			arr.push_back(o.AsJSON());
		}
	}

	ostringstream oss;
	std::copy(arr.begin(), arr.end() - 1, ostream_iterator<string>(oss, ","));
	oss << "{[" << arr.back() << "]}";

	string key(KEY_POSITIONS);
	key.append(to_string(login));

	redis->CacheSet(key, oss.str());

	positions->Release();
}

void PositionsManager::ClearCache(int login) { 
	string key(KEY_POSITIONS);
	key.append(to_string(login));

	redis->CacheClear(key);
}