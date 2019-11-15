#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Serializable.h"

struct Wall: public Serializable {
	std::pair<sf::Vector2f, sf::Vector2f> wall;
	sf::Vector2f inside;
	float distanceBetweenPoints;
	float distanceBetweenPointsDividedBy2RaisedBy2;

	sf::Vector2f middlePoint;
	void bake(bool clockWiseFromFirst = true);
	sf::Vector2f getOutside();
	bool isActive = true;

	void createFrom(const decomposedData&)override;
	decomposedData serialize()override;
};

