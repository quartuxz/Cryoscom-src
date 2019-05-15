#include "EnemyAI.h"
#include <iostream>
#include <math.h>
#include <limits>
#include <random>
#include "globalMutexes.h"


bool EnemyAI::globalChecksForAccuracy = true;

EnemyAI::EnemyAI()
{
}

EnemyAI::EnemyAI(unit *aunit, Weapon *weapon):
	m_controlledUnit(aunit),
	m_controlledWeapon(weapon)
{

}

void EnemyAI::createFromFile(std::string scriptName)
{
    try
    {
        boost::python::str tempStr(scriptName);
        m_pythonFunc = boost::python::import(tempStr).attr("AIBehaviour");
    }
    catch (const boost::python::error_already_set&)
    {
        PyErr_Print();
        while(true){

        }
        m_pythonFunc = boost::python::api::object();
        return;
    }
}

void EnemyAI::drawBullets(sf::RenderWindow &window)
{
	m_controlledWeapon->displayToWindow(window);
}

void EnemyAI::setHead(unit *head, sf::Vector2f followingDistance){
    m_followingHead = head;
    m_followingDistance = followingDistance;
}

void EnemyAI::update(float timeDelta, std::vector<unit*> targets, MessageBus *gameBus)
{
	//without_gil no_gil;
	with_gil gil;
	m_currentAITimeToInterval += timeDelta;
	unit *chosenTarget = targets[rand() % targets.size()];



    if(m_pythonFunc != boost::python::api::object()&& (m_currentAITimeToInterval >= m_AIUpdateInterval)){
		m_currentAITimeToInterval = 0;
        sf::Vector2f headPos = sf::Vector2f(0,0);
        float headRadius = 0;
        bool hasHead = false;
        if(m_followingHead != nullptr){
            headPos = m_followingHead->getBody()[0].first;
            headRadius = m_followingHead->getBody()[0].second;
            hasHead = true;
        }

        boost::python::list listElement;
        listElement.append(chosenTarget->getBody()[0].first.x);
        listElement.append(chosenTarget->getBody()[0].first.y);
        boost::python::list playerPoss;
        playerPoss.append(listElement);
        listElement.pop(0);
        listElement.pop(0);
        listElement.append(chosenTarget->lastPos.x);
        listElement.append(chosenTarget->lastPos.y);
        playerPoss.append(listElement);
        listElement.pop(0);
        listElement.pop(0);
        listElement.append(chosenTarget->lastLastPos.x);
        listElement.append(chosenTarget->lastLastPos.y);
        playerPoss.append(listElement);
		boost::python::object retVal;
		{
			
			retVal = m_pythonFunc(playerPoss, chosenTarget->getBody()[0].second,
                                                    chosenTarget->getVelocity().x, chosenTarget->getVelocity().y,
                                                    m_controlledWeapon->canFire(),
                                                    m_controlledUnit->getBody()[0].first.x, m_controlledUnit->getBody()[0].first.y, m_controlledUnit->getBody()[0].second,
                                                    m_controlledUnit->cModule.bulletSpeed,
                                                    hasHead,
                                                    headPos.x, headPos.y, headRadius,
                                                    m_followingDistance.x, m_followingDistance.y);
		}
        m_moveToPos.x = boost::python::extract<float>(retVal[0]);
        m_moveToPos.y = boost::python::extract<float>(retVal[1]);
        addedMoveSpeed = boost::python::extract<float>(retVal[2]);
        shootPos.x = boost::python::extract<float>(retVal[3]);
        shootPos.y = boost::python::extract<float>(retVal[4]);
        shootsNow = boost::python::extract<bool>(retVal[5]);
    }else if(m_followingHead != nullptr){
        m_moveToPos = m_followingHead->getBody()[0].first + m_followingDistance;
    }

//    if(m_controlledWeapon->canFire() && false){
//        if((rand() % m_likelyHoodOfAccuracy) == 0 && checksForAccuracy && globalChecksForAccuracy){
//            distToTarget = vectorDistance(chosenTarget->getPosition(), m_controlledUnit->getBody()[0].first);
//            timeToTarget = distToTarget / m_controlledUnit->Amodule->bulletSpeed;
//            sf::Vector2f newCTargetPos = chosenTarget->predictNextPos(timeToTarget);
//            float lastTimeToTarget = std::numeric_limits<float>::max();
//            int maxIterations = m_maxAccuracyIterations;
//            while (abs(timeToTarget - lastTimeToTarget) > m_accuracyThreshHold) {
//                //std::cout << "calcDist!" << std::endl;
//                distToTarget = vectorDistance(newCTargetPos, m_controlledUnit->getBody()[0].first);
//                if (distToTarget > (m_controlledUnit->Amodule->bulletSpeed * m_controlledUnit->Amodule->bulletDuration)) {
//                    maxIterations--;
//                }
//
//                if (--maxIterations <= 0) {
//                    break;
//                }
//                lastTimeToTarget = timeToTarget;
//                timeToTarget = distToTarget / m_controlledUnit->Amodule->bulletSpeed;
//                newCTargetPos = chosenTarget->predictNextPos(timeToTarget);
//            }
//
//        }
//        if (distToTarget > (m_controlledUnit->Amodule->bulletSpeed * m_controlledUnit->Amodule->bulletDuration)) {
//            timeToTarget = timeDelta *2;
//        }
//
//        //std::cout << std::endl;
//
//
//
//        if (distToTarget < (m_controlledUnit->Amodule->bulletSpeed * m_controlledUnit->Amodule->bulletDuration)||true) {
//            m_controlledWeapon->fire(m_controlledUnit->getUnitVecTo(chosenTarget->predictNextPos(timeToTarget)), timeDelta);
//        }
//        else {
//            m_controlledWeapon->fire(m_controlledUnit->getUnitVecTo(chosenTarget->predictNextPos()), timeDelta);
//        }
//    }


    if(shootsNow){
        m_controlledWeapon->fire(m_controlledUnit->getUnitVecTo(shootPos));
    }

    m_controlledUnit->cModule.moveSpeed *= addedMoveSpeed;
    if(vectorDistance(m_moveToPos, m_controlledUnit->getBody()[0].first) > 2){
        m_controlledUnit->applyMoveSpeed(m_controlledUnit->getUnitVecTo(m_moveToPos));
    }else{
        m_controlledUnit->applyInstantVelocity(sf::Vector2f(0,0));

    }
    m_controlledUnit->cModule.moveSpeed /= addedMoveSpeed;

	m_controlledWeapon->update(timeDelta, targets);
}

unit * EnemyAI::getUnit()
{
	return m_controlledUnit;
}

Weapon * EnemyAI::getWeapon()
{
	return m_controlledWeapon;
}

void EnemyAI::setUnit(unit *aunit)
{
	m_controlledUnit = aunit;
}

void EnemyAI::setWeapon(Weapon *weapon)
{
	m_controlledWeapon = weapon;
}


EnemyAI::~EnemyAI()
{
	delete m_controlledUnit;
	delete m_controlledWeapon;
}
