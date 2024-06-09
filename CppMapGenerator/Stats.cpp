#include "Stats.h"

Stats::Stats()
{}

Stats::Stats(std::map<std::string, int> _statMap)
	: statMap(_statMap)
{}

std::string Stats::statsToString(Stats stats)
{
	std::stringstream statsString;
	for (std::map<std::string, int>::iterator itr = stats.statMap.begin(); itr != stats.statMap.end(); ++itr)
	{
		statsString << (itr->first) << ':' << (itr->second) << std::endl;
	}
	return statsString.str();
}

Stats Stats::stringToStats(std::string string)
{
	Stats stats;
	std::string streamLine;
	std::stringstream statStream(string);
	std::string statName;
	int statValue;

	/*while (!statStream.fail())
	{
		std::getline(statStream, streamLine);
		statName = 
	}




	if (6 == sscanf(string, "atak: %d\npredkosc ataku: %d\nobrazenia magiczne: %d\nzdrowie: %d\npancerz: %d\nodpornosc na magie: %d", &statystyki->atak, &statystyki->predkoscAtaku, &statystyki->obrazeniaMagiczne, &statystyki->zdrowie, &statystyki->pancerz, &statystyki->odpornoscNaMagie))
	{
		return 1;tsS
	}*/
	return Stats();
}