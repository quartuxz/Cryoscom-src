#pragma once
#include "Tile.h"
#include "TileMap.h"
#include "unitManager.h"

enum tileTypes {
	floorTile, horizontalWallTile, verticalWallTile
};

tileTypes stringToTileTypes(std::string);

struct levelGeneratorParams {
	sf::Vector2u minRoomSize = sf::Vector2u(3,3);
	sf::Vector2u maxRoomSize = sf::Vector2u(15,15);

	sf::Vector2i minRoomSeparation = sf::Vector2i(-7,-7);
	sf::Vector2i maxRoomSeparation= sf::Vector2i(-3,-3);

	unsigned int minRooms = 9;
	unsigned int maxRooms = 10;
};

struct enemySpawnParams : public Serializable {
	unsigned int spawnChancePerTile;
	unsigned int maxSpawnChancePerTile;
	std::vector<std::string> enemyFileNames;
	unsigned int enemies;
	void createFrom(const decomposedData&)override;
	decomposedData serialize()override;
};

struct levelRoom {
	sf::Vector2i roomStart;
	sf::Vector2i roomEnd;
};

class LevelGenerator : public Parsable
{
private:
	std::map<tileTypes, std::vector<Tile> > m_tilePool;
	levelGeneratorParams m_params;
	std::vector<enemySpawnParams> m_enemySpawnData;
	void pv_parseStep(std::vector<std::string>)override;
	std::vector<levelRoom> m_rooms;
	std::vector<sf::Vector2i> m_floorTiles;
	void m_addRoom(const levelRoom&);
public:
	void addEnemySpawnParam(const enemySpawnParams&);
	std::vector<enemySpawnParams> getEnemySpawnParams()const;
	void addTileEntry(const Tile&, tileTypes);      
	void setParams(const levelGeneratorParams&);
	levelGeneratorParams getParams()const;
	//creates a dynamically allocated level
	UnitManager *generateLevel();
};

