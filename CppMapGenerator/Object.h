#pragma once
#include "Textured.h"
#include "JsonConfig.h"

enum class ObjectChild{
	NONE,
	OBJECT,
	ITEM,
	CHEST,
	MONSTER
};

class Object : public Textured
{

public:

	static const sf::Vector2f objectSize;

	virtual ObjectChild getClassType() const;

	static Object* createChildObject(const ObjectChild child);
	Object();
	virtual ~Object() {}

	/*void setOnlyOnCrossable(bool _onlyOnCrossable) { onlyOnCrossable = _onlyOnCrossable; }
	bool getOnlyOnCrossable() { return onlyOnCrossable; }*/

protected:

	//bool onlyOnCrossable;

	/*virtual void draw() = 0;
	virtual void interact() = 0;*/

};