#pragma once
#include <iostream>
#include "Entity.h"
#include "IdContainer.h"

class Item : public Entity
{

protected:
	std::string description;

public:
	static IdContainer<Item> itemTypes;

	virtual ObjectChild getClassType() const override;

	Item();
	virtual ~Item() {}

	std::string getDescription() { return description; }
	void setDescription(std::string _description) { description = _description; }


	bool operator>(const Item& item);
	bool operator<(const Item& item);
	bool operator>=(const Item& item);
	bool operator<=(const Item& item);
	bool operator==(const Item& item);

};

static void to_json(json& j, const Item& item)
{
	j["name"] = const_cast<Item&>(item).getName();
	j["description"] = const_cast<Item&>(item).getDescription();
	j["stats"] = const_cast<Item&>(item).getStats();
}

static void from_json(const json& j, const Item& item)
{
	const_cast<Item&>(item).setName(j.at("name"));
	const_cast<Item&>(item).setDescription(j.at("description"));
	const_cast<Item&>(item).setStats(j.at("stats"));
}