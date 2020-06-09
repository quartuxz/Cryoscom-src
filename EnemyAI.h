#pragma once
#include <queue>
#include <vector>
#include <SFML/Graphics.hpp>
#include <stdexcept>

#include "unit.h"
#include "Weapon.h"
#include "MessageBus.h"
#include "TileMap.h"

#include <Python.h>
#include <boost/python.hpp>
#include <cstdlib>


struct EnemyAIDatasheet {

};

class EnemyAI
{
private:
	unit *m_controlledUnit;
	Weapon *m_controlledWeapon;

	unit *m_followingHead = nullptr;

	sf::Vector2f m_followingDistance = sf::Vector2f(0,0);

	static constexpr float m_AIUpdateInterval = 0.1;
	float m_currentAITimeToInterval = m_AIUpdateInterval;

    boost::python::object m_pythonFunc;

    sf::Vector2f m_moveToPos = sf::Vector2f(0,0);

	//deprecated
	//sf::Vector2i m_controlledUnitLastTilePos;
	bool m_firstUpdate = true;

	//update function variables
	float addedMoveSpeed = 0;

	sf::Vector2f shootPos = sf::Vector2f(0, 0);
	bool shootsNow = false;

	bool m_checkIfPathfindingIsNeccesary(const sf::Vector2f&, const std::vector<std::vector<Tile>>&)const;

public:

	EnemyAI(const EnemyAI&);
	EnemyAI& operator =(const EnemyAI&);

	EnemyAI();
	EnemyAI(unit*, Weapon*);
	void createFromFile(std::string);
	void drawBullets(sf::RenderWindow&);
	//input the time delta, the targets, the game message bus, and the game tile map(for collision)
	void update(float, std::vector<unit*>, MessageBus*, TileMap&);
	unit *getUnit();
	Weapon *getWeapon();
	void setUnit(unit*);
	void setWeapon(Weapon*);
	void setHead(unit*, sf::Vector2f);
	//when the underlying unit is killed, this class is informed of that and what the last targets where for its AI
	void onUnitKilled(std::vector<unit*>);

	~EnemyAI();
};

