#pragma once
#include <list>
#include "Object.h"
#include "Item.h"

class Chest : virtual public Object
{
protected:
	int itemLimit;
	std::list<Item> contents;

public:

	virtual ObjectChild getClassType() const override;

	void setItemLimit(int _itemLimit) { itemLimit = _itemLimit; }
	int getItemLimit() { return itemLimit; }

	int getItemCount() { return contents.size(); }

	Item& accessItem(int index);
	void addItem(Item& item);
	void removeItem(int index);
	auto getIterator() { return contents.begin(); }
	auto getEnd() { return contents.end(); }

	Chest();
	virtual ~Chest() {}

};

static void to_json(json& j, const Chest& chest)
{
	j["itemLimit"] = const_cast<Chest&>(chest).getItemLimit();

	std::vector<Item> contents;

	for (auto itr = const_cast<Chest&>(chest).getIterator(); itr != const_cast<Chest&>(chest).getEnd(); ++itr)
		contents.push_back(*itr);

	j["contents"] = contents;
}

static void from_json(const json& j, const Chest& chest)
{
	const_cast<Chest&>(chest).setItemLimit(j.at("itemLimit"));
	
	std::vector<Item> contents = j.at("contents");

	for (Item& item : contents)
		const_cast<Chest&>(chest).addItem(item);
}