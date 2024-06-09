#include "Textured.h"

Textured::Textured()
{
	texturePtr = std::make_shared<sf::Texture>();
	setTextureFileName("");
}

Textured::~Textured()
{}

void Textured::setTextureFileName(const std::string& _textureFileName)
{
	textureFileName = _textureFileName;
	if (!texturePtr.get()->loadFromFile(textureFileName))
		texturePtr.get()->loadFromFile("textures/missingTexture.png");
}