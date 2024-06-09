#pragma once
#include <iostream>
#include "Object.h"
#include "Stats.h"

class Entity : virtual public Object
{
protected:
	std::string entityName;
	Stats stats;

	Entity();
	virtual ~Entity() {}

public:
	
	std::string getName() { return entityName; }
	void setName(std::string _entityName) { entityName = _entityName; }

	void setStats(const Stats& _stats) { stats = _stats; }
	Stats getStats() { return stats; }
	Stats& accessStats() { return stats; }

};

