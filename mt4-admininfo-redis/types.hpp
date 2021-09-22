#pragma once

#include "stdafx.h"
#include "MT4ManagerAPI.h"

using namespace std;

class ManagerException : public std::exception {
	char description[100];

public:
	int code;
	ManagerException(const int code_, const char * description_) {
		code = code_;
		strncpy_s(description, description_, 100);
	}

	const char * what() const throw() {
		return description;
	}

};

struct JSymbol {
	string symbol;                  // name
	string description;             // description
	int               type;                        // security group (see ConSymbolGroup)
	int               digits;                      // security precision
	int               trade;                       // trade mode
	double            contract_size;               // contract size

	static JSymbol FromConSymbol(const ConSymbol &symbol) {
		JSymbol s;

		s.symbol.append(symbol.symbol);
		s.description.append(symbol.description);

		s.type = symbol.type;
		s.digits = symbol.digits;
		s.trade = symbol.trade;
		s.contract_size = symbol.contract_size;

		return s;
	}


	string AsJSON() const {

		char buffer[255];

		sprintf_s(buffer, 255, "{\"symbol\":\"%s\",\"description\":\"%s\",\"type\":%d,\"digits\":%d,\"trade\":%d,\"contract_size\":%.2f}",
			symbol.c_str(), description.c_str(), type, digits, trade, contract_size);

		return string(buffer);
	}
};

struct JGroupSec {
	int               show, trade;            // enable show and trade for this group of securites
	int               execution;             // dealing mode-EXECUTION_MANUAL,EXECUTION_AUTO,EXECUTION_ACTIVITY
	double            comm_base;             // standart commission
	int               comm_type;             // commission type-COMM_TYPE_MONEY,COMM_TYPE_PIPS,COMM_TYPE_PERCENT
	int               comm_lots;             // commission lots mode-COMMISSION_PER_LOT,COMMISSION_PER_DEAL
	double            comm_agent;            // agent commission
	int               comm_agent_type;       // agent commission mode-COMM_TYPE_MONEY, COMM_TYPE_PIPS
	//---
	int               spread_diff;           // spread difference in compare with default security spread
	//---
	int               lot_min, lot_max;       // allowed minimal and maximal lot values
	int               lot_step;              // allowed step value (10 lot-1000, 1 lot-100, 0.1 lot-10)

	static JGroupSec FromConGroupSec(const ConGroupSec &sec) {
		JGroupSec s;

		s.show = sec.show;
		s.trade = sec.trade;
		s.execution = sec.execution;
		s.comm_base = sec.comm_base;
		s.comm_type = sec.comm_type;
		s.comm_lots = sec.comm_lots;
		s.comm_agent = sec.comm_agent;
		s.comm_agent_type = sec.comm_agent_type;

		s.spread_diff = sec.spread_diff;
		s.lot_min = sec.lot_min;
		s.lot_max = sec.lot_max;
		s.lot_step = sec.lot_step;

		return s;
	}


	string AsJSON() const {

		char buffer[1000];

		sprintf_s(buffer, 1000, "{\"show\":%d,\"trade\":%d, \"execution\":%d, \"comm_base\":%0.2f, \"comm_type\":%d,"
			"\"comm_lots\":%d, \"comm_agent\":%0.2f,\"comm_agent_type\":%d,\"spread_diff\":%d,\"lot_min\":%d,\"lot_max\":%d,\"lots_step\":%d}",
			show, trade, execution, comm_base, comm_type, comm_lots, comm_agent, comm_agent_type, spread_diff, lot_min, lot_max, lot_step);

		return string(buffer);
	}
};

struct JGroup {
	string group;                  // name
	string secs;

	static JGroup FromConGroup(const ConGroup &group) {
		JGroup g;
		vector<string> gs;

		g.group.append(group.group);

		for (int i = 0; i < MAX_SEC_GROUPS; i++) {

			if (group.secgroups[i].show != 1)
				continue;
			JGroupSec s = JGroupSec::FromConGroupSec(group.secgroups[i]);
			gs.push_back(s.AsJSON());

			//cout << s.AsJSON() << endl;
		}
		//
		ostringstream oss;
		std::copy(gs.begin(), gs.end() - 1, std::ostream_iterator<string>(oss, ","));
		oss << gs.back();

		g.secs.append(oss.str());

		return g;
	}


	string AsJSON() const {

		char buffer[8048];

		sprintf_s(buffer, 8048, "{\"group\":\"%s\",\"secs\":[%s]}",
			group.c_str(), secs.c_str());

		return string(buffer);
	}
};

struct JLevel {
	int               login;            // user login
	string            group;        // user group
	int               leverage;         // user leverage
	double            balance;          // balance+credit
	double            equity;           // equity
	int               volume;           // lots
	double            margin;           // margin requirements
	double            margin_free;      // free margin
	double            margin_level;     // margin level
	int               margin_type;      // margin controlling type (percent/currency)
	int               level_type;       // level type(ok/margincall/stopout)

	static JLevel FromMarginLevel(const MarginLevel &level) {
		JLevel l;

		l.group.append(level.group);

		l.login = level.login;
		l.leverage = level.leverage;
		l.balance = level.balance;
		l.equity = level.equity;
		l.volume = level.volume;
		l.margin = level.margin;
		l.margin_free = level.margin_free;
		l.margin_level = level.margin_level;
		l.margin_type = level.margin_type;
		l.level_type = level.level_type;

		return l;
	}


	string AsJSON() const {

		char buffer[255];

		sprintf_s(buffer, 255, "{\"login\":%d,\"group\":\"%s\",\"leverage\":%d,\"balance\":%0.2f,\"volume\":%d,"
			"\"margin\":%0.2f,\"margin_free\":%0.2f,\"margin_level\":%0.2f,\"margin_type\":%d,\"level_type\":%d}",
			login, group.c_str(), leverage, balance, volume, margin, margin_free, margin_level, margin_type, level_type);

		return string(buffer);
	}
};