#pragma once
#include "Tile.h"
#include "TileMap.h"
#include "unitManager.h"

enum tileTypes {
	floorTile, wallTile
};

struct LevelGeneratorParams {
	sf::Vector2i minRoomSize;
	sf::Vector2i maxRoomSize;

	sf::Vector2i minRoomSeparation;
	sf::Vector2i maxRoomSeparation;

	int minRooms;
	int maxRooms;
};

class LevelGenerator
{
private:
	std::map<tileTypes, std::vector<Tile> > m_tilePool;
	LevelGeneratorParams m_params;
public:
	void addTileEntry(Tile, tileTypes);
	void setParams(const LevelGeneratorParams&);
	LevelGeneratorParams getParams()const;
	UnitManager generateTileMap();
};

