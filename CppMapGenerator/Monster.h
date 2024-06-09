#pragma once
#include "Entity.h"
#include "Chest.h"
#include "IdContainer.h"

class Monster : public Entity, public Chest
{

public:
	static IdContainer<Monster> monsterTypes;

	virtual ObjectChild getClassType() const override;

	Monster();
	virtual ~Monster() {}

};

static void to_json(json& j, const Monster& monster)
{
	j["name"] = const_cast<Monster&>(monster).getName();
	j["stats"] = const_cast<Monster&>(monster).getStats();

	j["itemLimit"] = const_cast<Monster&>(monster).getItemLimit();

	std::vector<Item> contents;

	for (auto itr = const_cast<Monster&>(monster).getIterator(); itr != const_cast<Monster&>(monster).getEnd(); ++itr)
		contents.push_back(*itr);

	j["contents"] = contents;

}

static void from_json(const json& j, const Monster& monster)
{
	const_cast<Monster&>(monster).setName(j.at("name"));
	const_cast<Monster&>(monster).setStats(j.at("stats"));

	const_cast<Monster&>(monster).setItemLimit(j.at("itemLimit"));

	std::vector<Item> contents = j.at("contents");

	for (Item& item : contents)
		const_cast<Monster&>(monster).addItem(item);

}