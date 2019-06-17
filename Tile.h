#pragma once
#include <vector>
#include "AnimatorSprite.h"
#include "Wall.h"

class Tile
{
private:
	AnimatorSprite m_tileSprite;
	std::vector<Wall> m_bounds;
	bool m_isEmpty = true;
	bool m_hasSprite = false;
public:
	Tile();
	void addSprite();
	bool isEmpty()const;
	bool hasSprite()const;
	std::vector<Wall> getBounds()const;
	void setBounds(const std::vector<Wall>&);
	void addBound(const Wall&);
	AnimatorSprite getTileSprite()const;
	void setTileSprite(AnimatorSprite);
	~Tile();
};

