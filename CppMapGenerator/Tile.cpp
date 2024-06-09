#include "Tile.h"

const sf::Vector2f Tile::tileSize = sf::Vector2f(50.0f, 50.0f);
IdContainer<Tile> Tile::tileTypes;

Tile::Tile()
	: isCrossable(false)
{}

Tile::Tile(const Tile& tile)
	: isCrossable(tile.isCrossable)
	, placedObject(nullptr)
{
	textureFileName = tile.textureFileName;
	texturePtr = tile.texturePtr;
}

Tile& Tile::operator=(const Tile& tile)
{
	textureFileName = tile.textureFileName;
	isCrossable = tile.isCrossable;
	placedObject = nullptr;
	texturePtr = tile.texturePtr;
	return *this;
}

Tile::~Tile()
{}

sf::Texture Tile::generateTexture()
{
	sf::RenderTexture render;

	if (!render.create(Tile::tileSize.x, Tile::tileSize.y))
	{
		return *getTexturePtr();
	}

	sf::RectangleShape tile(Tile::tileSize);
	tile.setTexture(getTexturePtr());
	render.draw(tile);

	if (placedObject.get() != nullptr)
	{
		sf::RectangleShape object(Object::objectSize);
		object.setOrigin(Object::objectSize / 2.0f);
		object.setTexture(placedObject.get()->getTexturePtr());
		object.setPosition(Tile::tileSize / 2.0f);
		render.draw(object);
	}


	render.display();

	return render.getTexture();
}