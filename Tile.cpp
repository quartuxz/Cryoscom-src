#include "Tile.h"
#include "Animator.h"


Tile::Tile()
{
}

void Tile::addSprite()
{
	Animator::getInstance().addOneFrameSprite(m_tileSprite);
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

AnimatorSprite Tile::getTileSprite() const
{
	return m_tileSprite;
}

void Tile::setTileSprite(AnimatorSprite tileSprite)
{
	m_hasSprite = true;
	m_tileSprite = tileSprite;
}


Tile::~Tile()
{
}
