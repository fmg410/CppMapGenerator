#include "Chest.h"

Chest::Chest()
	: itemLimit(0)
{
	setTextureFileName("textures/chest.png");
}

ObjectChild Chest::getClassType() const
{
	return ObjectChild::CHEST;
}

Item& Chest::accessItem(int index)
{
	std::list<Item>::iterator itr = contents.begin();

	for (int k = 0; itr != contents.end() && k < index; ++itr, ++k) {}

	return *itr;

}

void Chest::addItem(Item& item)
{
	if (contents.size() >= itemLimit)
		return;

	if (contents.empty())
	{
		contents.push_back(item);
		return;
	}

	for (std::list<Item>::iterator itr = contents.begin(); itr != contents.end(); ++itr)
	{
		if (item < *itr)
		{
			contents.insert(itr, item);
			return;
		}
	}

	contents.push_back(item);
}

void Chest::removeItem(int index)
{
	std::list<Item>::iterator itr = contents.begin();

	for (int k = 0; itr != contents.end() && k < index; ++itr, ++k) {}

	contents.erase(itr);
}