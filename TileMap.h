#pragma once
#include <vector>
#include "Tile.h"
#include "Wall.h"
#include "Parsable.h"




class TileMap : public Parsable
{
private:
	std::vector<std::vector<Tile>> m_tiles;
	Tile* m_lastAddedTile;
	sf::Vector2i m_lastTileAddedPos;

	

	void pv_parseStep(std::vector<std::string>)override;
public:

	static constexpr unsigned int tileSize = 64;

	TileMap();
	void addTile(const Tile&, sf::Vector2i);
	std::vector<Wall> getWallRep();
	void addAllSprites();
	~TileMap();
};

inline Tile makeSquareTile() {
	Tile tempTile;
	Wall tempWall;
	tempWall.wall.first = sf::Vector2f(0, TileMap::tileSize);
	tempWall.wall.second = sf::Vector2f(0, 0);
	tempTile.addBound(tempWall);
	tempWall.wall.first = sf::Vector2f(TileMap::tileSize, TileMap::tileSize);
	tempWall.wall.second = sf::Vector2f(0, TileMap::tileSize);
	tempTile.addBound(tempWall);
	tempWall.wall.first = sf::Vector2f(TileMap::tileSize, 0);
	tempWall.wall.second = sf::Vector2f(TileMap::tileSize, TileMap::tileSize);
	tempTile.addBound(tempWall);
	tempWall.wall.first = sf::Vector2f(0, 0);
	tempWall.wall.second = sf::Vector2f(TileMap::tileSize, 0);
	tempTile.addBound(tempWall);
	return tempTile;
}

 