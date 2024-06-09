#pragma once
#include "SFML/Graphics.hpp"

class Directions
{
private:
	Directions() {}

public:
	static const sf::Vector2i UP;
	static const sf::Vector2i DOWN;
	static const sf::Vector2i RIGHT;
	static const sf::Vector2i LEFT;
	static const sf::Vector2i NONE;

	static sf::Vector2i randomDirection();
};

