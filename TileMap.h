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

	TileMap();

	void setTile(const Tile&, sf::Vector2i);
	std::vector<Wall> getWallRep()const;
	std::vector<std::vector<Tile>> *getAllTiles();
	void addAllSprites();
	~TileMap();
};



 