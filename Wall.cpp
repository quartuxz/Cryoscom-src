#include "Wall.h"
#include "cryoscom_defsAndUtils.h"

void Wall::bake(bool clockWiseFromFirst)
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

	middlePoint.x = (wall.first.x + wall.second.x) / 2;
	middlePoint.y = (wall.first.y + wall.second.y) / 2;

	distanceBetweenPoints = vectorDistance(wall.first, wall.second);
	distanceBetweenPointsDividedBy2RaisedBy2 = pow(distanceBetweenPoints / 2,2);
}

sf::Vector2f Wall::getOutside()
{
	return reflect(wall.first, wall.second, inside);
}

void Wall::createFrom(const decomposedData &DData)
{
	decomposedData tempDData = DData;
	wall.first = sf::Vector2f(ma_deserialize_float(tempDData.getChildByName("firstWall")->data[0]), ma_deserialize_float(tempDData.getChildByName("firstWall")->data[1]));
	wall.second = sf::Vector2f(ma_deserialize_float(tempDData.getChildByName("secondWall")->data[0]), ma_deserialize_float(tempDData.getChildByName("secondWall")->data[1]));
}
//make walls serializable for wall offloading
decomposedData Wall::serialize()
{
	return decomposedData();
}
