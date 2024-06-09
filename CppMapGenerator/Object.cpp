#include "Object.h"
#include "Monster.h"
#include "Chest.h"
#include "Item.h"

const sf::Vector2f Object::objectSize = sf::Vector2f(40.0f, 40.0f);

ObjectChild Object::getClassType() const
{
	return ObjectChild::OBJECT;
}

Object* Object::createChildObject(const ObjectChild child)
{
	switch (child)
	{
	case ObjectChild::NONE:
		return nullptr;
	case ObjectChild::ITEM:
		return new Item();
	case ObjectChild::CHEST:
		return new Chest();
	case ObjectChild::MONSTER:
		return new Monster();
	default:
		break;
	}
}

Object::Object()
{}