#include "Monster.h"

IdContainer<Monster> Monster::monsterTypes;

Monster::Monster()
{}

ObjectChild Monster::getClassType() const
{
	return ObjectChild::MONSTER;
}