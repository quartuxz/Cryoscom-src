#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <math.h>
#include "unit.h"

float minimum_distance(sf::Vector2f v, sf::Vector2f w, sf::Vector2f p, sf::Vector2f *proj);

sf::Vector2f reflect(sf::Vector2f v, sf::Vector2f w, sf::Vector2f p);
//first param is the origin of rotation and the second param is the point to be rotated
sf::Vector2f rotate90(sf::Vector2f, sf::Vector2f);

struct Wall {
	std::pair<sf::Vector2f, sf::Vector2f> wall;
	sf::Vector2f inside;
	sf::Vector2f getOutside();
	bool isActive = true;
};

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

