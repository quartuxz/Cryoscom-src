#pragma once
#include <vector>
#include "unit.h"
#include <SFML/Graphics.hpp>
#include "Gear.h"

class Bullet
{
private:
	unit *bulletBody;

	int ID;
	static std::map<unsigned int ,Bullet*> allBullets;

	float timeSinceCreation;
public:
	static std::map<unsigned int, Bullet*> getAllBullets();
	static void eraseBulletElement(unsigned int);

	combatModule* cModule;

	void create(sf::Vector2f, sf::Vector2f);
	Bullet(sf::Vector2f, sf::Vector2f, combatModule*);
	int hits(std::vector<unit*>);
	bool onHit(unit*);
	bool onWallHit();
	bool onExpiry();
	int getID()const;
	void update(float);

	unit* getBody()const;

	~Bullet();
};

