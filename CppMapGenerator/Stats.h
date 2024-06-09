#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include "JsonConfig.h"

//comparator(const std::string& s1, const std::string& s2) {return s1.compare(s2); };

class Stats
{
private:
	mutable std::map<std::string, int> statMap;

public:
	Stats();
	Stats(std::map<std::string, int> _statMap);

	static std::string statsToString(Stats stats);
	static Stats stringToStats(std::string string);

	void addStat(const std::string& name, int value) { statMap.emplace(name, value); }
	void removeStat(const std::string& name) { statMap.erase(name); }

	void setStatMap(std::map<std::string, int> _statMap) { statMap = _statMap; }
	std::map<std::string, int> getStatMap() const { return statMap; }

};

//static void to_json(json& j, const Stats& stats);
//
//static void from_json(const json& j, Stats& stats); nie dziala

static void to_json(json& j, const Stats& stats) {
	/*std::map<std::string, int>::iterator itr;
	for (itr = stats.getStatMap().begin(); itr != stats.getStatMap().end(); ++itr)
	{
		j[itr->first] = itr->second;
	}*/

	/*for (auto const& x : stats.getStatMap())
	{
		j[x.first] = x.second;
	}*/

	j = stats.getStatMap();
}

static void from_json(const json& j, Stats& stats) {
	std::map<std::string, int> statMap = j;
	stats.setStatMap(statMap);
}