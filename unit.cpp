#include "unit.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <iostream>
#include "Animator.h"
using namespace std;


unsigned int unit::ownedByIDTracker = 0;

unit::unit(std::pair<sf::Vector2f, float> m_body)
{
	static int IDCounter = 0;
	animationController->isActive = false;
	body = m_body;
	//texture.setRadius(m_body[0].second);
	//texture.setOrigin(sf::Vector2f(m_body[0].second, m_body[0].second));
	//texture.setPosition(m_body[0].first);
	//texture.setFillColor(sf::Color(255, 0, 0));
	ID = IDCounter++;
	m_bodyRadiusRaisedBy2 = pow(body.second,2);
}

unit::unit(const unit& other) : unit(other.body)
{

	actualWeight = other.actualWeight;
	lastAnimatorSprite = other.lastAnimatorSprite;
	animatorSprite = other.animatorSprite;
	currentPos = other.currentPos;
	acceleration = other.acceleration;
	velocity = other.velocity;
	animatorValues = other.animatorValues;
	cModule = other.cModule;
	typeOfUnit = other.typeOfUnit;


}

unit& unit::operator=(const unit &other)
{
	if (this != &other) {
		*this = unit(other);
	}
	return *this;
}

float unit::getBodyRadiusRaisedBy2() const
{
	return m_bodyRadiusRaisedBy2;
}

const std::pair<sf::Vector2f, float> *unit::getBody()const
{
	return &body;
}

void unit::stopMovement()
{
	velocity = sf::Vector2f(0,0);
	acceleration = sf::Vector2f(0,0);
}

sf::Vector2f unit::getVelocity() const
{
	return velocity;
}

void unit::applyMoveSpeed(sf::Vector2f unitVec)
{
	applyInstantVelocity(unitVec, cModule.moveSpeed);
}

void unit::applyInstantVelocity(sf::Vector2f unitVec, float mag)
{
	applyInstantVelocity(sf::Vector2f(unitVec.x * mag, unitVec.y * mag));
}

void unit::applyInstantVelocity(sf::Vector2f vel)
{
	velocity = vel;
}

void unit::addVelocity(sf::Vector2f unitVec, float mag)
{
	addVelocity(sf::Vector2f(unitVec.x * mag, unitVec.y * mag));
}

void unit::addVelocity(sf::Vector2f vel)
{
	velocity += vel;
}

void unit::applyForce(sf::Vector2f unitVec, float mag)
{
	applyForce(sf::Vector2f(unitVec.x * mag, unitVec.y * mag));
}

void unit::applyForce(sf::Vector2f force)
{
	force = sf::Vector2f(force.y /actualWeight, force.y / actualWeight);
	acceleration += force;
}

AnimatorSprite * unit::getAnimationController() const
{
	return animationController;
}

void unit::setAnimationController(AnimatorSprite *aSprite)
{
	animationController = aSprite;
}

bool unit::isMoving(float threshHold) const
{
	return ((abs(velocity.x)+abs(velocity.y))> threshHold);
}

AnimatorSprite unit::getAnimatorSprite()
{
	bool defaultedToLast = false;
	AnimatorSprite retVal;
	retVal = animatorSprite;
	retVal.position = body.first;
	if (animatorValues.usingCompositeTextures) {

		if (!isMoving()) {
			retVal = animatorValues.m_getIdleASprite(lastLookDir);
			m_resetAnimationController();
			defaultedToLast = true;
		}
		else if (velocity.x < 0 && velocity.y < 0) {
			retVal = m_getAnimatorSpriteHelper(back_left);
			//retVal = animatorValues.back_left_idle;
			
			
		}
		else if (velocity.x > 0 && velocity.y < 0) {
			retVal = m_getAnimatorSpriteHelper(back_right);
			//retVal = animatorValues.back_right_idle;
		}
		else if (velocity.x < 0 && velocity.y > 0) {
			retVal = m_getAnimatorSpriteHelper(front_left);
			//retVal = animatorValues.front_left_idle;
		}
		else{

			retVal = m_getAnimatorSpriteHelper(front_right);

			//retVal = animatorValues.front_right_idle;
			//lastAnimatorSprite = animatorValues.front_right_idle;
			//if (!animationController->isActive) {
			//	retVal = animatorValues.front_right_idle;
			//	Animator::getInstance().playAnimation(Animator::getInstance().getAnimationPresetID("player_north_west_walking"), animationController);
			//}
			//else {
			//	retVal = *animationController;
			//}
		}
		retVal.scale *= body.second / ((Animator::getInstance().getTexture(retVal.textureID)->getSize().y + Animator::getInstance().getTexture(retVal.textureID)->getSize().x) / 4);
		retVal.position = body.first;
		retVal.position.y -= body.second * 0.5;
	}
	
	if (!defaultedToLast) {
		lastAnimatorSprite = retVal;
	}

	return retVal;
}

void unit::setAnimatorSprite(AnimatorSprite aSprite, float additionalScale)
{

	aSprite.scale = body.second / min(Animator::getInstance().getTexture(aSprite.textureID)->getSize().x / 2, Animator::getInstance().getTexture(aSprite.textureID)->getSize().y / 2);
	aSprite.scale += additionalScale;
	animatorSprite = aSprite;
}

sf::Vector2f unit::predictNextPos(float timeDelta)
{
	return body.first + (velocity*timeDelta);
}

sf::Vector2f unit::predictNextPos()
{
	return body.first + (velocity*lastTime);
}

void unit::move(sf::Vector2f delta)
{
    lastLastPos = lastPos;
    lastPos = body.first;
	currentPos += delta;
	body.first += delta;

	lastMove = delta;

}

void unit::setPosition(sf::Vector2f pos)
{
	move(pos-body.first);
}

sf::Vector2f unit::getPosition() const
{
	return currentPos;
}

float unit::getVelocityMag() const
{
	return sqrt(pow(velocity.x,2) + pow(velocity.y, 2));
}

sf::Vector2f unit::getUnitVecTo(sf::Vector2f pos)
{
	sf::Vector2f dist = sf::Vector2f(pos) - (body.first);
	float mag = sqrt(pow(dist.x, 2) + pow(dist.y, 2));
	return sf::Vector2f(dist.x / mag, dist.y / mag);
}

void unit::kill(){
    isDead = true;
    move(-body.first);
    move(graveyard);
}

void unit::collideOne(unit *collider) {
	if (collider->getID() != ID) {
		sf::Vector2f evadePos;
		float collisionVal = collides(*collider, &evadePos);
		if (collisionVal != -1) {
			//std::cout << "asd" << std::endl;
			//direction-of-movement based collision avoidance
			//evadePos = sf::Vector2f(0, 0);
			float temp = collisionVal / sqrt(pow(evadePos.x, 2) + pow(evadePos.y, 2));
			evadePos *= temp;
			//weight based momentum transfer
			//colliders[i]->actions.push(std::pair<actionsOnUnit, metaActionData>(moves, -evadePos));
			collider->move(-evadePos * actualWeight / (actualWeight + collider->actualWeight));
			move(evadePos * collider->actualWeight / (actualWeight + collider->actualWeight));
		}
	}

}
void unit::m_collide(std::vector<unit*> colliders) {
	for (size_t i = 0; i < colliders.size(); i++)
	{
		collideOne(colliders[i]);
	}

}

void unit::m_resetAnimationController()
{
	animationController->isActive = false;
	Animator::getInstance().eraseInactiveAnimatorPresets();
	delete animationController;
	animationController = new AnimatorSprite();
	animationController->isActive = false;
}

AnimatorSprite unit::m_getAnimatorSpriteHelper(lookDirection lDir)
{
	AnimatorSprite* defaultIdleASprite = &animatorValues.front_right_idle;
	unsigned int animationPresetID = 0;

	if (lastLookDir != lDir) {
		m_resetAnimationController();
	}

	switch (lDir)
	{
	case back_left:
		defaultIdleASprite = &animatorValues.back_left_idle;
		animationPresetID = animatorValues.back_left_walking;
		break;
	case back_right:
		defaultIdleASprite = &animatorValues.back_right_idle;
		animationPresetID = animatorValues.back_right_walking;
		break;
	case front_left:
		defaultIdleASprite = &animatorValues.front_left_idle;
		animationPresetID = animatorValues.front_left_walking;
		break;
	case front_right:
		defaultIdleASprite = &animatorValues.front_right_idle;
		animationPresetID = animatorValues.front_right_walking;
		break;
	}
	AnimatorSprite retVal;
	lastAnimatorSprite = *defaultIdleASprite;
	if (!animatorValues.hasWalking) {
		return *defaultIdleASprite;
	}
	if (!animationController->isActive) {
		retVal = *defaultIdleASprite;
		Animator::getInstance().playAnimation(animationPresetID, animationController);
	}
	else {
		retVal = *animationController;
	}
	lastLookDir = lDir;
	return retVal;
}



void unit::update(float seconds, std::vector<unit*> colliders)
{
	if (seconds > 1) {
		velocity += acceleration;
		acceleration = sf::Vector2f(0,0);
	}
	else {
		velocity += acceleration * seconds;
		acceleration -= acceleration * seconds;
	}

	float accelerationMagnitude = sqrt(pow(acceleration.x, 2) + pow(acceleration.y, 2));
	if (accelerationMagnitude < 0.0001) {
		acceleration = sf::Vector2f(0, 0);
	}

	move(velocity * seconds);
	cModule.processEffects(seconds);
	cModule.update(seconds);


	if (!cModule.pushes.empty()) {
		for (size_t i = 0; i < cModule.pushes.size(); i++)
		{
			move(cModule.pushes[i].first * std::min<float>(seconds, cModule.pushes[i].second));
			cModule.pushes[i].second -= seconds;
			if (cModule.pushes[i].second < 0) {
				cModule.pushes.erase(cModule.pushes.begin() + i);
			}
		}
			
	}


	if (cModule.hitpoints <= 0 && recentlyDead) {
		kill();
	}


	m_collide(colliders);


	if (velocity.x == 0 && velocity.y == 0) {

	}
	else {
		animatorSprite.rotation = getAngleInDegrees(getUnitVec(sf::Vector2f(0, 0), velocity*seconds));
	}

	if (animationController != nullptr) {
		//animationController->position = body[0].first;
		//animationController->rotation = animatorSprite.rotation;
	}


	lastTime = seconds;

}

float unit::getWeight() const
{
	return actualWeight;
}

void unit::setWeight(float newWeight)
{
	actualWeight = newWeight;
}

int unit::getID() const
{
	return ID;
}

float unit::collides(const unit &collider, sf::Vector2f *evadePos)
{
	//std::cout << body[i].first.x << ", " << body[i].first.y << std::endl;
	//std::cout << collider.body[o].first.x << ", " << collider.body[o].first.y << std::endl;

	float xDis = body.first.x - collider.body.first.x;
	float yDis = body.first.y - collider.body.first.y;
	float xSq = xDis * xDis;
	float ySq = yDis * yDis;

	float temp = sqrt(xSq + ySq);

	//cout << temp << ", " << (body[i].second + collider.body[o].second) << std::endl;
	if (temp < (body.second + collider.body.second)) {
		if (evadePos != nullptr) {
			*evadePos = body.first - collider.body.first;
		}
		return (body.second + collider.body.second) - temp;
	}
	return -1;
}


unit::~unit()
{
	delete animationController;
}


AnimatorSprite unitAnimatorValues::m_getIdleASprite(lookDirection lDir)
{
	switch (lDir)
	{
	case back_left:
		return back_left_idle;
		break;
	case back_right:
		return back_right_idle;
		break;
	case front_left:
		return front_left_idle;
		break;
	case front_right:
		return front_right_idle;
		break;
	default:
		break;
	}
}

unit* makePlayer(sf::Vector2f pos, float radius, float weight)
{
	std::pair<sf::Vector2f, float> unitBody;
	unitBody = std::pair<sf::Vector2f, float>(pos, radius);
	unit* retVal = new unit(unitBody);
	retVal->typeOfUnit = playerType;
	retVal->setWeight(weight);
	/*if (tokens.size() > 5) {
		AnimatorSprite tempASprtie;
		tempASprtie.textureID = Animator::getInstance().getTextureID(tokens[5]);

		retVal->setAnimatorSprite(tempASprtie);
	}*/
	//m_player->animatorValues.usingCompositeTextures = false;
	/*AnimatorSprite tempASprite;
	tempASprite.textureID = 1;
	m_player->setAnimatorSprite(tempASprite);
	mutexLock.unlock();
	return;*/
	retVal->animatorValues.back_left_idle.textureID = Animator::getInstance().getTextureID("player_back_left_idle.png");
	retVal->animatorValues.back_left_idle.drawLayer = 2;
	retVal->animatorValues.back_right_idle.textureID = Animator::getInstance().getTextureID("player_back_right_idle.png");
	retVal->animatorValues.back_right_idle.drawLayer = 2;
	retVal->animatorValues.front_left_idle.textureID = Animator::getInstance().getTextureID("player_front_left_idle.png");
	retVal->animatorValues.front_left_idle.drawLayer = 2;
	retVal->animatorValues.front_right_idle.textureID = Animator::getInstance().getTextureID("player_front_right_idle.png");
	retVal->animatorValues.front_right_idle.drawLayer = 2;
	retVal->animatorValues.front_right_walking = Animator::getInstance().getAnimationPresetID("player_front_right_walk");
	retVal->animatorValues.front_left_walking = Animator::getInstance().getAnimationPresetID("player_front_left_walk");
	retVal->animatorValues.back_right_walking = Animator::getInstance().getAnimationPresetID("player_back_right_walk");
	retVal->animatorValues.back_left_walking = Animator::getInstance().getAnimationPresetID("player_back_left_walk");
	retVal->animatorValues.usingCompositeTextures = true;
	retVal->animatorValues.hasWalking = true;
	retVal->animatorValues.hasRunning = false;
	return retVal;
}
