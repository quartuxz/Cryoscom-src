#include "TileMap.h"
#include "Serializable.h"
#include "Animator.h"

void TileMap::pv_parseStep(std::vector<std::string> line)
{
	if (line[0] == "tile") {
		Tile tempTile;
		sf::Vector2i tilePos = sf::Vector2i(ma_deserialize_int(line[2]), ma_deserialize_int(line[3]));
		if (line[1] == "square") {
			Wall tempWall;
			tempWall.wall.first = sf::Vector2f(0, m_tileSize);
			tempWall.wall.second = sf::Vector2f(0, 0);
			tempTile.addBound(tempWall);
			tempWall.wall.first = sf::Vector2f(m_tileSize, m_tileSize);
			tempWall.wall.second = sf::Vector2f(0, m_tileSize) ;
			tempTile.addBound(tempWall);
			tempWall.wall.first = sf::Vector2f(m_tileSize, 0);
			tempWall.wall.second = sf::Vector2f(m_tileSize, m_tileSize);
			tempTile.addBound(tempWall);
			tempWall.wall.first = sf::Vector2f(0, 0);
			tempWall.wall.second = sf::Vector2f(m_tileSize, 0);
			tempTile.addBound(tempWall);
		}
		else {

			for (size_t i = 2; i < line.size(); i += 4)
			{
				Wall tempWall;
				tempWall.wall.first = sf::Vector2f(ma_deserialize_float(line[i]) * m_tileSize, ma_deserialize_float(line[i + 1])*m_tileSize);
				tempWall.wall.second = sf::Vector2f(ma_deserialize_float(line[i+ 2])*m_tileSize, ma_deserialize_float(line[i + 3])*m_tileSize);
				tempTile.addBound(tempWall);
			}
			
		}
		addTile(tempTile, tilePos);
		
	}
	else if (line[0] == "tileTex"){
		AnimatorSprite tempAnimatorSprite;
		tempAnimatorSprite.createFrom(decomposedData().createFrom(line[1]));
		tempAnimatorSprite.position += sf::Vector2f(m_lastTileAddedPos.x*m_tileSize, m_lastTileAddedPos.y*m_tileSize);
		m_lastAddedTile->addTileSprite(tempAnimatorSprite);
	}
}

TileMap::TileMap()
{
}

void TileMap::addTile(const Tile& tile, sf::Vector2i pos)
{
	if (m_tiles.size() <= pos.x) {
		m_tiles.resize(pos.x + 1);
	}
	if (m_tiles[pos.x].size() <= pos.y) {
		m_tiles[pos.x].resize(pos.y + 1); 
	}

	m_tiles[pos.x][pos.y] = tile;

	m_lastAddedTile = &m_tiles[pos.x][pos.y];
	m_lastTileAddedPos = pos;
	sf::Vector2f offset = sf::Vector2f(pos.x * m_tileSize, pos.y * m_tileSize);
	m_lastAddedTile->moveBounds(offset);
	m_lastAddedTile->moveSprites(offset);
}

std::vector<Wall> TileMap::getWallRep()
{
	std::vector<Wall> retVal;
	for (size_t i = 0; i < m_tiles.size(); i++)
	{
		for (size_t o = 0; o < m_tiles[i].size(); o++)
		{
			if (!m_tiles[i][o].isEmpty()) {
				std::vector<Wall> tempTileBounds = m_tiles[i][o].getBounds();
				retVal.insert(retVal.end(), tempTileBounds.begin(), tempTileBounds.end());
			}
		}
	}
	return retVal;
}


void TileMap::addAllSprites()
{
	for (size_t i = 0; i < m_tiles.size(); i++)
	{
		for (size_t o = 0; o < m_tiles[i].size(); o++)
		{
			if (m_tiles[i][o].hasSprite()) {
				m_tiles[i][o].addSprites();
			}
		}
	}
}

TileMap::~TileMap()
{
}
