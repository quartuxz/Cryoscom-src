#include "Bullet.h"
#include <random>
#include <iostream>

std::map<unsigned int, Bullet*> Bullet::allBullets;

std::map<unsigned int, Bullet*> Bullet::getAllBullets()
{
	return allBullets;
}

void Bullet::eraseBulletElement(unsigned int itemID)
{
	allBullets.erase(itemID);
}

void Bullet::create(sf::Vector2f startPos, sf::Vector2f moveDir)
{
	std::pair<sf::Vector2f, float> body;
	body = std::pair<sf::Vector2f, float>(startPos, cModule->bulletSize);
	bulletBody = new unit(body);
	bulletBody->applyInstantVelocity(sf::Vector2f(moveDir.x, moveDir.y), cModule->bulletSpeed);
}

Bullet::Bullet(sf::Vector2f startPos, sf::Vector2f moveDir, combatModule *cmodule):
	cModule(cmodule)
{

	create(startPos, moveDir);
	bulletBody->typeOfUnit = bulletType;
	ID = unit::ownedByIDTracker++;
	bulletBody->ownedByID = ID;
	allBullets[ID] = this;
}

int Bullet::hits(std::vector<unit*> units)
{
	int retVal = -1;
	for (size_t i = 0; i < units.size(); i++)
	{
		if (units[i]->collides(*bulletBody)>0) {
			cModule->attack(&units[i]->cModule);
			if (units[i]->cModule.hitpoints <= 0) {
                units[i]->kill();
				retVal = i;
			}
			bulletBody->kill();
			return retVal;
		}
	}
	return -1;
}

bool Bullet::onHit(unit *hitUnit)
{
	return true;
}

bool Bullet::onWallHit()
{
	bulletBody->kill();
	return true;
}

bool Bullet::onExpiry()
{
	return false;
}

int Bullet::getID() const
{
	return ID;
}

void Bullet::update(float timeDelta)
{
	timeSinceCreation += timeDelta;
	if (timeSinceCreation > cModule->bulletDuration) {
		bulletBody->move(sf::Vector2f(-bulletBody->getPosition().x, -bulletBody->getPosition().y));
		bulletBody->move(graveyard);
		bulletBody->isDead = true;
		onExpiry();
		//std::cout << "BULLET DEAD" << std::endl;
	}
	bulletBody->update(timeDelta);
}

unit *Bullet::getBody() const
{
	return bulletBody;
}


Bullet::~Bullet()
{
	delete bulletBody;
}
