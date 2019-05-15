#pragma once
#include <queue>
#include <vector>
#include <SFML/Graphics.hpp>
#include "unit.h"
#include "Weapon.h"
#include "MessageBus.h"

#include <Python.h>
#include <boost/python.hpp>
#include <cstdlib>



class EnemyAI
{
private:
	unit *m_controlledUnit;
	Weapon *m_controlledWeapon;

	unit *m_followingHead = nullptr;

	sf::Vector2f m_followingDistance = sf::Vector2f(0,0);

	int m_distanceForStop = 5;

	float m_AIUpdateInterval = 0.1;
	float m_currentAITimeToInterval = m_AIUpdateInterval;

	float m_accuracyThreshHold = 0.10f;
	int m_maxAccuracyIterations = 20;
	int m_likelyHoodOfAccuracy = 1; //one in every how many attacks checks for accurate(slow) bullet direction

    boost::python::object m_pythonFunc;

    sf::Vector2f m_moveToPos = sf::Vector2f(0,0);


	//update function variables
	float addedMoveSpeed = 0;

	float distToTarget = 0;
	float timeToTarget = 100;

	sf::Vector2f shootPos = sf::Vector2f(0, 0);
	bool shootsNow = false;

public:
	bool checksForAccuracy = true;//desactivating this significantly improves performance, but makes AI units very inaccurate
	static bool globalChecksForAccuracy;//desactivates checking for accuracy for all EnemyAIs

	EnemyAI();
	EnemyAI(unit*, Weapon*);
	void createFromFile(std::string);
	void drawBullets(sf::RenderWindow&);
	void update(float, std::vector<unit*>, MessageBus*);
	unit *getUnit();
	Weapon *getWeapon();
	void setUnit(unit*);
	void setWeapon(Weapon*);
	void setHead(unit*, sf::Vector2f);

	~EnemyAI();
};

