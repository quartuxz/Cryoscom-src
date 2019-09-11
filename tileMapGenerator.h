#pragma once
#include "Tile.h"
#include "TileMap.h"

enum tileTypes {
	floorTile, wallTile
};

class tileMapGenerator
{
public:
	void addTileEntry(Tile, tileTypes);
	TileMap generateTileMap();
};

