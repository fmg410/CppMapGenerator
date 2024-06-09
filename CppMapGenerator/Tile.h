#pragma once
#include <map>
#include <memory>
#include "JsonConfig.h"
#include "SFML/Graphics.hpp"
#include "Textured.h"
#include "Object.h"
#include "Item.h"
#include "Chest.h"
#include "Monster.h"
#include "IdContainer.h"

class Tile : public Textured
{
private:
	bool isCrossable;
	std::unique_ptr<Object> placedObject;

public:
	static const sf::Vector2f tileSize;

public:
	static IdContainer<Tile> tileTypes;

	Tile();
	Tile(const Tile& tile);
	Tile& operator=(const Tile& tile);
	~Tile();

	sf::Texture generateTexture();

	void setIsCrossable(bool _isCrossable) { isCrossable = _isCrossable; }
	bool getIsCrossable() { return isCrossable; }

	void setTypeOfPlacedObject(const ObjectChild type) { placedObject.reset(Object::createChildObject(type)); }
	void setPlacedObject(Object* _placedObject) { placedObject.reset(_placedObject); }
	Object* getPlacedObject() { return placedObject.get(); }

};

static void to_json(json& j, const Tile& tile) {

	ObjectChild object = ObjectChild::NONE;
	j["isCrossable"] = const_cast<Tile&>(tile).getIsCrossable();
	if (const_cast<Tile&>(tile).getPlacedObject() != nullptr)
	{
		object = const_cast<Tile&>(tile).getPlacedObject()->getClassType();
	}
	j["placedObjectType"] = object;
	switch (object)
	{
	case ObjectChild::NONE:
		j["placedObject"] = nullptr;
		break;
	case ObjectChild::ITEM:
		j["placedObject"] = *dynamic_cast<Item*>(const_cast<Tile&>(tile).getPlacedObject());
		break;
	case ObjectChild::CHEST:
		j["placedObject"] = *dynamic_cast<Chest*>(const_cast<Tile&>(tile).getPlacedObject());
		break;
	case ObjectChild::MONSTER:
		j["placedObject"] = *dynamic_cast<Monster*>(const_cast<Tile&>(tile).getPlacedObject());
		break;
	default:
		break;
	}

}

static void from_json(const json& j, Tile& tile) {
	tile.setIsCrossable(j.at("isCrossable"));

	ObjectChild child = j.at("placedObjectType");

	Item item;
	Monster monster;
	Chest chest;

	switch (child)
	{
	case ObjectChild::NONE:
		tile.setPlacedObject(nullptr);
		break;
	case ObjectChild::ITEM:
		tile.setPlacedObject(new Item(item = j.at("placedObject")));
		break;
	case ObjectChild::CHEST:
		tile.setPlacedObject(new Chest(chest = j.at("placedObject")));
		break;
	case ObjectChild::MONSTER:
		tile.setPlacedObject(new Monster(monster = j.at("placedObject")));
		break;
	default:
		break;
	}

}