#pragma once
#include <SFML/Graphics.hpp>

struct AnimatorSprite {
	unsigned int textureID;
	unsigned drawLayer = 0;

	//animation variables, counting an object of this struct as a single frame
	float timeDisplayed = 0.5;
	float timeElapsed = 0;

	bool originToCenter = true;
	bool offsets = false;
	bool isActive = true;

	bool usesVectorScale = false;
	sf::Vector2f vectorScale = sf::Vector2f(1,1);

	float rotation = 0;
	float scale = 1;
	sf::Vector2f position = sf::Vector2f(0, 0);

	AnimatorSprite();

	static AnimatorSprite create(bool);

	//constructs an object with the given texture ID and default parameters
	explicit AnimatorSprite(unsigned int);
	//constructs an object with the given texture ID, drawLayer and default parameters
	AnimatorSprite(unsigned int, unsigned int);
};

