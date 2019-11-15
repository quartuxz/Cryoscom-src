#pragma once
#include <SFML/Graphics.hpp>
#include "Serializable.h"

enum rescaleTypes {
	fitX, fitY, fitMin, fitDistorted
};

struct AnimatorSprite : public Serializable{

	static float zoom;

	unsigned int textureID;
	unsigned drawLayer = 0;

	//animation variables, counting an object of this struct as a single frame
	float timeDisplayed = 0.5;
	float timeElapsed = 0;

	bool isUI = false;
	sf::Vector2f UIDisplacement = sf::Vector2f(0,0);

	bool isInvisible = false;

	bool originToCenter = true;
	bool offsets = false;
	bool isActive = true;

	bool usesVectorScale = false;
	sf::Vector2f vectorScale = sf::Vector2f(1,1);

	float rotation = 0;
	float scale = 1;
	sf::Vector2f position = sf::Vector2f(0, 0);

	AnimatorSprite();

	void scaleToMatch(rescaleTypes, float);
	void scaleToMatch(sf::Vector2f);

	void createFrom(const decomposedData&);
	decomposedData serialize();

	static AnimatorSprite create(bool);

	//constructs an object with the given texture ID and default parameters
	explicit AnimatorSprite(unsigned int);
	//constructs an object with the given texture ID, drawLayer and default parameters
	AnimatorSprite(unsigned int, unsigned int);
};

