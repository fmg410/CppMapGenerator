#pragma once
#include <iostream>
#include <memory>
#include "SFML/Graphics.hpp"

class Textured
{
protected:
	std::string textureFileName;
	std::shared_ptr<sf::Texture> texturePtr;

public:
	Textured();
	~Textured(); // added texturePtr.reset(), not sure how removing works with static IdContainer

	std::string getTextureFileName() { return textureFileName; }
	sf::Texture* getTexturePtr() { return texturePtr.get(); }

	void setTextureFileName(const std::string& _textureFileName);
};

