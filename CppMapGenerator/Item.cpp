#include "Item.h"

IdContainer<Item> Item::itemTypes;

Item::Item()
{}

ObjectChild Item::getClassType() const
{
	return ObjectChild::ITEM;
}

bool Item::operator>(const Item& item)
{
	return this->entityName > item.entityName;
}

bool Item::operator<(const Item& item)
{
	return !(*this > item || *this == item);
}

bool Item::operator>=(const Item& item)
{
	return *this > item || *this == item;
}

bool Item::operator<=(const Item& item)
{
	return *this < item || *this == item;
}

bool Item::operator==(const Item& item)
{
	return this->entityName == item.entityName;
}