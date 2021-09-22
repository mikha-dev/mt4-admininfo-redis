#include "stdafx.h"
#include "types.hpp"
#include "onlines_manager.hpp"

void      OnlinesManager::OnUserLogin(LPCWSTR ip, const IMTUser* user, const UINT type) {

	refreshCache();

	jOnline o = jOnline::fromMTUser(ip, *user, type);

	string t(KEY_USER_ONLINE);
	t.append(std::to_string(o.login));

	redis->Send(t, o.AsJSON());

	pos_man->RefreshCache(user->Login());
}

void OnlinesManager::OnUserLogout(LPCWSTR ip, const IMTUser* user, const UINT type) {

	refreshCache();

	jOnline o = jOnline::fromMTUser(ip, *user, type);

	string t(KEY_USER_OFFLINE);
	t.append(std::to_string(o.login));

	redis->Send(t, o.AsJSON());

	pos_man->ClearCache(user->Login());
}

void OnlinesManager::refreshCache() {
	vector<string> arr;

	for( UINT i = 0; i < man->OnlineTotal(); i++ ) {
		IMTOnline *mt_o =  man->OnlineCreate();
		jOnline o;

		man->OnlineNext(i, mt_o);
		o = jOnline::fromMTOnline(*mt_o);
		arr.push_back( o.AsJSON());

		mt_o->Release();
	}

	ostringstream oss;
	std::copy(arr.begin(), arr.end() - 1, ostream_iterator<string>(oss, ","));
	oss << "{[" << arr.back() << "]}";

	redis->CacheSet(KEY_USERS_ONLINE, oss.str());
}