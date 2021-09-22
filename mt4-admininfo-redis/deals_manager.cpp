#include "stdafx.h"
#include "types.hpp"
#include "deals_manager.hpp"

void DealsManager::OnDealAdd(const IMTDeal* deal) {
	jDeal o = jDeal::fromDeal(*deal);

	string t(KEY_DEAL_ADD);
	//t.append(symbol);

	redis->Send(t, o.AsJSON());
} 

void DealsManager::OnDealUpdate(const IMTDeal* deal) {
	jDeal o = jDeal::fromDeal(*deal);

	string t(KEY_DEAL_UPD);
	//t.append(symbol);

	redis->Send(t, o.AsJSON());
}

void DealsManager::OnDealDelete(const IMTDeal* deal) {
	jDeal o = jDeal::fromDeal(*deal);

	string t(KEY_DEAL_RM);
	t.append(std::to_string(o.account));

	redis->Send(t, o.AsJSON());
}