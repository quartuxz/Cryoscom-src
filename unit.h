#pragma once
#include <vector>
#include <map>
#include <tuple>
#include <functional>
#include <random>
#include <mutex>
#include <stack>
#include <math.h>
#include <SFML/Graphics.hpp>
#include "Gear.h"
//#include "Weapon.h"
#include "AnimatorSprite.h"
#include "cryoscom_defsAndUtils.h"

const sf::Vector2f graveyard = sf::Vector2f(0, 0);



enum unitType {
	playerType, bulletType, defaultType
};  

enum lookDirection {
	back_left,back_right,front_left,front_right
};

struct unitAnimatorValues {

	bool usingCompositeTextures = false;

	AnimatorSprite back_left_idle;
	AnimatorSprite back_right_idle;
	AnimatorSprite front_left_idle;
	AnimatorSprite front_right_idle;

	AnimatorSprite m_getIdleASprite(lookDirection);

	bool hasWalking = false;
	unsigned int back_left_walking;
	unsigned int back_right_walking;
	unsigned int front_left_walking;
	unsigned int front_right_walking;

	bool hasRunning = false;
	unsigned int back_left_running;
	unsigned int back_right_running;
	unsigned int front_left_running;
	unsigned int front_right_running;
};

class unit
{
private:

	//std::mutex m_allMutex;

	std::pair<sf::Vector2f, float> body;
	float actualWeight = 100;

	//the last update cycle time
	float lastTime = 0;

	AnimatorSprite lastAnimatorSprite;
	AnimatorSprite animatorSprite;

	sf::Vector2f currentPos;


	sf::Vector2f acceleration;
	sf::Vector2f velocity = sf::Vector2f(0,0);

	unsigned int ID;

	AnimatorSprite *animationController = new AnimatorSprite();

	
	void m_collide(std::vector<unit*>);

	lookDirection lastLookDir;

	float m_bodyRadiusRaisedBy2;


	void m_resetAnimationController();

	AnimatorSprite m_getAnimatorSpriteHelper(lookDirection);

public:
	static unsigned int ownedByIDTracker;

	unit(const unit&);
	unit& operator =(const unit&);

	unitAnimatorValues animatorValues;

	sf::Vector2f lastMove;

    sf::Vector2f lastPos;
    sf::Vector2f lastLastPos;

	AnimatorSprite *getAnimationController()const;
	void setAnimationController(AnimatorSprite*);

	combatModule cModule;

	bool isMoving(float threshHold = 0.1)const;

	//gets the animator Sprite and updates is values
	AnimatorSprite getAnimatorSprite();
	void setAnimatorSprite(AnimatorSprite, float additionalScale = 0);

    void kill();

	sf::Vector2f predictNextPos(float);
	sf::Vector2f predictNextPos();

	int ownedByID;
	bool isDead = false;
	bool recentlyDead = true;
	unitType typeOfUnit = defaultType;

	void move(sf::Vector2f);
	unit(std::pair<sf::Vector2f, float>);

	float getBodyRadiusRaisedBy2()const;

	const std::pair<sf::Vector2f, float> *getBody()const;

	void stopMovement();

	sf::Vector2f getVelocity()const;

	void applyMoveSpeed(sf::Vector2f);
	void applyInstantVelocity(sf::Vector2f, float);
	void applyInstantVelocity(sf::Vector2f);
	void addVelocity(sf::Vector2f, float);
	void addVelocity(sf::Vector2f);
	void applyForce(sf::Vector2f, float);
	void applyForce(sf::Vector2f);

	void setPosition(sf::Vector2f);
	sf::Vector2f getPosition()const;

	float getVelocityMag()const;

	sf::Vector2f getUnitVecTo(sf::Vector2f);

	void collideOne(unit*);

	//updates the position in accordance to the surroundings
	void update(float, std::vector<unit*> colliders = std::vector<unit*>());

	float getWeight()const;
	void setWeight(float);

	int getID()const;

	//calculates the margin by which to spheres collide, without giving the most optimal movement for de-collision, too much math, collision avoidance is handled by the update function.
	float collides(const unit&, sf::Vector2f *evadePos = nullptr);

	~unit();
};


//creates a dynamically allocated player unit
//(first argument is the position second is the size third is the physics weight)
unit* makePlayer(sf::Vector2f, float, float);