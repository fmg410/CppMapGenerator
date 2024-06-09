#pragma once
#include "JsonConfig.h"
#include "Item.h"
#include "Monster.h"
#include "Tile.h"
#include "MyMap.h"

static void to_json(json& j, const Object* object)
{
	switch (object->getClassType())
	{
	case ObjectChild::ITEM:
		j = *dynamic_cast<Item*>(const_cast<Object*>(object));
		break;
	case ObjectChild::CHEST:
		j = *dynamic_cast<Chest*>(const_cast<Object*>(object));
		break;
	case ObjectChild::MONSTER:
		j = *dynamic_cast<Monster*>(const_cast<Object*>(object));
		break;
	default:
		break;
	}
}

static void from_json(const json& j, const Object* object)
{
	Item item;
	Chest chest;
	Monster monster;
	switch (object->getClassType())
	{
	case ObjectChild::ITEM:
		item = j;
		*dynamic_cast<Item*>(const_cast<Object*>(object)) = item;
		break;
	case ObjectChild::CHEST:
		chest = j;
		*dynamic_cast<Chest*>(const_cast<Object*>(object)) = chest;
		break;
	case ObjectChild::MONSTER:
		monster = j;
		*dynamic_cast<Monster*>(const_cast<Object*>(object)) = monster;
		break;
	default:
		break;
	}
}

static void to_json(json& j, const MyMap& map)
{
	/*j["dimensionX"] = const_cast<MyMap*>(map)->getDimensions().x;
	j["dimensionY"] = const_cast<MyMap*>(map)->getDimensions().y;*/
	j["terrain"] = const_cast<MyMap&>(map).getTerrain();
}

static void from_json(const json& j, const MyMap& map)
{
	/*const_cast<MyMap*>(map)->setDimensions(sf::Vector2i(j.at("dimensionX"), j.at("dimensionY")));*/
	const_cast<MyMap&>(map).setTerrain(j.at("terrain"));
}