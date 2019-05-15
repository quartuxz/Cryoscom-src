#include "Weapon.h"
#include <iostream>
#include "Animator.h"
#include "cryoscom_defsAndUtils.h"
#include <limits>



void Weapon::setBulletTex(sf::Sprite &circleTex)
{
	m_bulletTex = circleTex;
}

void Weapon::setbulletAnimatorTex(AnimatorSprite aSprite)
{
	m_bulletATex = aSprite;
}

sf::Vector2f Weapon::getUnitVecTo(sf::Vector2f posTo)
{
	return m_wieldingUnit->getUnitVecTo(posTo);
}

Weapon::Weapon(unit *wieldingUnit):
m_wieldingUnit(wieldingUnit)
{
}

void Weapon::m_useSkill(skillParam* sParam, std::vector<unit*> units) {
	AnimatorSprite tempASprite;
	switch (sParam->sType)
	{
	case dash:

			m_wieldingUnit->cModule.stamina -= sParam->staminaCost;
			m_wieldingUnit->cModule.pushes.push_back(std::pair<sf::Vector2f, float>());
			m_wieldingUnit->cModule.pushes.back().first = sf::Vector2f(sParam->dirUnitVec.x * m_wieldingUnit->cModule.moveSpeed * (100 / (m_wieldingUnit->cModule.weight + 20)), sParam->dirUnitVec.y * m_wieldingUnit->cModule.moveSpeed * (100 / (m_wieldingUnit->cModule.weight + 20)));
			m_wieldingUnit->cModule.pushes.back().second = 0.25;
				
			
		break;
	case meleeAttack:
		m_wieldingUnit->cModule.stamina -= sParam->staminaCost;

		for (size_t i = 0; i < units.size(); i++)
		{
			if (units[i]->getBody()[0].second >= vectorDistance(units[i]->getBody()[0].first, sParam->dirUnitVec)) {
				tempASprite.textureID = 0;//Animator::getInstance().getTextureID("blood");
				tempASprite.position = sParam->dirUnitVec;
				tempASprite.timeDisplayed = 3;
				tempASprite.rotation = rand() % 360;
				Animator::getInstance().addDecal(tempASprite);
				units[i]->cModule.hitpoints -= sParam->strenght;
				return;
			}
		}
		break;
	default:
		break;
	}
	//std::cout << "dash" << std::endl;
	
}

void Weapon::m_fire(sf::Vector2f unitVec)
{
	//std::cout << m_wieldingUnit->Amodule->bulletSpeed << std::endl;
	//system("pause");
	float r3 = -(m_wieldingUnit->cModule.inaccuracy / 2) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((m_wieldingUnit->cModule.inaccuracy / 2) - -(m_wieldingUnit->cModule.inaccuracy / 2))));
	//float r3 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / Amodule->inaccuracy));
	//std::cout << r3 << std::endl;
	if (r3 < 0) {
		r3 += 360;
	}
	//std::cout << unitVec.x << ", " << unitVec.y << std::endl;
	unitVec = rotateByAngle(sf::Vector2f(0,0), unitVec, r3);
	//std::cout << unitVec.x << ", " << unitVec.y  << std::endl;
	m_bullets.push_back(new Bullet(m_wieldingUnit->getBody()[0].first + (unitVec * m_wieldingUnit->getBody()[0].second), unitVec, &m_wieldingUnit->cModule));
	m_bullets.back()->cModule = &m_wieldingUnit->cModule;
	m_bullets.back()->getBody()->setTexture(m_bulletTex);
	m_bullets.back()->getBody()->setAnimatorSprite(m_bulletATex);
#if ADD_PLAYER_VELOCITY_TO_BULLET
	m_bullets.back()->getBody()->addVelocity(m_wieldingUnit->getVelocity());
#endif
}

bool Weapon::fire(sf::Vector2f unitVec)
{
	if (m_wieldingUnit->cModule.isMelee) {
		if (!canFire()) {
			return false;
		}

		skillParam *tempSkillParam = new skillParam();
		tempSkillParam->sType = meleeAttack;
		tempSkillParam->dirUnitVec = unitVec;
		tempSkillParam->strenght = m_wieldingUnit->cModule.damage;
		tempSkillParam->staminaCost = 5;
	
		if (addSkillToQueue(tempSkillParam) == notFail) {
			
			return true;
		}else{
			return false;
		}
	}

    if(m_magBulletsUsed >= m_wieldingUnit->cModule.magSize){
        //std::cout << m_wieldingUnit->Amodule->reloadTime << std::endl;
        if(m_accumulatedFireTime >= m_wieldingUnit->cModule.reloadTime){
            m_magBulletsUsed = 0;
        }
        return false;
	}
    if(canFire()){
		m_fire(unitVec);
		m_accumulatedFireTime = 0;
		m_magBulletsUsed++;
		m_wieldingUnit->cModule.ammo -= 1;
		return true;
	}
	return false;
}


//todo: implement in-melee-range checking and return val
failCastTypes Weapon::addSkillToQueue(skillParam* sParam)
{
	std::cout << sParam->dirUnitVec.x << ", " << sParam->dirUnitVec.y << std::endl;
	if (vectorDistance(sParam->dirUnitVec, m_wieldingUnit->getBody()[0].first) <= sParam->maxRange ||sParam->sType != meleeAttack) {
		if (m_wieldingUnit->cModule.psiPoints >= sParam->manaCost) {
			if (m_wieldingUnit->cModule.stamina >= sParam->staminaCost) {

				m_activeSkills.push(sParam);

				return notFail;
			}
			return outOfStamina;
		}
		return outOfPsiPoints;
	}
	return outOfMeleeRange;
}

std::vector<unsigned int> Weapon::update(float timeDelta, std::vector<unit*> units)
{


	while (!m_activeSkills.empty()) {
		m_useSkill(m_activeSkills.front(), units);
		delete m_activeSkills.front();
		m_activeSkills.pop();
	}

	std::vector<unsigned int> retVal;
	std::list<Bullet*>::iterator it = m_bullets.begin();
	while (it != m_bullets.end()) {
		(*it)->update(timeDelta*m_bulletSpeed);
		int tempID = (*it)->hits(units);
		if (tempID != -1) {
			retVal.push_back(tempID);
		}
		if ((*it)->getBody()->isDead) {
			delete Bullet::getAllBullets()[(*it)->getID()];
			m_bullets.erase(it++);
		}
		else {
			++it;
		}
	}
	m_accumulatedFireTime += timeDelta;
	return retVal;
}

std::vector<unsigned int> Weapon::update(float timeDelta, std::queue<unit*> units)
{
	std::vector<unit*> updateUnits;
	while(!units.empty()){
		updateUnits.push_back(units.front());
		units.pop();
	}
	return update(timeDelta, updateUnits);
}

std::list<Bullet*> Weapon::getBullets()const
{
	return m_bullets;
}

void Weapon::reload(){
    m_magBulletsUsed = m_wieldingUnit->cModule.magSize;
}

void Weapon::displayToWindow(sf::RenderWindow &window)
{
	std::list<Bullet*>::iterator it;
	for (it = m_bullets.begin(); it != m_bullets.end(); ++it) {
		//window.draw((*it)->getBody()->getTexture());
		Animator::getInstance().addOneFrameSprite((*it)->getBody()->getAnimatorSprite());
	}
}


bool Weapon::canFire(){
    float fireCooldown = 1/ m_wieldingUnit->cModule.fireRate;
	//m_accumulatedFireTime += fireTime;
	if ( (m_accumulatedFireTime > fireCooldown) && (m_magBulletsUsed < m_wieldingUnit->cModule.magSize) && (m_wieldingUnit->cModule.ammo > 0) ) {
        return true;
    }
    return false;
}

Weapon::~Weapon()
{
	std::list<Bullet*>::iterator it;
	for (it = m_bullets.begin(); it != m_bullets.end(); ++it) {
		delete (*it);
	}
}
