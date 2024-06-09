#pragma once
#include <iostream>
#include <set>

class Config
{
public:
	Config();
	Config(int _monsterCount, int _chestCount, int _maxItemsInChestCount, bool _isItemsInChestCountRandom, int _mapHeight, int _mapWidth, int _maxTunnelLength, int _maxTunnelCount, int _minTunnelCount, bool _isBorderAroundMap, bool _isMapGeneratedFromMiddle, int _minDistanceChest, int _minDIstanceMonster, bool _canChestsBlockPath, bool _canMonstersBlockPath, bool _generateChests, bool _generateMonsters, bool _monstersHaveItems);

	void addID(int _ID) { availableTileIDs.insert(_ID); }
	void removeID(int _ID) { availableTileIDs.erase(_ID); }
	std::set<int> getAvailableTileIDs() { return availableTileIDs; }

private:
	std::set<int> availableTileIDs;
	std::string configName;

	int monsterCount;
	int chestCount;
	int maxItemsInChestCount;
	bool isItemsInChestCountRandom;
	int mapHeight;
	int mapWidth;
	int maxTunnelLength;
	int maxTunnelCount;
	int minTunnelCount;
	bool isBorderAroundMap;
	bool isMapGeneratedFromMiddle;
	int minDistanceChest;
	int minDistanceMonster;
	bool canChestsBlockPath;
	bool canMonstersBlockPath;
	bool generateChests;
	bool generateMonsters;
	bool monstersHaveItems;

public:
	std::string getConfigName() { return configName; }
	int getMonsterCount() { return monsterCount; }
	int getChestCount() { return chestCount; }
	int getMaxItemsInChestCount() { return maxItemsInChestCount; }
	bool getIsItemsInChestCountRandom() { return isItemsInChestCountRandom; }
	int getMapHeight() { return mapHeight; }
	int getMapWidth() { return mapWidth; }
	int getMaxTunnelLength() { return maxTunnelLength; }
	int getMaxTunnelCount() { return maxTunnelCount; }
	int getMinTunnelCount() { return minTunnelCount; }
	bool getIsBorderAroundMap() { return isBorderAroundMap; }
	bool getIsMapGeneratedFromMiddle() { return isMapGeneratedFromMiddle; }
	int getMinDistanceChest() { return minDistanceChest; }
	int getMinDistanceMonster() { return minDistanceMonster; }
	bool getCanChestsBlockPath() { return canChestsBlockPath; }
	bool getCanMonstersBlockPath() { return canMonstersBlockPath; }
	bool getGenerateChests() { return generateChests; }
	bool getGenerateMonsters() { return generateMonsters; }
	bool getMonstersHaveItems() { return monstersHaveItems; }

	void setConfigName(std::string& _configName);
	void setMonsterCount(int _monsterCount);
	void setChestCount(int _chestCount);
	void setMaxItemsInChestCount(int _maxItemsInChestCount);
	void setIsItemsInChestCountRandom(bool _isItemsInChestCountRandom);
	void setMapHeight(int _mapHeight);
	void setMapWidth(int _mapWidth);
	void setMaxTunnelLength(int _maxTunnelLength);
	void setMaxTunnelCount(int _maxTunnelCount);
	void setMinTunnelCount(int _minTunnelCount);
	void setIsBorderAroundMap(bool _isBorderAroundMap);
	void setIsMapGeneratedFromMiddle(bool _isMapGeneratedFromMiddle);
	void setMinDistanceChest(int _minDistanceChest);
	void setMinDistanceMonster(int _minDistanceMonster);
	void setCanChestsBlockPath(bool _canChestsBlockPath);
	void setCanMonstersBlockPath(bool _canMonstersBlockPath);
	void setGenerateChests(bool _generateChests);
	void setGenerateMonsters(bool _generateMonsters);
	void setMonstersHaveItems(bool _monstersHaveItems);
};

