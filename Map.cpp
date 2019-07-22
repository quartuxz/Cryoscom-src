#include "Map.h"
#include <iostream>
#include "cryoscom_defsAndUtils.h"
#if CRYOSCOM_DEBUG || MAKING_LEVELS
	#include "Animator.h"
#endif // CRYOSCOM_DEBUG





Map::Map()
{
}

void Map::update(std::vector<unit*> units)
{
	for (size_t i = 0; i < units.size(); i++)
	{
		if (units[i]->isDead) {
			continue;
		}
		update(units[i]);
	}
}

void Map::update(unit *units)
{
	for (size_t o = 0; o < walls.size(); o++)
	{
		if (!walls[o].isActive) {
			continue;
		}
		#if CRYOSCOM_DEBUG || MAKING_LEVELS
			sf::VertexArray lines(sf::LinesStrip, 2);
			lines[0].position = sf::Vector2f(walls[o].wall.first.x, walls[o].wall.first.y);
			lines[0].color = sf::Color::Magenta;
			lines[1].position = sf::Vector2f(walls[o].wall.second.x, walls[o].wall.second.y);
			lines[1].color = sf::Color::Yellow;
			/*sf::RectangleShape inside;
			inside.setFillColor(sf::Color::Blue);
			inside.setSize(sf::Vector2f(10, 10));
			inside.setPosition(sf::Vector2f(walls[o].inside.x *1, walls[o].inside.y * 1));*/
			Animator::getInstance().getWindow()->draw(lines);
			if (units == nullptr) {
				continue;
			}
		#endif // CRYOSCOM_DEBUG

		auto body = units->getBody();
		for (size_t p = 0; p < body.size(); p++)
		{
			sf::Vector2f intersectMovePoint;
			sf::Vector2f proj;
			float dist = minimum_distance(walls[o].wall.first, walls[o].wall.second, body[p].first, &proj);
			//this if checks for collision with wall
			if (dist < body[p].second) {
				if (LineSegementsIntersect(units->lastPos, body[p].first, walls[o].wall.first, walls[o].wall.second, &intersectMovePoint)) {
					units->setPosition(units->lastLastPos);
					//units->stopMovement();
				}
				//this if checks if the object is on the correct side of the wall, else, it transports(deprecated after the comma) it to the correct side upon collsiion.
				if (distanceSquared(walls[o].inside, body[p].first) < distanceSquared(walls[o].getOutside(), body[p].first)) {

					float dist2 = sqrt(pow(body[p].first.x - proj.x, 2) + pow(body[p].first.y - proj.y, 2));
					units->move(sf::Vector2f(((body[p].first.x - proj.x) / dist2) * (body[p].second - dist), ((body[p].first.y - proj.y) / dist2) * (body[p].second - dist)));
					//units->hitsWall(sf::Vector2f(((body[p].first.x - proj.x) / dist2) * (body[p].second - dist), ((body[p].first.y - proj.y) / dist2) * (body[p].second - dist)));
				}
				
				//deprecated walls transportation
				//else {
				//	sf::Vector2f reflected = reflect(walls[o].wall.first, walls[o].wall.second, body[p].first);
				//	units->move(reflected - body[p].first);
				//	//units->hitsWall(reflected - body[p].first);
				//}
				
			}
		}
	}
}

bool Map::collides(unit *units)
{
	for (size_t o = 0; o < walls.size(); o++)
	{
		
		auto body = units->getBody();
		for (size_t p = 0; p < body.size(); p++)
		{
			sf::Vector2f proj;
			float dist = minimum_distance(walls[o].wall.first, walls[o].wall.second, body[p].first, &proj);
			if (dist < body[p].second) {
				return true;
			}
		}
	}
	return false;
}

void Map::addWall(sf::Vector2f first, sf::Vector2f second, bool clockWiseFromFirst)
{
	Wall wall;
	wall.wall = std::pair<sf::Vector2f, sf::Vector2f>(first, second);
	addWall(wall);
}

void Map::addWall(const Wall &wall)
{
	walls.push_back(wall);
	walls.back().calculateInside();;
}

void Map::addNamedWall(sf::Vector2f first, sf::Vector2f second, std::string name, bool clockWiseFromFirst) {
	addWall(first, second, clockWiseFromFirst);
	m_wallNames[name] = walls.size()-1;
}

std::map<std::string, size_t> Map::getWallNames()const {
	return m_wallNames;
}

std::vector<Wall> Map::getWallsConst()const {
	return walls;
}

std::vector<Wall> *Map::getWalls()
{
	return &walls;
}

Map::~Map()
{
}





