#pragma once
#include <vector>
#include "AnimatorSprite.h"
#include "Wall.h"

class Tile
{
private:
	std::vector<AnimatorSprite> m_tileSprites;
	std::vector<Wall> m_bounds;
	bool m_isEmpty = true;
	bool m_hasSprite = false;
public:
	Tile();
	//adds the tile sprites to the queue for drawing
	void addSprites();
	//adds a new texture for the tile
	void addTileSprite(const AnimatorSprite&);

	bool isEmpty()const;
	bool hasSprite()const;
	std::vector<Wall> getBounds()const;
	void setBounds(const std::vector<Wall>&);
	void addBound(const Wall&);

	void moveBounds(sf::Vector2f);
	void moveSprites(sf::Vector2f);

	//gets all tileSprites(textures) for this tile
	std::vector<AnimatorSprite> getTileSprites()const;
	//sets the tiles sprites
	void setTileSprites(const std::vector<AnimatorSprite>&);
	~Tile();
};

