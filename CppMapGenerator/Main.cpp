#include <iostream>
#include <map>
#include <list>
#include "JsonConfig.h"
#include "Stats.h"
#include "Monster.h"
#include "MyMap.h"
#include "Config.h"
#include "JsonFunctions.h"
#include <TGUI/TGUI.hpp>

static const float VIEW_HEIGHT = 512.0f;
static const float NO_GUI_WIDTH = 0.7f;

void resizeView(const sf::RenderWindow& window, sf::View& view)
{
	float aspectRatio = float(window.getSize().x) / float(window.getSize().y);
	view.setSize(VIEW_HEIGHT * aspectRatio * NO_GUI_WIDTH, VIEW_HEIGHT);
}

void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom, sf::View& view, int delta)
{
	// delta < 0 -> zoom out
	// zoom > 0 -> zoom out
	if (delta > 0)
	{
		const sf::Vector2f beforeCoord{ window.mapPixelToCoords(pixel) };
		view.zoom(1.0f / zoom);
		window.setView(view);
		const sf::Vector2f afterCoord{ window.mapPixelToCoords(pixel) };
		const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
		view.move(offsetCoords);
		window.setView(view);
	}
	else if (delta < 0)
	{
		view.zoom(zoom);
	}
}

void loadMainWindow(sf::RenderWindow& window, tgui::Gui& gui, MyMap& map, Config& config);
void loadLists(sf::RenderWindow& window, tgui::Gui& gui, MyMap& map, Config& config);
void loadTileSelected(sf::RenderWindow& window, tgui::Gui& gui, MyMap& map, Config& config);
void loadItemInfo(sf::RenderWindow& window, tgui::Gui& gui, MyMap& map, Config& config);
void loadConfig(sf::RenderWindow& window, tgui::Gui& gui, MyMap& map, Config& config);
void loadFilePane(sf::RenderWindow& window, tgui::Gui& gui, MyMap& map, Config& config, int mode); // modes: 0 - save map ; 1 - load map ; 2 - save map as image

void loadMainWindow(sf::RenderWindow& window, tgui::Gui& gui, MyMap& map, Config& config)
{
	gui.loadWidgetsFromFile("MainWindow.txt");
	gui.get<tgui::Button>("GenerateMapButton")->onPress(&MyMap::generateMap, &map, std::ref(config));
	/*gui.get<tgui::Button>("SaveMapButton")->onPress(&loadFilePane, std::ref(window), std::ref(gui), std::ref(map), std::ref(config), 0);
	gui.get<tgui::Button>("LoadMapButton")->onPress(&loadFilePane, std::ref(window), std::ref(gui), std::ref(map), std::ref(config), 1);
	gui.get<tgui::Button>("SaveMapAsImageButton")->onPress(&loadFilePane, std::ref(window), std::ref(gui), std::ref(map), std::ref(config), 2);*/
	gui.get<tgui::Button>("SaveMapButton")->onPress(&MyMap::saveToFile, &map, "MapJson.txt");
	gui.get<tgui::Button>("LoadMapButton")->onPress(&MyMap::loadFromFile, &map, "MapJson.txt");
	gui.get<tgui::Button>("SaveMapAsImageButton")->onPress(&MyMap::saveAsImage, &map, "MapImage.png");

	//gui.get<tgui::Button>("LoadMapButton")->onPress(&loadFilePane, std::ref(window), std::ref(gui), std::ref(map), std::ref(config), 1);
	//gui.get<tgui::Button>("SaveMapAsImageButton")->onPress(&loadFilePane, std::ref(window), std::ref(gui), std::ref(map), std::ref(config), 2);
	gui.get<tgui::Button>("TileInfoButton")->onClick(&loadTileSelected, std::ref(window), std::ref(gui), std::ref(map), std::ref(config));
	gui.get<tgui::Button>("LoadedListsButton")->onPress(&loadLists, std::ref(window), std::ref(gui), std::ref(map), std::ref(config));
	gui.get<tgui::Button>("ConfigButton")->onPress(&loadConfig, std::ref(window), std::ref(gui), std::ref(map), std::ref(config));
	gui.get<tgui::Button>("ExitButton")->onPress([&] {window.close(); });

}

void loadLists(sf::RenderWindow& window, tgui::Gui& gui, MyMap& map, Config& config)
{
	gui.loadWidgetsFromFile("LoadedLists.txt");

	gui.get<tgui::RadioButton>("ItemListRadioButton")->onCheck([&] {
		gui.get<tgui::RadioButton>("MonsterListRadioButton")->setChecked(false);
		gui.get<tgui::RadioButton>("TileListRadioButton")->setChecked(false);
		gui.get<tgui::ListBox>("LoadedListsBox")->removeAllItems();
		for (auto itr = Item::itemTypes.getIterator(); itr != Item::itemTypes.getEnd(); ++itr)
			gui.get<tgui::ListBox>("LoadedListsBox")->addItem(itr->second.getName(), std::to_string(itr->first)); });

	gui.get<tgui::RadioButton>("MonsterListRadioButton")->onCheck([&] {
		gui.get<tgui::RadioButton>("ItemListRadioButton")->setChecked(false);
		gui.get<tgui::RadioButton>("TileListRadioButton")->setChecked(false);
		gui.get<tgui::ListBox>("LoadedListsBox")->removeAllItems();
		for (auto itr = Monster::monsterTypes.getIterator(); itr != Monster::monsterTypes.getEnd(); ++itr)
			gui.get<tgui::ListBox>("LoadedListsBox")->addItem(itr->second.getName(), std::to_string(itr->first)); });

	gui.get<tgui::RadioButton>("TileListRadioButton")->onCheck([&] {
		gui.get<tgui::RadioButton>("MonsterListRadioButton")->setChecked(false);
		gui.get<tgui::RadioButton>("ItemListRadioButton")->setChecked(false);
		gui.get<tgui::ListBox>("LoadedListsBox")->removeAllItems();
		for (auto itr = Tile::tileTypes.getIterator(); itr != Tile::tileTypes.getEnd(); ++itr)
			gui.get<tgui::ListBox>("LoadedListsBox")->addItem(itr->second.getTextureFileName(), std::to_string(itr->first)); });

	gui.get<tgui::Button>("RemoveButton")->onPress([&] {
		if (gui.get<tgui::ListBox>("LoadedListsBox")->getSelectedItemIndex() != -1)
		{
			if (gui.get<tgui::RadioButton>("TileListRadioButton")->isChecked())
				Tile::tileTypes.removeEntry(gui.get<tgui::ListBox>("LoadedListsBox")->getSelectedItemId().toInt());
			else if (gui.get<tgui::RadioButton>("MonsterListRadioButton")->isChecked())
				Monster::monsterTypes.removeEntry(gui.get<tgui::ListBox>("LoadedListsBox")->getSelectedItemId().toInt());
			else if (gui.get<tgui::RadioButton>("ItemListRadioButton")->isChecked())
				Item::itemTypes.removeEntry(gui.get<tgui::ListBox>("LoadedListsBox")->getSelectedItemId().toInt());
		}});

	gui.get<tgui::ListBox>("LoadedListsBox")->onItemSelect([&] {
		if (gui.get<tgui::RadioButton>("TileListRadioButton")->isChecked())
		{
			if (Tile::tileTypes.containsID(gui.get<tgui::ListBox>("LoadedListsBox")->getSelectedItemId().toInt()))
				gui.get<tgui::Picture>("Picture")->getRenderer()->setTexture(tgui::Texture(*Tile::tileTypes.getEntry(gui.get<tgui::ListBox>("LoadedListsBox")->getSelectedItemId().toInt()).getTexturePtr()));

		}
		else if (gui.get<tgui::RadioButton>("MonsterListRadioButton")->isChecked())
		{
			if (Monster::monsterTypes.containsID(gui.get<tgui::ListBox>("LoadedListsBox")->getSelectedItemId().toInt()))
				gui.get<tgui::Picture>("Picture")->getRenderer()->setTexture(tgui::Texture(*Monster::monsterTypes.getEntry(gui.get<tgui::ListBox>("LoadedListsBox")->getSelectedItemId().toInt()).getTexturePtr()));

		}
		else if (gui.get<tgui::RadioButton>("ItemListRadioButton")->isChecked())
		{
			if (Item::itemTypes.containsID(gui.get<tgui::ListBox>("LoadedListsBox")->getSelectedItemId().toInt()))
				gui.get<tgui::Picture>("Picture")->getRenderer()->setTexture(tgui::Texture(*Item::itemTypes.getEntry(gui.get<tgui::ListBox>("LoadedListsBox")->getSelectedItemId().toInt()).getTexturePtr()));

		}});

	gui.get<tgui::ChildWindow>("LoadedListsWindow")->onClose(&loadMainWindow, std::ref(window), std::ref(gui), std::ref(map), std::ref(config));

}

void loadTileSelected(sf::RenderWindow& window, tgui::Gui& gui, MyMap& map, Config& config)
{
	gui.loadWidgetsFromFile("TileSelected.txt");

	gui.get<tgui::Button>("ChangeTileTypeButton")->onClick([&] {
		gui.get<tgui::ListBox>("ListsBox")->removeAllItems();
		gui.get<tgui::Label>("TypeLabel")->setText("Tiles");
		for (auto itr = Tile::tileTypes.getIterator(); itr != Tile::tileTypes.getEnd(); ++itr)
			gui.get<tgui::ListBox>("ListsBox")->addItem(itr->second.getTextureFileName(), std::to_string(itr->first)); });

	gui.get<tgui::Button>("ChangePlacedMonstertButton")->onClick([&] {
		gui.get<tgui::ListBox>("ListsBox")->removeAllItems();
		gui.get<tgui::Label>("TypeLabel")->setText("Monsters");
		for (auto itr = Monster::monsterTypes.getIterator(); itr != Monster::monsterTypes.getEnd(); ++itr)
			gui.get<tgui::ListBox>("ListsBox")->addItem(itr->second.getName(), std::to_string(itr->first)); });

	gui.get<tgui::Button>("ConfirmButton")->onClick([&] {
		bool didChange = false;
		if (gui.get<tgui::Label>("TypeLabel")->getText() == "Monsters")
		{
			if (Monster::monsterTypes.containsID(gui.get<tgui::ListBox>("ListsBox")->getSelectedItemId().toInt()))
			{
				map.getMarkedTile().setPlacedObject(new Monster(Monster::monsterTypes.getEntry(gui.get<tgui::ListBox>("ListsBox")->getSelectedItemId().toInt())));
				didChange = true;
			}
		}
		else if (gui.get<tgui::Label>("TypeLabel")->getText() == "Tiles")
		{
			if (Tile::tileTypes.containsID(gui.get<tgui::ListBox>("ListsBox")->getSelectedItemId().toInt()))
			{
				map.getTile(map.getMarkedCoordinates()) = Tile(Tile::tileTypes.getEntry(gui.get<tgui::ListBox>("ListsBox")->getSelectedItemId().toInt()));
				didChange = true;
			}
		}
		if (didChange)
		{
			std::vector<sf::Vector2i> toUpdate;
			toUpdate.push_back(map.getMarkedCoordinates());
			map.updateTiles(toUpdate);
			map.updateMapTexture();
		}
		gui.get<tgui::ListBox>("ListsBox")->removeAllItems();
		});

	gui.get<tgui::Button>("PlaceChestButton")->onClick([&] {
		map.getTile(map.getMarkedCoordinates()).setTypeOfPlacedObject(ObjectChild::CHEST);
		std::vector<sf::Vector2i> toUpdate;
		toUpdate.push_back(map.getMarkedCoordinates());
		map.updateTiles(toUpdate);
		map.updateMapTexture();
		/*sf::RectangleShape mapShape(sf::Vector2f(map.getDimensions().x * Tile::tileSize.x, map.getDimensions().y * Tile::tileSize.y));
		mapShape.setTexture(map.getMapTexture());
		mapShape.setPosition(0.0f, 0.0f);
		mapShape.setTexture(map.getMapTexture());
		window.draw(mapShape);*/
		});

	gui.get<tgui::Button>("ExitButton")->onClick(&loadMainWindow, std::ref(window), std::ref(gui), std::ref(map), std::ref(config));


}

void loadItemInfo(sf::RenderWindow& window, tgui::Gui& gui, MyMap& map, Config& config)
{
	std::cout << "iteminfo\n";
}

void loadConfig(sf::RenderWindow& window, tgui::Gui& gui, MyMap& map, Config& config)
{
	std::cout << "loadconfig\n";
}

void loadFilePane(sf::RenderWindow& window, tgui::Gui& gui, MyMap& map, Config& config, int mode)
{
	std::cout << "loadfilepane\n";
}

int main()
{
	sf::err().rdbuf(nullptr);
	srand(time(NULL));

	// example variables

	Monster monster1;
	Monster monster2;

	monster1.setTextureFileName("textures/stickManTransparent.png");
	monster2.setTextureFileName("textures/ufetubus.png");

	monster1.setName("Maniek");
	monster2.setName("Ufetubus");

	Monster::monsterTypes.addEntry(1, monster1);
	Monster::monsterTypes.addEntry(2, monster2);

	Tile tile1;
	Tile tile2;

	tile1.setIsCrossable(true);
	tile2.setIsCrossable(false);
	tile1.setTextureFileName("textures/tileGrass.png");
	tile2.setTextureFileName("textures/rock 2.png");

	Tile tile3;
	tile3.setIsCrossable(true);
	tile3.setTextureFileName("textures/grass1.png");
	Tile::tileTypes.addEntry(3, tile3);


	Tile::tileTypes.addEntry(1, tile1);
	Tile::tileTypes.addEntry(2, tile2);

	Config config;
	config.addID(1);
	config.addID(2);

	config.setMapWidth(20);
	config.setMapHeight(20);

	Item item1;
	Item item2;
	Item item3;

	item1.setTextureFileName("textures/rod_0_new.png");
	item2.setTextureFileName("textures/sky_blue.png");
	item3.setTextureFileName("textures/steel.png");

	item1.setName("rod");
	item2.setName("potion");
	item3.setName("ring");

	item1.setDescription("mage's");
	item2.setDescription("to drink");
	item3.setDescription("to marry");

	Stats stats1;
	Stats stats2;
	Stats stats3;

	stats1.addStat("attack", 1);
	stats2.addStat("defence", 2);
	stats3.addStat("agility", 3);

	item1.setStats(stats1);
	item2.setStats(stats2);
	item3.setStats(stats3);

	Item::itemTypes.addEntry(1, item1);
	Item::itemTypes.addEntry(2, item2);
	Item::itemTypes.addEntry(3, item3);

	MyMap myMap;
	myMap.generateMap(config);

	sf::RectangleShape mapShape(sf::Vector2f(myMap.getDimensions().x * Tile::tileSize.x, myMap.getDimensions().y * Tile::tileSize.y));
	mapShape.setTexture(myMap.getMapTexture());
	mapShape.setPosition(0.0f, 0.0f);

	sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

	sf::View view = window.getDefaultView();
	view.setCenter(sf::Vector2f(myMap.getDimensions().x / 2.0f * Tile::tileSize.x, myMap.getDimensions().y / 2.0f * Tile::tileSize.y));
	view.setViewport(sf::FloatRect(0.15f, 0.0f, 0.7f, 1.0f));
	view.setSize(window.getSize().x * NO_GUI_WIDTH, window.getSize().y);
	window.setView(view);

	tgui::GuiSFML gui;
	gui.setTarget(window);
	loadMainWindow(window, gui, myMap, config);

	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;


		while (window.pollEvent(event))
		{
			// crashed last time
			if (gui.handleEvent(event))
				continue;

			if (event.type == sf::Event::MouseWheelScrolled)
			{
				zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, 1.1f, view, event.mouseWheelScroll.delta);
			}
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{
					myMap.markTile(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
					if (myMap.isMarkedTile())
					{
						if (myMap.getMarkedTile().getPlacedObject() != nullptr)
						{
							for (auto itr = dynamic_cast<Chest*>(myMap.getMarkedTile().getPlacedObject())->getIterator(); itr != dynamic_cast<Chest*>(myMap.getMarkedTile().getPlacedObject())->getEnd(); ++itr)
							{
								std::cout << itr->getName() << itr->getDescription() << std::endl << Stats::statsToString(itr->accessStats()) << "\n";
							}
						}
					}
				}
			}

			if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
				{
					/*int i = 0;
					std::ifstream f("map0.png");
					while (f.good())
					{
						f.close();
						f.open(std::string("map").append(std::to_string(i)).append(".png")); // do doczytania
					}
					myMap.saveAsImage(std::string("map").append(std::to_string(i)).append(".png"));*/
					myMap.saveAsImage("map2.png");
				}
			}

			if (event.type == sf::Event::Resized)
				resizeView(window, view);
		}

		window.setView(view);
		window.clear();
		//window.draw(tileT);
		//myMap.drawMap(&window); //add framerate limit
		mapShape.setTexture(myMap.getMapTexture());
		window.draw(mapShape);
		gui.draw();
		window.display();
	}

	return 0;
}

//int main()
//{
//	sf::err().rdbuf(nullptr);
//	//json j;
//	srand(time(NULL));
//
//	json j;
//
//	j["k"] = 1;
//
//	std::map<std::string, int> map;
//
//	map.insert(std::pair<std::string, int>("Jeden", 1));
//	map.insert(std::pair<std::string, int>("Dwa", 2));
//	map.insert(std::pair<std::string, int>("Trzy", 3));
//	map.insert(std::pair<std::string, int>("Trzy", 4)); // tego nie dodaje
//
//	for (std::map<std::string, int>::iterator itr = map.begin(); itr != map.end(); ++itr)
//	{
//		std::cout << itr->first << std::endl;
//		std::cout << itr->second << std::endl;
//	}
//
//	Stats stats1;
//	stats1.addStat("Sila", 1);
//	json s = stats1;
//
//	std::cout << s << std::endl;
//
//	std::list<int> list;
//
//	/*std::list<int>::iterator it = list.begin();
//
//	std::cout << *it << std::endl;*/
//
//	list.push_back(2);
//	list.push_back(22);
//	list.push_back(49);
//	list.push_back(81);
//
//	std::cout << "\n";
//
//	for (std::list<int>::iterator itr = list.begin(); itr != list.end(); ++itr) // no difference between itr++ I guess
//	{
//		std::cout << *itr << std::endl;
//	}
//
//	Monster monster1;
//	Monster monster2;
//
//	monster1.setTextureFileName("textures/stickManTransparent.png");
//	monster2.setTextureFileName("textures/ufetubus.png");
//
//	Monster::monsterTypes.addEntry(1, monster1);
//	Monster::monsterTypes.addEntry(2, monster2);
//
//
//	Tile tile1;
//	Tile tile2;
//
//	tile1.setIsCrossable(true);
//	tile2.setIsCrossable(false);
//	tile1.setTextureFileName("textures/tileGrass.png");
//	tile2.setTextureFileName("textures/rock 2.png");
//
//	Tile tile3;
//	tile3.setIsCrossable(true);
//	tile3.setTextureFileName("textures/grass1.png");
//	Tile::tileTypes.addEntry(3, tile3);
//
//
//	Tile::tileTypes.addEntry(1, tile1);
//	Tile::tileTypes.addEntry(2, tile2);
//	Config config;
//	config.addID(1);
//	config.addID(2);
//	//config.addID(2);
//
//	config.setMapWidth(20);
//	config.setMapHeight(20);
//
//	Item item1;
//	Item item2;
//	Item item3;
//
//	item1.setTextureFileName("rod_0_new.png");
//	item2.setTextureFileName("sky_blue.png");
//	item3.setTextureFileName("steel.png");
//
//	item1.setName("rod");
//	item2.setName("potion");
//	item3.setName("ring");
//
//	item1.setDescription("mage's");
//	item2.setDescription("to drink");
//	item3.setDescription("to marry");
//
//	Stats stats;
//	stats.addStat("attack", 1);
//
//	item1.setStats(stats);
//	item2.setStats(Stats());
//	stats.addStat("defense", 1);
//	item3.setStats(stats);
//
//	Item::itemTypes.addEntry(1, item1);
//	Item::itemTypes.addEntry(2, item2);
//	Item::itemTypes.addEntry(3, item3);
//
//	MyMap myMap;
//	myMap.generateMap(config);
//
//	myMap.markTile(sf::Vector2i(0, 0));
//	myMap.markTile(sf::Vector2i(1, 1));
//
//	Tile& tileTest = myMap.getTile(sf::Vector2i(5, 5));
//	/*tileTest.setTypeOfPlacedObject(ObjectChild::MONSTER);
//	tileTest.getPlacedObject()->setTextureFileName("textures/stickManTransparent.png");*/
//
//	Tile& tileTest2 = myMap.getTile(sf::Vector2i(6, 6));
//	/*tileTest2.setTypeOfPlacedObject(ObjectChild::MONSTER);
//	tileTest2.getPlacedObject()->setTextureFileName("textures/ufetubus.png");*/
//	// add verification if a monster is flying or not if it is placed on a tile
//
//	sf::Texture tileTexture = tileTest.generateTexture();
//
//	sf::RectangleShape tileT(Tile::tileSize);
//	tileT.setTexture(&tileTexture);
//	tileT.setPosition(0.0f, 0.0f);
//
//	sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
//	sf::View view = window.getDefaultView();
//	view.setCenter(sf::Vector2f(myMap.getDimensions().x / 2.0f * Tile::tileSize.x, myMap.getDimensions().y / 2.0f * Tile::tileSize.y));
//	view.setViewport(sf::FloatRect(0.15f, 0.0f, 0.7f, 1.0f));
//	view.setSize(window.getSize().x * NO_GUI_WIDTH, window.getSize().y);
//	//view.setCenter(sf::Vector2f(0.0f, 0.0f));
//	window.setView(view);
//
//	//window.clear();
//	//window.draw(tileT);
//	//myMap.drawMap(&window);
//	//window.display();
//
//	myMap.updateMapTexture();
//	sf::RectangleShape mapShape(sf::Vector2f(myMap.getDimensions().x * Tile::tileSize.x, myMap.getDimensions().y * Tile::tileSize.y));
//	mapShape.setTexture(myMap.getMapTexture());
//	mapShape.setPosition(0.0f, 0.0f);
//
//	while (window.isOpen())
//	{
//		// check all the window's events that were triggered since the last iteration of the loop
//		sf::Event event;
//		while (window.pollEvent(event))
//		{
//			if (event.type == sf::Event::MouseWheelScrolled)
//			{
//				zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, 1.1f, view, event.mouseWheelScroll.delta);
//			}
//			// "close requested" event: we close the window
//			if (event.type == sf::Event::Closed)
//				window.close();
//
//			if (event.type == sf::Event::MouseButtonPressed)
//			{
//				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
//				{
//					myMap.markTile(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
//					if (myMap.isMarkedTile())
//					{
//						if (myMap.getMarkedTile().getPlacedObject() != nullptr)
//						{
//							for (auto itr = dynamic_cast<Chest*>(myMap.getMarkedTile().getPlacedObject())->getIterator(); itr != dynamic_cast<Chest*>(myMap.getMarkedTile().getPlacedObject())->getEnd(); ++itr)
//							{
//								std::cout << Stats::statsToString(itr->accessStats()) << "\n";
//							}
//						}
//					}
//				}
//			}
//
//			if (event.type == sf::Event::KeyPressed)
//			{
//				if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
//				{
//					/*int i = 0;
//					std::ifstream f("map0.png");
//					while (f.good())
//					{
//						f.close();
//						f.open(std::string("map").append(std::to_string(i)).append(".png")); // do doczytania
//					}
//					myMap.saveAsImage(std::string("map").append(std::to_string(i)).append(".png"));*/
//					myMap.saveAsImage("map2.png");
//				}
//			}
//
//			if (event.type == sf::Event::Resized)
//				resizeView(window, view);
//		}
//
//		window.setView(view);
//		window.clear();
//		//window.draw(tileT);
//		//myMap.drawMap(&window); //add framerate limit
//		mapShape.setTexture(myMap.getMapTexture());
//		window.draw(mapShape);
//		window.display();
//	}
//
//
//	return 0;
//}

//int main()
//{
//	//Item* items = new Item[3];
//	std::vector<Item> items(3);
//
//	for (int i = 0; i < 3; i++)
//	{
//		items[i].setName(std::string("name").append(std::to_string(i)));
//		items[i].setDescription(std::string("description").append(std::to_string(i)));
//		items[i].accessStats().addStat("attack", 1);
//	}
//
//	items[0].setTextureFileName(std::string("textures/rod_0_new.png"));
//	items[1].setTextureFileName(std::string("textures/sky_blue.png"));
//	items[2].setTextureFileName(std::string("textures/steel.png"));
//
//	json j = items;
//
//	json k = items[1];
//
//	std::vector<Item> itemsFromJson = j;
//
//
//	std::cout << j << std::endl;
//	std::cout << k << std::endl;
//
//	std::cout << Stats::statsToString(itemsFromJson.at(1).accessStats());
//
//	Monster monster;
//	monster.setItemLimit(5);
//	monster.setName("monster");
//	monster.setStats(items[1].getStats());
//	monster.setTextureFileName("textures/ufetubus.png");
//	monster.addItem(items[1]);
//
//	json z = monster;
//	std::cout << z << std::endl;
//
//	std::vector<json> jsonArray;
//
//	jsonArray.push_back(j);
//	jsonArray.push_back(k);
//	jsonArray.push_back(z);
//
//	std::cout << jsonArray << std::endl;
//
//	std::ofstream file;
//	file.open("jsonArray", std::ofstream::out);
//	file << jsonArray;
//	file.close();
//
//
//	std::ifstream file2;
//	file2.open("jsonArray", std::ifstream::in);
//	std::vector<json> jsonArrayFromFile = json::parse(file2);
//	file2.close();
//
//	std::cout << "\n\njsonArrayFromFile:\n\n" << jsonArrayFromFile << std::endl;
//
//	/*Stats stats;
//	stats.addStat("attack", 1);
//
//	json j = stats;
//	std::cout << j;*/
//
//	//delete items;
//
//	return 0;
//}
