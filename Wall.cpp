#include "Wall.h"
#include "cryoscom_defsAndUtils.h"

sf::Vector2f Wall::getOutside()
{
	return reflect(wall.first, wall.second, inside);
}