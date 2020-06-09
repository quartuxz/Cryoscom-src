#include "LevelGenerator.h"
#include <random>
#include "Animator.h"

void LevelGenerator::pv_parseStep(std::vector<std::string> tokens)
{
	if (tokens[0] == "addTileEntry") {
		Tile tempTile;
		tempTile.createFrom(decomposedData().createFrom(tokens[2]));
		addTileEntry(tempTile, stringToTileTypes(tokens[1]));
	}
	else if (tokens[0] == "enemySpawnParam") {
		enemySpawnParams tempEnemySpawnParams;
		tempEnemySpawnParams.createFrom(decomposedData().createFrom(tokens[1]));
		addEnemySpawnParam(tempEnemySpawnParams);
	}
}

void LevelGenerator::m_addRoom(const levelRoom &levelRoom)
{
	m_rooms.push_back(levelRoom); 
}

void LevelGenerator::addEnemySpawnParam(const enemySpawnParams &enemySpawnParam)
{
	m_enemySpawnData.push_back(enemySpawnParam);
}

std::vector<enemySpawnParams> LevelGenerator::getEnemySpawnParams() const
{
	return m_enemySpawnData;
}

void LevelGenerator::addTileEntry(const Tile& tile, tileTypes tileType)
{
	m_tilePool[tileType].push_back(tile);
}

void LevelGenerator::setParams(const levelGeneratorParams &params)
{
	m_params = params;
}

levelGeneratorParams LevelGenerator::getParams() const
{
	return m_params;
}

//TODO: add randomized tile picking for floor and wall tiles
UnitManager *LevelGenerator::generateLevel()
{
	int roomNumber = generateRandomNumber(m_params.minRooms, m_params.maxRooms);



	TileMap tempTileMap;
	UnitManager *level = new UnitManager();


	level->setMap(new Map());


	sf::Vector2i lastEndPoint = sf::Vector2i(0,0);

	for (size_t i = 0; i < roomNumber; i++)
	{
		unsigned int roomSizeX = generateRandomNumber(m_params.minRoomSize.x, m_params.maxRoomSize.x);
		unsigned int roomSizeY = generateRandomNumber(m_params.minRoomSize.y, m_params.maxRoomSize.y);
		for (size_t o = lastEndPoint.x; o < (roomSizeX + lastEndPoint.x); o++)
		{
			for (size_t p = lastEndPoint.y; p < (roomSizeY + lastEndPoint.y); p++)
			{
				if (p==lastEndPoint.y||p==(lastEndPoint.y+roomSizeY-1)) {
					tempTileMap.setTile(m_tilePool[horizontalWallTile][0], sf::Vector2i(o,p));
				}
				else if (o==lastEndPoint.x||o==(lastEndPoint.x+roomSizeX-1)) {
					tempTileMap.setTile(m_tilePool[verticalWallTile][0], sf::Vector2i(o, p));
				}
			}
		}
		levelRoom tempRoom;
		tempRoom.roomStart = lastEndPoint;
		tempRoom.roomEnd = lastEndPoint + sf::Vector2i(roomSizeX, roomSizeY);
		m_addRoom(tempRoom);
		lastEndPoint = sf::Vector2i(roomSizeX+lastEndPoint.x, roomSizeY+lastEndPoint.y)+sf::Vector2i(generateRandomNumber(m_params.minRoomSeparation.x, m_params.maxRoomSeparation.x), generateRandomNumber(m_params.minRoomSeparation.y, m_params.maxRoomSeparation.y));

		
	}

	for (size_t i = 0; i < m_rooms.size(); i++)
	{
		for (size_t o = m_rooms[i].roomStart.x; o < m_rooms[i].roomEnd.x; o++)
		{
			for (size_t p = m_rooms[i].roomStart.y;p < m_rooms[i].roomEnd.y; p++)
			{
				if (o == m_rooms[i].roomStart.x || p == m_rooms[i].roomStart.y || o == (m_rooms[i].roomEnd.x -1) || p == (m_rooms[i].roomEnd.y-1)) {
					
				}
				else {
					m_floorTiles.push_back(sf::Vector2i(o,p));
					tempTileMap.setTile(m_tilePool[floorTile][0], sf::Vector2i(o, p));
				}
			}
		}
	}
	//adds the player at the first available floor tile
	level->setPlayer(makePlayer(sf::Vector2f(m_floorTiles[m_rooms[0].roomStart.y+m_rooms[0].roomEnd.y-1]*int(Tile::tileSize)), 20, 20));
	Weapon* tempWeapon = new Weapon(level->getPlayer());
	AnimatorSprite tempAnimatorSprite;
	tempAnimatorSprite.textureID = Animator::getInstance().getTextureID("bullet_1.png");
	tempWeapon->setbulletAnimatorTex(tempAnimatorSprite);
	level->setPlayerWeapon(tempWeapon);
	
	

	for (size_t i = 0; i < m_floorTiles.size(); i++)
	{
		for (size_t p = 0; p < m_enemySpawnData.size(); p++)
		{
			if (generateRandomNumber(0,m_enemySpawnData[p].maxSpawnChancePerTile) < m_enemySpawnData[p].spawnChancePerTile) {
				level->spawnEnemies(m_enemySpawnData[p].enemies, sf::Vector2f(m_floorTiles[i] * int(Tile::tileSize)), m_enemySpawnData[p].enemyFileNames);
			}
				
		}
	}

	level->setTileMap(tempTileMap);

	return level;
}

tileTypes stringToTileTypes(std::string tileTypeName)
{
	if (tileTypeName == "floorTile") {
		return floorTile;
	}
	else if (tileTypeName == "horizontalWallTile") {
		return horizontalWallTile;
	}
	else if (tileTypeName == "verticalWallTile") {
		return verticalWallTile;
	}
}

void enemySpawnParams::createFrom(const decomposedData &DData)
{
	decomposedData tempDData = DData;
	if (tempDData.getChildByName("spawnChancePerTile")) {
		spawnChancePerTile = ma_deserialize<unsigned int>(tempDData.getChildByName("spawnChancePerTile")->data[0]);
	}
	if (tempDData.getChildByName("maxSpawnChancePerTile")) {
		maxSpawnChancePerTile = ma_deserialize<unsigned int>(tempDData.getChildByName("maxSpawnChancePerTile")->data[0]);
	}
	if (tempDData.getChildByName("enemies")) {
		enemies = ma_deserialize<unsigned int>(tempDData.getChildByName("enemies")->data[0]);
	}


	for (size_t i = 0; i < tempDData.childrenObjects.size(); i++)
	{
		if (tempDData.childrenObjects[i].name == "enemyFileName") {
			enemyFileNames.push_back(tempDData.childrenObjects[i].data[0]);
		}
	}
	
}

decomposedData enemySpawnParams::serialize()
{
	return decomposedData();
}
