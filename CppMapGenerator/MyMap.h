#pragma once
#include <cmath>
#include <vector>
#include <map>
#include <set>
#include <fstream>

#include "SFML/Graphics.hpp"
#include "Tile.h"
#include "Object.h"
#include "Config.h"
#include "Directions.h"
#include <TGUI/TGUI.hpp>

class MyMap
{
private:
	sf::Vector2i dimensions;
	std::unique_ptr<sf::Texture> mapTexture;
	sf::Vector2i markedTile;
	std::vector<std::vector<Tile>> terrain;

	void avoidBlockades(int** tempMap);

	void generateTerrain(Config& config);
	void generateChests(Config& config);
	void generateMonsters(Config& config);
	void generateItems(Config& config);

public:
	MyMap();

	Tile& getTile(sf::Vector2i position) { return terrain.at(position.x).at(position.y); }
	Tile& getTile(sf::Vector2f worldPosition) { return terrain.at((int)(worldPosition.x / Tile::tileSize.x)).at((int)(worldPosition.y / Tile::tileSize.y)); }
	Tile& getMarkedTile() { return terrain.at(markedTile.x).at(markedTile.y); }
	sf::Vector2i getMarkedCoordinates() { return markedTile; }
	bool isMarkedTile() { return markedTile.x >= 0 && markedTile.y >= 0; }
	sf::Vector2i getDimensions() { return dimensions; }
	sf::Texture* getMapTexture() { return mapTexture.get(); }
	std::vector<std::vector<Tile>>& getTerrain() { return terrain; }
	void setTerrain(std::vector<std::vector<Tile>> _terrain);

	void drawTile(sf::RenderTarget* target, sf::Vector2i position);
	void drawTile(sf::RenderTarget* target, sf::Vector2f worldPosition);
	void drawMap(sf::RenderTarget* target);
	void updateTiles(std::vector<sf::Vector2i> positions);
	void updateTiles(std::vector<sf::Vector2f> worldPositions);
	void updateMapTexture();

	void markTile(sf::Vector2i position);
	void markTile(sf::Vector2f worldPosition);
	void unMarkTile();

	void generateMap(Config& config);

	void saveToFile(const std::string& fileName);
	void loadFromFile(const std::string& fileName);

	void clearMap();

	void saveAsImage(const std::string& fileName);

};