#include "Wall.h"
#include "cryoscom_defsAndUtils.h"

void Wall::calculateInside(bool clockWiseFromFirst)
{
	inside = rotate90(sf::Vector2f((wall.first.x + wall.second.x) / 2, (wall.first.y + wall.second.y) / 2), wall.first);

	float dist = vectorDistance(inside - (sf::Vector2f((wall.first.x + wall.second.x) / 2, (wall.first.y + wall.second.y) / 2)), sf::Vector2f((wall.first.x + wall.second.x) / 2, (wall.first.y + wall.second.y) / 2));
	//std::cout << wall.inside.x << ", " << wall.inside.y << std::endl;
	inside.x -= (wall.first.x + wall.second.x) / 2;
	inside.y -= (wall.first.y + wall.second.y) / 2;
	//std::cout << (first.x + second.x) / 2 << std::endl;
	//std::cout << wall.inside.x << ", " << wall.inside.y << std::endl;
	inside.x = inside.x / dist;
	inside.y = inside.y / dist;
	//std::cout << wall.inside.x << ", " << wall.inside.y << std::endl;
	inside.x += (wall.first.x + wall.second.x) / 2;
	inside.y += (wall.first.y + wall.second.y) / 2;
	//std::cout << wall.inside.x << ", " << wall.inside.y << std::endl;
}

sf::Vector2f Wall::getOutside()
{
	return reflect(wall.first, wall.second, inside);
}