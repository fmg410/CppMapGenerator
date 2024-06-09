#include "MyMap.h"
#include "JsonFunctions.h"

MyMap::MyMap()
	: dimensions(0, 0)
	, markedTile(-1, -1)
{}

void MyMap::setTerrain(std::vector<std::vector<Tile>> _terrain)
{
	terrain = _terrain;
	dimensions.x = terrain.size();
	if (dimensions.x > 0)
	{
		dimensions.y = terrain.at(0).size();
	}
	else
	{
		terrain.clear();
		dimensions.x = 0;
		dimensions.y = 0;
	}
}

void MyMap::drawTile(sf::RenderTarget* target, sf::Vector2i position)
{
	sf::RectangleShape tile(Tile::tileSize);
	sf::Texture texture;
	try {
		texture = getTile(position).generateTexture();
	}
	catch (const std::out_of_range& e) {
		return;
	}
	tile.setTexture(&texture);
	tile.setPosition(sf::Vector2f(position.x * Tile::tileSize.x, position.y * Tile::tileSize.y));
	if (position == markedTile)
	{
		float thickness = sqrt(Tile::tileSize.x * Tile::tileSize.y) * 0.1;
		tile.setSize(sf::Vector2f(Tile::tileSize.x - thickness * 2, Tile::tileSize.y - thickness * 2));
		tile.move(sf::Vector2f(thickness, thickness));
		tile.setOutlineThickness(thickness);
		tile.setOutlineColor(sf::Color::Red);
	}
	target->draw(tile);
}

void MyMap::drawTile(sf::RenderTarget* target, sf::Vector2f worldPosition)
{
	drawTile(target, sf::Vector2i((int)(worldPosition.x / Tile::tileSize.x), (int)(worldPosition.y / Tile::tileSize.y)));
}

void MyMap::drawMap(sf::RenderTarget* target)
{
	for (int i = 0; i < dimensions.x; i++)
	{
		for (int j = 0; j < dimensions.y; j++)
		{
			drawTile(target, sf::Vector2i(i, j));
		}
	}
}

void MyMap::updateTiles(std::vector<sf::Vector2i> positions)
{
	sf::RenderTexture render;

	if (!render.create(dimensions.x * Tile::tileSize.x, dimensions.y * Tile::tileSize.y))
	{
		return;
	}

	sf::RectangleShape map(sf::Vector2f(dimensions.x * Tile::tileSize.x, dimensions.y * Tile::tileSize.y));
	map.setOrigin(0.0f, 0.0f);
	map.setTexture(mapTexture.get());
	render.draw(map);

	for (sf::Vector2i position : positions)
	{
		drawTile(&render, position);
	}

	render.display();

	if (mapTexture.get() != nullptr)
		*mapTexture.get() = render.getTexture();
	//mapTexture = std::make_unique<sf::Texture>(render.getTexture());
}

void MyMap::updateTiles(std::vector<sf::Vector2f> worldPositions)
{
	std::vector<sf::Vector2i> positions;
	for (sf::Vector2f worldPosition : worldPositions)
	{
		positions.push_back(sf::Vector2i((int)(worldPosition.x / Tile::tileSize.x), (int)(worldPosition.y / Tile::tileSize.y)));
	}
	updateTiles(positions);
}

void MyMap::updateMapTexture()
{
	sf::RenderTexture render;

	if (!render.create(dimensions.x * Tile::tileSize.x, dimensions.y * Tile::tileSize.y))
	{
		return;
	}

	for (int i = 0; i < dimensions.x; i++)
	{
		for (int j = 0; j < dimensions.y; j++)
		{
			drawTile(&render, sf::Vector2i(i, j));
		}
	}

	render.display();

	mapTexture = std::make_unique<sf::Texture>(render.getTexture());
}

void MyMap::markTile(sf::Vector2i position)
{
	if (position.x >= 0 && position.y >= 0 && position.x < dimensions.x && position.y < dimensions.y)
	{
		std::vector<sf::Vector2i> toUpdate;
		if (markedTile.x >= 0 && markedTile.y >= 0)
			toUpdate.push_back(markedTile);
		if (markedTile == position)
			unMarkTile();
		else
			markedTile = position;
		toUpdate.push_back(markedTile);
		updateTiles(toUpdate);
	}
}

void MyMap::markTile(sf::Vector2f worldPosition)
{
	markTile(sf::Vector2i((int)(worldPosition.x / Tile::tileSize.x), (int)(worldPosition.y / Tile::tileSize.y)));
}

void MyMap::unMarkTile()
{
	markedTile.x = -1;
	markedTile.y = -1;
}

void MyMap::generateMap(Config& config)
{
	clearMap();

	generateTerrain(config);

	if (config.getGenerateChests())
		generateChests(config);

	if (config.getGenerateMonsters())
		generateMonsters(config);

	generateItems(config);

	updateMapTexture();
}

void MyMap::generateTerrain(Config& config)
{
	dimensions.x = config.getMapWidth();
	dimensions.y = config.getMapHeight();

	sf::Vector2i randomDirection = Directions::NONE;
	sf::Vector2i previousDirection = Directions::NONE;

	int** tunnelMap = new int* [dimensions.x];
	for (int i = 0; i < dimensions.x; i++)
	{
		tunnelMap[i] = new int[dimensions.y];
	}

	for (int i = 0; i < dimensions.x; i++)
	{
		for (int j = 0; j < dimensions.y; j++)
		{
			tunnelMap[i][j] = 0;
		}
	}

	int rightBorder, leftBorder, topBorder, bottomBorder;

	if (config.getIsBorderAroundMap())
	{
		rightBorder = dimensions.x - 2;
		bottomBorder = dimensions.y - 2;
		leftBorder = 1;
		topBorder = 1;
	}
	else
	{
		rightBorder = dimensions.x - 1;
		bottomBorder = dimensions.y - 1;
		leftBorder = 0;
		topBorder = 0;
	}

	int currentTunnelLength;
	int currentTunnelCount;

	int currentColumn;
	int currentRow;

	if (config.getIsMapGeneratedFromMiddle())
	{
		currentColumn = dimensions.y / 2;
		currentRow = dimensions.x / 2;
	}
	else
	{
		if (config.getIsBorderAroundMap())
		{
			currentColumn = rand() % (dimensions.y - 2) + 1;
			currentRow = rand() % (dimensions.x - 2) + 1;
		}
		else
		{
			currentColumn = rand() % dimensions.y;
			currentRow = rand() % dimensions.x;
		}
	}

	tunnelMap[currentColumn][currentRow] = 1;

	currentTunnelCount = rand() % (config.getMaxTunnelCount() - config.getMinTunnelCount()) + config.getMinTunnelCount() + 1;

	for (int i = 0; i < currentTunnelCount;)
	{
		do {
			randomDirection = Directions::randomDirection();
		} while (randomDirection == previousDirection || randomDirection == -previousDirection);

		currentTunnelLength = rand() % config.getMaxTunnelLength() + 1;

		for (int j = 0; j < currentTunnelLength; j++)
		{
			if (randomDirection == Directions::UP || randomDirection == Directions::DOWN)
			{
				if (currentColumn + randomDirection.y >= bottomBorder || currentColumn + randomDirection.y < topBorder)
				{
					break;
				}
				currentColumn += randomDirection.y;
			}
			else
			{
				if (randomDirection == Directions::RIGHT || randomDirection == Directions::LEFT)
				{
					if (currentRow + randomDirection.x >= rightBorder || currentRow + randomDirection.x < leftBorder)
					{
						break;
					}
					currentRow += randomDirection.x;
				}
			}
			tunnelMap[currentColumn][currentRow] = 1;
		}

		if (currentTunnelLength)
		{
			previousDirection = randomDirection;
			i++;
		}
	}

	//for (int i = 0; i < dimensions.x; i++)
	//{
	//	for (int j = 0; j < dimensions.y; j++)
	//	{
	//		std::cout << tunnelMap[j][i] << " "; // testing only
	//	}
	//	std::cout << "\n";
	//}

	std::vector<int> crossableTiles;
	std::vector<int> notCrossableTiles;
	std::vector<Tile> tileColumn;

	for (int ID : config.getAvailableTileIDs())
	{
		try {
			if (Tile::tileTypes.getEntry(ID).getIsCrossable())
				crossableTiles.push_back(ID);
			else
				notCrossableTiles.push_back(ID);
		}
		catch (const std::out_of_range& e) {
			config.removeID(ID);
		}
	}

	for (int i = 0; i < dimensions.x; i++)
	{
		for (int j = 0; j < dimensions.y; j++)
		{
			if (tunnelMap[i][j])
			{
				if (crossableTiles.size() > 0)
					tileColumn.push_back(Tile::tileTypes.getEntry(crossableTiles.at(rand() % crossableTiles.size())));
			}
			else
			{
				if (notCrossableTiles.size() > 0)
					tileColumn.push_back(Tile::tileTypes.getEntry(notCrossableTiles.at(rand() % notCrossableTiles.size()))); // add exception when no tiles available, map cannot consist of nothing // add default tiles
			}
			/*std::cout << i << " " << j << std::endl; // for testing purposes only*/
		}
		terrain.push_back(tileColumn);
		tileColumn.clear();
	}

	for (int i = 0; i < dimensions.x; i++)
	{
		delete[] tunnelMap[i];
	}
	delete[] tunnelMap;
}

void MyMap::generateChests(Config& config)
{
	int** tempMap;
	int chestCount = 0;

	tempMap = new int* [dimensions.x];
	for (int i = 0; i < dimensions.x; i++)
		tempMap[i] = new int[dimensions.y];

	for (int i = 0; i < dimensions.x; i++)
		for (int j = 0; j < dimensions.y; j++)
			tempMap[i][j] = terrain[i][j].getIsCrossable() && !terrain[i][j].getPlacedObject();

	if (!config.getCanChestsBlockPath())
		avoidBlockades(tempMap);

	int currentChestCount;

	for (currentChestCount = 0; currentChestCount < config.getChestCount();)
	{
		int height;
		int wideness;
		int failCount = 0;

		do {
			height = rand() % dimensions.y;
			wideness = rand() % dimensions.x;
			failCount++;

			if (failCount > dimensions.x * dimensions.y)
			{
				break;
			}

		} while (tempMap[wideness][height] != 1);

		if (failCount > dimensions.x * dimensions.y)
		{
			for (height = 0; height < dimensions.y - 1 && tempMap[wideness][height] != 1; height++)
			{
				for (wideness = 0; wideness < dimensions.x - 1 && tempMap[wideness][height] != 1; wideness++) {}
			}
		}

		if (tempMap[wideness][height] == 1)
		{
			terrain[wideness][height].setTypeOfPlacedObject(ObjectChild::CHEST);

			currentChestCount++;
			tempMap[wideness][height] = 0;

			for (int i = wideness - config.getMinDistanceChest(); i < wideness + config.getMinDistanceChest(); i++)
			{
				for (int j = height - config.getMinDistanceChest(); j < height + config.getMinDistanceChest(); j++)
				{
					if (i < dimensions.x && j < dimensions.y && i >= 0 && j >= 0)
					{
						tempMap[i][j] = 0;
					}
					else
					{
						if (i < 0)
						{
							i = 0;
						}
						if (j < 0)
						{
							j = 0;
						}
					}
				}
			}
		}
		else
		{
			break;
		}
	}

	for (int i = 0; i < dimensions.x; i++)
		delete[] tempMap[i];
	delete[] tempMap;

	// item generation

	/*for (int i = 0; i < dimensions.x; i++)
	{
		for(int j = 0; j < dimensions.y; j++)
		{
			Tile& tile = terrain.at(i).at(j);

			if (tile.getPlacedObject() != nullptr && tile.getPlacedObject()->getClassType() == ObjectChild::CHEST)
			{
				Chest* chest = dynamic_cast<Chest*>(tile.getPlacedObject());
				if (config.getIsItemsInChestCountRandom())
				{
					chest->setItemLimit(rand() % (config.getMaxItemsInChestCount() + 1));
				}
				else
				{
					chest->setItemLimit(config.getMaxItemsInChestCount());
				}

				std::vector<Item> rolledItems;
				int itemNumber;

				for (int k = 0; k < chest->getItemLimit(); k++)
				{
					itemNumber = rand() % Item::itemTypes.getEntryCount() + 1;

					std::map<int, Item>::iterator itr = Item::itemTypes.getIterator();
					for (; itr != Item::itemTypes.getEnd(); ++itr)
					{
						--itemNumber;
						if (!itemNumber)
							break;
					}
					rolledItems.push_back(itr->second);

				}
				for (Item& item : rolledItems)
					chest->addItem(item);
			}
		}
	}*/
}

void MyMap::generateMonsters(Config& config)
{
	int** tempMap;
	int monsterCount = 0;

	tempMap = new int* [dimensions.x];
	for (int i = 0; i < dimensions.x; i++)
		tempMap[i] = new int[dimensions.y];

	for (int i = 0; i < dimensions.x; i++)
		for (int j = 0; j < dimensions.y; j++)
			tempMap[i][j] = terrain[i][j].getIsCrossable() && !terrain[i][j].getPlacedObject();

	if (!config.getCanMonstersBlockPath())
		avoidBlockades(tempMap);

	int currentMonsterCount;

	for (currentMonsterCount = 0; currentMonsterCount < config.getMonsterCount();)
	{
		int height;
		int wideness;
		int failCount = 0;

		do {
			height = rand() % dimensions.y;
			wideness = rand() % dimensions.x;
			failCount++;

			if (failCount > dimensions.x * dimensions.y)
			{
				break;
			}

		} while (tempMap[wideness][height] != 1);

		if (failCount > dimensions.x * dimensions.y)
		{
			for (height = 0; height < dimensions.y - 1 && tempMap[wideness][height] != 1; height++)
			{
				for (wideness = 0; wideness < dimensions.x - 1 && tempMap[wideness][height] != 1; wideness++) {}
			}
		}

		if (tempMap[wideness][height] == 1)
		{
			int monsterNumber;
			monsterNumber = rand() % Monster::monsterTypes.getEntryCount() + 1;

			std::map<int, Monster>::iterator itr = Monster::monsterTypes.getIterator();
			for (; itr != Monster::monsterTypes.getEnd(); ++itr)
			{
				--monsterNumber;
				if (!monsterNumber)
					break;
			}

			terrain[wideness][height].setPlacedObject(new Monster(itr->second));

			currentMonsterCount++;
			tempMap[wideness][height] = 0;

			for (int i = wideness - config.getMinDistanceMonster(); i < wideness + config.getMinDistanceMonster(); i++)
			{
				for (int j = height - config.getMinDistanceMonster(); j < height + config.getMinDistanceMonster(); j++)
				{
					if (i < dimensions.x && j < dimensions.y && i >= 0 && j >= 0)
					{
						tempMap[i][j] = 0;
					}
					else
					{
						if (i < 0)
						{
							i = 0;
						}
						if (j < 0)
						{
							j = 0;
						}
					}
				}
			}
		}
		else
		{
			break;
		}
	}

	for (int i = 0; i < dimensions.x; i++)
		delete[] tempMap[i];
	delete[] tempMap;

	// item generation

	/*if (!config.getMonstersHaveItems())
		return;

	for (auto vector : terrain)
	{
		for (Tile& tile : vector)
		{
			if (tile.getPlacedObject() != nullptr && tile.getPlacedObject()->getClassType() == ObjectChild::MONSTER)
			{
				Monster* monster = dynamic_cast<Monster*>(tile.getPlacedObject());
				if (config.getIsItemsInChestCountRandom())
				{
					monster->setItemLimit(rand() % config.getMaxItemsInChestCount() + 1);
				}
				else
				{
					monster->setItemLimit(config.getMaxItemsInChestCount());
				}

				std::vector<Item> rolledItems;
				int itemNumber;

				for (int i = 0; i < monster->getItemLimit(); i++)
				{
					itemNumber = rand() % Item::itemTypes.getEntryCount() + 1;

					std::map<int, Item>::iterator itr = Item::itemTypes.getIterator();
					for (; itr != Item::itemTypes.getEnd(); ++itr)
					{
						--itemNumber;
						if (!itemNumber)
							break;
					}
					rolledItems.push_back(itr->second);

				}
				for (Item& item : rolledItems)
					monster->addItem(item);
			}
		}
	}*/
}

void MyMap::generateItems(Config& config)
{
	for (int i = 0; i < dimensions.x; i++)
	{
		for (int j = 0; j < dimensions.y; j++)
		{
			Tile& tile = terrain.at(i).at(j);

			if (tile.getPlacedObject() != nullptr && (tile.getPlacedObject()->getClassType() == ObjectChild::CHEST || tile.getPlacedObject()->getClassType() == ObjectChild::MONSTER))
			{
				if (tile.getPlacedObject()->getClassType() == ObjectChild::MONSTER)
					if (!config.getMonstersHaveItems())
						continue;

				Chest* chest = dynamic_cast<Chest*>(tile.getPlacedObject());

				if (config.getIsItemsInChestCountRandom())
				{
					chest->setItemLimit(rand() % (config.getMaxItemsInChestCount() + 1));
				}
				else
				{
					chest->setItemLimit(config.getMaxItemsInChestCount());
				}

				std::vector<Item> rolledItems;
				int itemNumber;

				for (int k = 0; k < chest->getItemLimit(); k++)
				{
					itemNumber = rand() % Item::itemTypes.getEntryCount() + 1;

					std::map<int, Item>::iterator itr = Item::itemTypes.getIterator();
					for (; itr != Item::itemTypes.getEnd(); ++itr)
					{
						--itemNumber;
						if (!itemNumber)
							break;
					}
					rolledItems.push_back(itr->second);

				}
				for (Item& item : rolledItems)
					chest->addItem(item);
			}
		}
	}
}

void MyMap::saveToFile(const std::string& fileName)
{
	json j = *this;
	std::ofstream file(fileName, std::ofstream::out);
	if (file.is_open())
	{
		file << j;
		file.close();
	}
}

void MyMap::loadFromFile(const std::string& fileName)
{
	std::ifstream file(fileName, std::ifstream::in);
	if (file.is_open())
	{
		*this = json::parse(file);
		file.close();
	}
}

void MyMap::clearMap()
{
	terrain.clear();
	dimensions.x = 0;
	dimensions.y = 0;
	unMarkTile();
	mapTexture.reset();
}

void MyMap::saveAsImage(const std::string& fileName)
{
	updateMapTexture();
	sf::Image image = mapTexture.get()->copyToImage();
	image.saveToFile(fileName);
}

void MyMap::avoidBlockades(int** tempMap)
{
	int nearbyField[3][3];

	for (int i = 0; i < dimensions.x; i++)
	{
		for (int j = 0; j < dimensions.y; j++)
		{
			if (terrain[i][j].getIsCrossable() && !terrain[i][j].getPlacedObject() == 1)
			{
				for (int k = -1; k < 2; k++)
				{
					for (int l = -1; l < 2; l++)
					{
						if (((i + k) != -1 && (j + l) != -1) && ((i + k) < dimensions.x && (j + l) < dimensions.y))
						{
							nearbyField[k + 1][l + 1] = terrain[i + k][j + l].getIsCrossable() && !terrain[i][j].getPlacedObject();
						}
						else
						{
							nearbyField[k + 1][l + 1] = 0;
						}
					}
				}

				tempMap[i][j] = 1;

				if (nearbyField[1][2] + nearbyField[1][0] + nearbyField[2][1] + nearbyField[0][1] > 1)
				{
					if (nearbyField[1][0] && nearbyField[1][2])
					{
						if (!((nearbyField[2][0] && nearbyField[2][1] && nearbyField[2][2]) || (nearbyField[0][0] && nearbyField[0][1] && nearbyField[0][2])))
						{
							tempMap[i][j] = 0;
						}
					}
					if (nearbyField[0][1] && nearbyField[2][1])
					{
						if (!((nearbyField[0][2] && nearbyField[1][2] && nearbyField[2][2]) || (nearbyField[0][0] && nearbyField[1][0] && nearbyField[2][0])))
						{
							tempMap[i][j] = 0;
						}
					}

					if (nearbyField[0][1] && nearbyField[1][0])
					{
						if (!nearbyField[0][0])
						{
							tempMap[i][j] = 0;
						}
					}
					if (nearbyField[1][0] && nearbyField[2][1])
					{
						if (!nearbyField[2][0])
						{
							tempMap[i][j] = 0;
						}
					}
					if (nearbyField[2][1] && nearbyField[1][2])
					{
						if (!nearbyField[2][2])
						{
							tempMap[i][j] = 0;
						}
					}
					if (nearbyField[1][2] && nearbyField[0][1])
					{
						if (!nearbyField[0][2])
						{
							tempMap[i][j] = 0;
						}
					}
				}
			}
		}
	}
}

