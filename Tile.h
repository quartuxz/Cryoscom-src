#pragma once
#include <vector>
#include "AnimatorSprite.h"
#include "Wall.h"
#include "Serializable.h"

class Tile : public Serializable
{
private:
	std::vector<AnimatorSprite> m_tileSprites;
	std::vector<Wall> m_bounds;
	bool m_isEmpty = true;
	bool m_hasSprite = false;

	sf::Vector2f m_lastMove = sf::Vector2f(0,0);

public:

	void createFrom(const decomposedData&)override;
	decomposedData serialize()override;

	static constexpr unsigned int tileSize = 64;

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

	void resetBoundsPos();
	void resetSpritesPos();

	void moveBounds(sf::Vector2f);
	void moveSprites(sf::Vector2f);

	//gets all tileSprites(textures) for this tile
	std::vector<AnimatorSprite> getTileSprites()const;
	//sets the tiles sprites
	void setTileSprites(const std::vector<AnimatorSprite>&);
	~Tile();
};

inline Tile makeSquareTile() {
	Tile tempTile;
	Wall tempWall;
	tempWall.wall.first = sf::Vector2f(0, Tile::tileSize);
	tempWall.wall.second = sf::Vector2f(0, 0);
	tempTile.addBound(tempWall);
	tempWall.wall.first = sf::Vector2f(Tile::tileSize, Tile::tileSize);
	tempWall.wall.second = sf::Vector2f(0, Tile::tileSize);
	tempTile.addBound(tempWall);
	tempWall.wall.first = sf::Vector2f(Tile::tileSize, 0);
	tempWall.wall.second = sf::Vector2f(Tile::tileSize, Tile::tileSize);
	tempTile.addBound(tempWall);
	tempWall.wall.first = sf::Vector2f(0, 0);
	tempWall.wall.second = sf::Vector2f(Tile::tileSize, 0);
	tempTile.addBound(tempWall);
	return tempTile;
}

