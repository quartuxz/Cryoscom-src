#include "TileMap.h"
#include "Serializable.h"
#include "Animator.h"

void TileMap::pv_parseStep(std::vector<std::string> line)
{
	if (line[0] == "tile") {
		Tile tempTile;
		sf::Vector2i tilePos = sf::Vector2i(ma_deserialize_int(line[2]), ma_deserialize_int(line[3]));
		if (line[1] == "square") {

			tempTile = makeSquareTile();

		}
		else if (line[1] == "empty") {

		}
		else if (line[1] == "copy") {
			tempTile = *m_lastAddedTile;
			sf::Vector2f offset = -sf::Vector2f(m_lastTileAddedPos.x * Tile::tileSize, m_lastTileAddedPos.y * Tile::tileSize);
			tempTile.moveBounds(offset);
			tempTile.moveSprites(offset);
		}
		else if (line[1] == "copyPaint") {
			tempTile = *m_lastAddedTile;
			sf::Vector2f offset = -sf::Vector2f(m_lastTileAddedPos.x * Tile::tileSize, m_lastTileAddedPos.y * Tile::tileSize);
			tempTile.moveBounds(offset);
			tempTile.moveSprites(offset);
			for (size_t i = tilePos.x; i < ma_deserialize_int(line[4]); i++)
			{
				for (size_t o = tilePos.y; o < ma_deserialize_int(line[5]); o++)
				{
					if (!(i == tilePos.x && o == tilePos.y)) {
						setTile(tempTile, sf::Vector2i(i,o));
					}
					
				}
			}
		}
		else {

			for (size_t i = 2; i < line.size(); i += 4)
			{
				Wall tempWall;
				tempWall.wall.first = sf::Vector2f(ma_deserialize_float(line[i]) * Tile::tileSize, ma_deserialize_float(line[i + 1])*Tile::tileSize);
				tempWall.wall.second = sf::Vector2f(ma_deserialize_float(line[i+ 2])*Tile::tileSize, ma_deserialize_float(line[i + 3])*Tile::tileSize);
				tempTile.addBound(tempWall);
			}
			
		}
		setTile(tempTile, tilePos);
		
	}
	else if (line[0] == "tileTex"){
		AnimatorSprite tempAnimatorSprite;
		tempAnimatorSprite.createFrom(decomposedData().createFrom(line[4]));
		tempAnimatorSprite.position += sf::Vector2f(m_lastTileAddedPos.x*Tile::tileSize, m_lastTileAddedPos.y*Tile::tileSize);
		tempAnimatorSprite.position.x += ma_deserialize_float(line[1])*Tile::tileSize;
		tempAnimatorSprite.position.y += ma_deserialize_float(line[2]) * Tile::tileSize;
		if (line[3] == "fitTileX") {
			tempAnimatorSprite.scaleToMatch(fitX, Tile::tileSize);
		}
		else if (line[3] == "fitTileY") {
			tempAnimatorSprite.scaleToMatch(fitY, Tile::tileSize);
		}
		else if (line[3] == "fitTileMin") {
			tempAnimatorSprite.scaleToMatch(fitMin, Tile::tileSize);
		}
		else if (line[3] == "fitTileDistorted") {
			tempAnimatorSprite.scaleToMatch(fitDistorted, Tile::tileSize);
		}
		m_lastAddedTile->addTileSprite(tempAnimatorSprite);
	}

}

TileMap::TileMap()
{
}

void TileMap::setTile(const Tile& tile, sf::Vector2i pos)
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
	sf::Vector2f offset = sf::Vector2f(pos.x * Tile::tileSize, pos.y * Tile::tileSize);
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
