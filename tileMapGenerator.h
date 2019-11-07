#pragma once
#include "Tile.h"
#include "TileMap.h"
#include "unitManager.h"

enum tileTypes {
	floorTile, wallTile
};

class LevelGenerator
{
private:
	std::map<tileTypes, std::vector<Tile> > m_tilePool;
public:
	void addTileEntry(Tile, tileTypes);
	unitManager generateTileMap();
};

