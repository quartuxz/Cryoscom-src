  #include "Tile.h"
#include "Animator.h"


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
