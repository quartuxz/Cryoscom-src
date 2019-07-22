#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

struct Wall {
	std::pair<sf::Vector2f, sf::Vector2f> wall;
	sf::Vector2f inside;
	void calculateInside(bool clockWiseFromFirst = true);
	sf::Vector2f getOutside();
	bool isActive = true;
};

