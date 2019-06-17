#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <math.h>
#include "Wall.h"
#include "unit.h"

float minimum_distance(sf::Vector2f v, sf::Vector2f w, sf::Vector2f p, sf::Vector2f *proj);



class Map
{
private:
	std::vector<Wall> walls;
	std::map<std::string, size_t> m_wallNames;
public:
	Map();
	void update(std::vector<unit*>);
	void update(unit*);

	bool collides(unit*);

	void addNamedWall(sf::Vector2f, sf::Vector2f, std::string, bool clockWiseFromFirst = true);

	void addWall(sf::Vector2f, sf::Vector2f, bool clockWiseFromFirst = true);

	std::vector<Wall> getWallsConst()const;
	std::vector<Wall> *getWalls();
	std::map<std::string, size_t> getWallNames()const;

	~Map();
};

