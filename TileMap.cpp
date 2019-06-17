#include "TileMap.h"
#include "Serializable.h"
#include "Animator.h"

void TileMap::pv_parseStep(std::vector<std::string> line)
{
	if (line[0] == "tile") {
		Tile tempTile;
		sf::Vector2i tilePos = sf::Vector2i(ma_deserialize_int(line[1]), ma_deserialize_int(line[1]));
		if (m_tiles.size() <= tilePos.x) {
			m_tiles.resize(tilePos.x+1);
		}
		if (m_tiles[tilePos.x].size() <= tilePos.y) {
			m_tiles[tilePos.x].resize(tilePos.y+1);
		}

		m_lastAddedTile = &m_tiles[tilePos.x][tilePos.y];
	}
	else if (line[0] == "tileTex"){

	}
}

TileMap::TileMap()
{
}

void TileMap::addTile(const Tile& tile, sf::Vector2i pos)
{
	
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
				m_tiles[i][o].addSprite();
			}
		}
	}
}

TileMap::~TileMap()
{
}
