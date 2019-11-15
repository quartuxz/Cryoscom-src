  #include "Tile.h"
#include "Animator.h"


void Tile::createFrom(const decomposedData& DData)
{
	decomposedData tempDData = DData;
	for (size_t i = 0; i < tempDData.childrenObjects.size(); i++)
	{
		if (tempDData.childrenObjects[i].type == "AnimatorSprite") {
			
			AnimatorSprite tempAnimatorSprite;
			tempAnimatorSprite.createFrom(tempDData.childrenObjects[i]);
			decomposedData *fitTileTypeDData = tempDData.childrenObjects[i].getChildByName("fitTileType");
			if(fitTileTypeDData != nullptr){
				if (fitTileTypeDData->data[0] == "fitTileX") {
					tempAnimatorSprite.scaleToMatch(fitX, Tile::tileSize);
				}
				else if (fitTileTypeDData->data[0] == "fitTileY") {
					tempAnimatorSprite.scaleToMatch(fitY, Tile::tileSize);
				}
				else if (fitTileTypeDData->data[0] == "fitTileMin") {
					tempAnimatorSprite.scaleToMatch(fitMin, Tile::tileSize);
				}
				else if (fitTileTypeDData->data[0] == "fitTileDistorted") {
					tempAnimatorSprite.scaleToMatch(fitDistorted, Tile::tileSize);
				}

			}
			decomposedData* relativePositionToTileX = tempDData.childrenObjects[i].getChildByName("relativePositionToTileX");
			if (relativePositionToTileX != nullptr) {
				tempAnimatorSprite.position.x += ma_deserialize<float>(relativePositionToTileX->data[0])*Tile::tileSize;

			}
			decomposedData* relativePositionToTileY = tempDData.childrenObjects[i].getChildByName("relativePositionToTileY");
			if (relativePositionToTileY != nullptr) {
				tempAnimatorSprite.position.y += ma_deserialize<float>(relativePositionToTileY->data[0]) * Tile::tileSize;
			}

			addTileSprite(tempAnimatorSprite);
				
		}
		else if (tempDData.childrenObjects[i].type == "Wall") {
			Wall tempWall;
			tempWall.createFrom(tempDData.childrenObjects[i]);
			addBound(tempWall);
		}
		else if (tempDData.childrenObjects[i].name == "WallConfig") {
			if (tempDData.childrenObjects[i].data[0] == "square") {
				Tile tempTile;
				tempTile = makeSquareTile();
				std::vector<Wall> tempBounds = tempTile.getBounds();
				for (size_t o = 0; o < tempBounds.size(); o++)
				{
					addBound(tempBounds[o]);
				}

			}
		}
			
	}
	
}

//TODO: make tiles serializable for level offloading
decomposedData Tile::serialize()
{
	return decomposedData();
}

Tile::Tile()
{
}

void Tile::addTileSprite(const AnimatorSprite &aSprite)
{
	m_hasSprite = true;
	m_tileSprites.push_back(aSprite);
}

void Tile::addSprites()
{
	for (size_t i = 0; i < m_tileSprites.size(); i++)
	{
		Animator::getInstance().addOneFrameSprite(m_tileSprites[i]);
	}
	
}

bool Tile::isEmpty() const
{
	return m_isEmpty;
}

bool Tile::hasSprite() const
{
	return m_hasSprite;
}

std::vector<Wall> Tile::getBounds() const
{
	return m_bounds;
}

void Tile::setBounds(const std::vector<Wall>&bounds)
{
	if (!bounds.empty()) {
		m_isEmpty = false;
	}
	m_bounds = bounds;
}

void Tile::addBound(const Wall&bound)
{
	m_isEmpty = false;
	m_bounds.push_back(bound);
}

void Tile::resetBoundsPos()
{
	moveBounds(-m_lastMove);
}

void Tile::resetSpritesPos()
{
	moveSprites(-m_lastMove);
}

void Tile::moveBounds(sf::Vector2f offset)
{
	for (size_t i = 0; i < m_bounds.size(); i++)
	{
		m_bounds[i].inside += offset;
		m_bounds[i].wall.first += offset;
		m_bounds[i].wall.second += offset;
	}
	m_lastMove += offset;
}

void Tile::moveSprites(sf::Vector2f offset)
{
	for (size_t i = 0; i < m_tileSprites.size(); i++)
	{
		m_tileSprites[i].position += offset;
	}	
}

std::vector<AnimatorSprite> Tile::getTileSprites() const
{
	return m_tileSprites;
}

void Tile::setTileSprites(const std::vector<AnimatorSprite> &tileSprites)
{
	m_hasSprite = true;
	m_tileSprites = tileSprites;
}


Tile::~Tile()
{
}
