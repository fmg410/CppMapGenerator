#include "Directions.h"

const sf::Vector2i Directions::UP = sf::Vector2i(0, -1);
const sf::Vector2i Directions::DOWN = sf::Vector2i(0, 1);
const sf::Vector2i Directions::RIGHT = sf::Vector2i(1, 0);
const sf::Vector2i Directions::LEFT = sf::Vector2i(-1, 0);
const sf::Vector2i Directions::NONE = sf::Vector2i(0, 0);

sf::Vector2i Directions::randomDirection()
{
	switch (rand() % 4)
	{
	case 0:
		return UP;
	case 1:
		return DOWN;
	case 2:
		return LEFT;
	case 3:
		return RIGHT;
	}
	return sf::Vector2i(0, 0);
}
