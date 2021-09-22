#include "stdafx.h"
#include "types.hpp"
#include "orders_manager.hpp"

void OrdersManager::OnOrderAdd(const IMTOrder* order) {
	jOrder o = jOrder::fromMTOrder(*order);

	string t(KEY_ORD_ADD);
	//t.append(symbol);

	redis->Send(t, o.AsJSON());
} 

void OrdersManager::OnOrderUpdate(const IMTOrder* order) {
	jOrder o = jOrder::fromMTOrder(*order);

	string t(KEY_ORD_UPD);
	//t.append(symbol);

	redis->Send(t, o.AsJSON());
}

void OrdersManager::OnOrderDelete(const IMTOrder* order) {
	jOrder o = jOrder::fromMTOrder(*order);

	string t(KEY_ORD_RM);
	t.append(std::to_string(o.account));

	redis->Send(t, o.AsJSON());
}