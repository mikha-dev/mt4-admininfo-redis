#include "stdafx.h"
#include "api_manager.hpp"

//__CPP_REDIS_LOGGING_ENABLED;

CManagerInterface *ApiManager::pump = NULL;
CManagerInterface *ApiManager::man = NULL;
RedisClient *ApiManager::redis_client = NULL;

void __stdcall ApiManager::PumpingFunc(int code, int type, void * data, void *param) {
	if (code == PUMP_UPDATE_MARGINCALL || code == PUMP_UPDATE_TRADES) {
		if (code == PUMP_UPDATE_MARGINCALL) {
			cout << "PUMP_UPDATE_MARGINCALL..." << endl;

			if (data == NULL)
				return;

			ApiManager::FillCacheMargins();

			MarginLevel level;

			try {
				level = *(MarginLevel*)data;				cout << "account: " << level.login << endl;

				JLevel jlevel= JLevel::FromMarginLevel(level);
				redis_client->Send("margin_call:", jlevel.AsJSON());			}			catch (const exception & ex) {
				cout << ex.what() << endl;			}
		}

	}			

	if (code == PUMP_UPDATE_GROUPS) {
		cout << "PUMP_UPDATE_GROUPS...." << endl;

		if (data == NULL)
			return;
		FillCacheGroups();

		ConGroup group;

		try {
			group = *(ConGroup*)data;			cout << "group: " << group.group << endl;

			JGroup jgroup = JGroup::FromConGroup(group);
			redis_client->Send("group:" + jgroup.group, jgroup.AsJSON());		}		catch (const exception & ex) {
			cout << ex.what() << endl;		}

		cout << "group updated." << endl;
	}

	if (code == PUMP_UPDATE_SYMBOLS) {
		cout << "PUMP_UPDATE_SYMBOLS...." << endl;

		if (data == NULL)
			return;
		FillCacheSymbols();

		ConSymbol symbol;

		try {
			symbol = *(ConSymbol*)data;			cout << "symbol: " << symbol.symbol << endl;

			JSymbol jsymbol = JSymbol::FromConSymbol(symbol);
			redis_client->Send("symbol:" + jsymbol.symbol, jsymbol.AsJSON());		} catch (const exception & ex) {
			cout << ex.what() << endl;		}

		cout << "symbol updated." << endl;
	}
}

bool ApiManager::Start(const string mt4_server_, const int mt4_login_, const string mt4_password_, const string redis_host, int redis_port, bool show_debug) {

	mt4_server = mt4_server_;
	mt4_password = mt4_password_;
	mt4_login = mt4_login_;

	if (show_debug)
		cout << "connecting manager..." << endl;

	connectManagers();

	if (show_debug)
		cout << "manager connected to " << mt4_server << endl;

	redis_client = new RedisClient(show_debug);
	redis_client->Connect(redis_host, redis_port);
	redis_client->Send("init", "{\"init\":1}");

	fillCache();

	return true;
}

void ApiManager::Stop() {

	if (redis_client) {
		redis_client->Disconnect();
		redis_client = NULL;
	} 

	if (man) {
		man->Disconnect();
		man->Release();
		man = NULL;
	}

	if (pump) {
		pump->Disconnect();
		pump->Release();
		pump = NULL;
	}

	if (factory)
		factory->WinsockCleanup();
}

void ApiManager::connectManagers() {
	int res;

	factory = new CManagerFactory("mtmanapi64.dll");

	if (factory->WinsockStartup() != RET_OK) {
		throw exception("WinsockStartup failed");
	}

	if (factory->IsValid() == FALSE) {
		throw exception("Failed to create factory");
	}

	man = factory->Create(ManAPIVersion);

	if (!man) {
		throw exception("Failed to create manager");
	}

	if (man->IsConnected())
		man->Disconnect();

	pump = factory->Create(ManAPIVersion);

	if (!pump) {
		throw exception("Failed to create manager with pump");
	}

	if (pump->IsConnected())
		pump->Disconnect();

	if ((res = man->Connect(mt4_server.c_str())) == RET_OK) {
		res = man->Login(mt4_login, mt4_password.c_str());
		if (res != RET_OK) {
			throw ManagerException(res, man->ErrorDescription(res));
		}
	}
	else {
		throw ManagerException(res, man->ErrorDescription(res));
	}

	if ((res = pump->Connect(mt4_server.c_str())) == RET_OK) {
		res = pump->Login(mt4_login, mt4_password.c_str());
		if (res != RET_OK) {
			throw ManagerException(res, pump->ErrorDescription(res));
		}
	}
	else {
		throw ManagerException(res, pump->ErrorDescription(res));
	}

	if ((res = pump->PumpingSwitchEx(ApiManager::PumpingFunc, CLIENT_FLAGS_HIDENEWS | CLIENT_FLAGS_HIDEMAIL, 0)) != RET_OK) {
		throw ManagerException(res, man->ErrorDescription(res));
	}
}

void ApiManager::FillCacheMargins() {
	MarginLevel* levels;
	int total;
	vector<string> jLevels;

	cout << "FillCacheMargins" << endl;

	levels = pump->MarginsGet(&total);
	cout << "marings total: " << total << endl;

	if (total == 0) {
		return;
	}

	for (int i = 0; i < total; i++) {
		string key("level:");
		key.append(to_string(levels[i].login));

		JLevel jLevel = JLevel::FromMarginLevel(levels[i]);

		string json(jLevel.AsJSON());
		redis_client->CacheSet(key, json);
		jLevels.push_back(json);
	}

	ostringstream oss;
	oss << "{\"margins\":[";
	std::copy(jLevels.begin(), jLevels.end() - 1, std::ostream_iterator<string>(oss, ","));
	oss << jLevels.back();
	oss << "]}";

	redis_client->CacheSet("levels_all", oss.str());

	cout << oss.str() << endl;

	pump->MemFree(levels);
}

void ApiManager::FillCacheSymbols() {
	ConSymbol* symbols;
	int total;
	vector<string> jSymbols;

	cout << "FillCacheSymbols" << endl;

	man->SymbolsRefresh();
	symbols = man->SymbolsGetAll(&total);

	cout << "symbols total: " << total << endl;

	if (total == 0) {
		return;
	}

	for (int i = 0; i < total; i++) {
		string key("symbol:");
		key.append(symbols[i].symbol);

		JSymbol jSymbol = JSymbol::FromConSymbol(symbols[i]);

		string json(jSymbol.AsJSON());
		redis_client->CacheSet(key, json);
		jSymbols.push_back(json);
	}

	ostringstream oss;
	oss << "{\"symbols\":[";
	std::copy(jSymbols.begin(), jSymbols.end() - 1, std::ostream_iterator<string>(oss, ","));
	oss << jSymbols.back();
	oss << "]}";

	redis_client->CacheSet("symbols_all", oss.str());

	cout << oss.str() << endl;

	man->MemFree(symbols);
}

void ApiManager::FillCacheGroups() {

	cout << "FillCacheGroups" << endl;

	ConGroup *groups;
	int total;
	vector<string> jGroups;

	groups = man->GroupsRequest(&total);
	cout << "groups total: " << total << endl;

	if (total == 0)
		return;

	for (int i = 0; i < total; i++) {
		string key("group:");
		key.append(groups[i].group);

		//cout << "processing group: " << groups[i].group << endl;

		JGroup jGroup = JGroup::FromConGroup(groups[i]);

		string json(jGroup.AsJSON());
		redis_client->CacheSet(key, json);
		jGroups.push_back(json);

		//cout << json << endl;
	}

	ostringstream oss;
	oss << "{\"groups\":[";
	std::copy(jGroups.begin(), jGroups.end() - 1, std::ostream_iterator<string>(oss, ","));
	oss << jGroups.back();
	oss << "]}";

	//"{[{},{}]}"

	redis_client->CacheSet("groups_all", oss.str());

	cout << oss.str() << endl; 
	man->MemFree(groups);
}

void ApiManager::fillCache() {
	FillCacheSymbols();
	FillCacheGroups();
	FillCacheMargins();
}
