#include "LevelGenerator.h"
#include <random>

void LevelGenerator::addTileEntry(Tile tile, tileTypes tileType)
{
	m_tilePool[tileType].push_back(tile);
}

void LevelGenerator::setParams(const LevelGeneratorParams &params)
{
	m_params = params;
}

LevelGeneratorParams LevelGenerator::getParams() const
{
	return m_params;
}

UnitManager LevelGenerator::generateTileMap()
{
	std::default_random_engine generator;
	std::uniform_int_distribution<int> roomDist(m_params.minRooms, m_params.maxRooms);
	int roomNumber = roomDist(generator);

	TileMap tempTileMap;
	UnitManager level;


	std::uniform_int_distribution<int> roomSizeXDist(m_params.minRoomSize.x, m_params.maxRoomSize.x);
	std::uniform_int_distribution<int> roomSizeYDist(m_params.minRoomSize.y, m_params.maxRoomSize.y);

	std::uniform_int_distribution<int> roomSeparationXDist(m_params.maxRoomSeparation.x, m_params.minRoomSeparation.x);
	std::uniform_int_distribution<int> roomSeparationYDist(m_params.maxRoomSeparation.y, m_params.minRoomSeparation.y);

	sf::Vector2i lastEndPoint = sf::Vector2i(0,0);

	for (size_t i = 0; i < roomNumber; i++)
	{
		
		sf::Vector2i roomSize = sf::Vector2i(roomSizeXDist(generator), roomSizeYDist(generator));;
		for (size_t o = lastEndPoint.x; o < roomSize.x; o++)
		{
			for (size_t p = lastEndPoint.y; p < roomSize.y; p++)
			{
				if (o == 0 || o == (roomSize.x -1) || p == 0 || p == (roomSize.y-1)) {
					std::uniform_int_distribution<int> tileChosenDist(0, m_tilePool[wallTile].size());
					tempTileMap.addTile(m_tilePool[wallTile][tileChosenDist(generator)], sf::Vector2i(o, p));
				}
				else {
					std::uniform_int_distribution<int> tileChosenDist(0, m_tilePool[floorTile].size());
					tempTileMap.addTile(m_tilePool[floorTile][tileChosenDist(generator)], sf::Vector2i(o, p));
				}
				
				lastEndPoint = sf::Vector2i(o, p);
			}

		}
		lastEndPoint.x += roomSeparationXDist(generator);
		lastEndPoint.y += roomSeparationYDist(generator);
		
	}
	

	return UnitManager();
}
