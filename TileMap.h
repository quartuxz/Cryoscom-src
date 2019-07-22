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

	static constexpr unsigned int m_tileSize = 64;

	void pv_parseStep(std::vector<std::string>)override;
public:
	TileMap();
	void addTile(const Tile&, sf::Vector2i);
	std::vector<Wall> getWallRep();
	void addAllSprites();
	~TileMap();
};

 