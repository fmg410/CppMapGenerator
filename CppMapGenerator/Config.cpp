#include "Config.h"

Config::Config()
	: Config(5, 5, 3, false, 20, 20, 7, 100, 50, true, true, 3, 3, false, false, true, true, true)
{}

Config::Config(int _monsterCount, int _chestCount, int _maxItemsInChestCount, bool _isItemsInChestCountRandom, int _mapHeight, int _mapWidth, int _maxTunnelLength, int _maxTunnelCount, int _minTunnelCount, bool _isBorderAroundMap, bool _isMapGeneratedFromMiddle, int _minDistanceChest, int _minDistanceMonster, bool _canChestsBlockPath, bool _canMonstersBlockPath, bool _generateChests, bool _generateMonsters, bool _monstersHaveItems)
	: monsterCount(_monsterCount)
	, chestCount(_chestCount)
	, maxItemsInChestCount(_maxItemsInChestCount)
	, isItemsInChestCountRandom(_isItemsInChestCountRandom)
	, mapHeight(_mapHeight)
	, mapWidth(_mapWidth)
	, maxTunnelLength(_maxTunnelLength)
	, maxTunnelCount(_maxTunnelCount)
	, minTunnelCount(_minTunnelCount)
	, isBorderAroundMap(_isBorderAroundMap)
	, isMapGeneratedFromMiddle(_isMapGeneratedFromMiddle)
	, minDistanceChest(_minDistanceChest)
	, minDistanceMonster(_minDistanceMonster)
	, canChestsBlockPath(_canChestsBlockPath)
	, canMonstersBlockPath(_canMonstersBlockPath)
	, generateChests(_generateChests)
	, generateMonsters(_generateMonsters)
	, monstersHaveItems(_monstersHaveItems)
{}

void Config::setConfigName(std::string& _configName)
{
	configName = _configName;
}

void Config::setMonsterCount(int _monsterCount)
{
	if (_monsterCount > 0)
		monsterCount = _monsterCount;
}

void Config::setChestCount(int _chestCount)
{
	if (_chestCount > 0)
		chestCount = _chestCount;
}

void Config::setMaxItemsInChestCount(int _maxItemsInChestCount)
{
	if (_maxItemsInChestCount >= 0)
		maxItemsInChestCount = _maxItemsInChestCount;
}

void Config::setIsItemsInChestCountRandom(bool _isItemsInChestCountRandom)
{
	isItemsInChestCountRandom = _isItemsInChestCountRandom;
}

void Config::setMapHeight(int _mapHeight)
{
	if (_mapHeight >= 0)
		mapHeight = _mapHeight;
}

void Config::setMapWidth(int _mapWidth)
{
	if (_mapWidth >= 0)
		mapWidth = _mapWidth;
}

void Config::setMaxTunnelLength(int _maxTunnelLength)
{
	if (_maxTunnelLength >= 0)
		maxTunnelLength = _maxTunnelLength;
}

void Config::setMaxTunnelCount(int _maxTunnelCount)
{
	if (_maxTunnelCount >= 0 && _maxTunnelCount >= minTunnelCount)
		maxTunnelCount = _maxTunnelCount;
}

void Config::setMinTunnelCount(int _minTunnelCount)
{
	if (_minTunnelCount >= 0 && _minTunnelCount <= maxTunnelCount)
		minTunnelCount = _minTunnelCount;
}

void Config::setIsBorderAroundMap(bool _isBorderAroundMap)
{
	isBorderAroundMap = _isBorderAroundMap;
}

void Config::setIsMapGeneratedFromMiddle(bool _isMapGeneratedFromMiddle)
{
	isMapGeneratedFromMiddle = _isMapGeneratedFromMiddle;
}

void Config::setMinDistanceChest(int _minDistanceChest)
{
	if (_minDistanceChest >= 0)
		minDistanceChest = _minDistanceChest;
}

void Config::setMinDistanceMonster(int _minDistanceMonster)
{
	if (_minDistanceMonster >= 0)
		minDistanceMonster = _minDistanceMonster;
}

void Config::setCanChestsBlockPath(bool _canChestsBlockPath)
{
	canChestsBlockPath = _canChestsBlockPath;
}

void Config::setCanMonstersBlockPath(bool _canMonstersBlockPath)
{
	canMonstersBlockPath = _canMonstersBlockPath;
}

void Config::setGenerateChests(bool _generateChests)
{
	generateChests = _generateChests;
}

void Config::setGenerateMonsters(bool _generateMonsters)
{
	generateMonsters = _generateMonsters;
}

void Config::setMonstersHaveItems(bool _monstersHaveItems)
{
	monstersHaveItems = _monstersHaveItems;
}
