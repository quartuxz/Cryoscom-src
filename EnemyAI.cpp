#include "EnemyAI.h"
#include <iostream>
#include <math.h>
#include <limits>
#include <random>
#include "globalMutexes.h"
#include "cryoscom_defsAndUtils.h"
#include "InGameMessages.h"

typedef std::pair<bool, std::vector<std::vector<float>>> DistanceMap;

void displayDistanceMap(const DistanceMap &distanceMap) {
    for (size_t i = 0; i < distanceMap.second[0].size(); i++)
    {
        for (size_t o = 0; o < distanceMap.second.size(); o++)
        {
            if (distanceMap.second[o][i] == 0) {
                std::cout << "00" << "-";
            }
            else {
                std::cout << std::min(int(trunc(distanceMap.second[o][i] * 10)),99) << "-";
            }
            
        }
        std::cout << std::endl;
    }
    system("pause");

}

void displayTextTileMapRep(const std::vector<std::vector<Tile>>& tileMapRep) {
    for (size_t i = 0; i < tileMapRep[0].size(); i++)
    {
        for (size_t o = 0; o < tileMapRep.size(); o++)
        {

            std::cout << !tileMapRep[o][i].isEmpty();
        }
        std::cout << std::endl;
    }
    system("pause");

}

//the check for wether the specified radius can pass trough(currently oversimplified)
bool tileIsPassable(sf::Vector2u pos, const std::vector<std::vector<Tile>>& tileMapRep, float radius = Tile::tileSize) {

    return tileMapRep[pos.x][pos.y].isEmpty();
}

//returns wether the function found the endPos and the distance map from staetPos for pathfinding
DistanceMap getDistancesFromStartPoint(sf::Vector2i startPos, 
    sf::Vector2i endPos, 
    const std::vector<std::vector<Tile>> &tileMapRep) {


    DistanceMap retVal = DistanceMap();
    retVal.first = false;
    std::vector<float> tempDistanceMapYDim = std::vector<float>(tileMapRep[0].size(),0);
    retVal.second.resize(tileMapRep.size(), tempDistanceMapYDim);

    std::queue<sf::Vector2i> newStartPositions;
    newStartPositions.push(startPos);
    sf::Vector2i currentPos = startPos;
    while (currentPos != endPos && !newStartPositions.empty()) {
        currentPos = newStartPositions.front();
        newStartPositions.pop();
        for (int i = (currentPos.x - 1); i <= (currentPos.x + 1); i++)
        {
            if (i < 0 || i >= tileMapRep.size()) {
                continue;
            }
            for (int o = (currentPos.y - 1); o <= (currentPos.y + 1); o++)
            {
                //making the "cross" pattern
                if ((i == (currentPos.x-1) && o == (currentPos.y-1))
                    || (i == (currentPos.x+1)&& o == (currentPos.y-1))
                    || (i==(currentPos.x-1)&&o==(currentPos.y+1))
                    || (i==(currentPos.x+1)&&o==(currentPos.y+1))) {
                    continue;
                }
                
                if (o < 0 || o >= tileMapRep[i].size()) {
                    continue;
                }
                if (retVal.second[i][o] != 0) {
                    continue;
                }
                sf::Vector2i ioV2iRep = sf::Vector2i(i, o);
                //checks if the tile has ANY collideable geometry(should
                //improve this later to check if the tile is PASSABLE!!!)
                if (!tileIsPassable(sf::Vector2u(i,o), tileMapRep) || ioV2iRep == currentPos || ioV2iRep == startPos) {
                    continue;
                }
                float distanceFromCurrent = vectorDistance(sf::Vector2f(ioV2iRep), sf::Vector2f(currentPos));
                retVal.second[i][o] = distanceFromCurrent + retVal.second[currentPos.x][currentPos.y];

                newStartPositions.push(ioV2iRep);
            }
        }
    }
    if (currentPos == endPos) {
        retVal.first = true;
    }
    return retVal;
}

//only call if there is a guaranteed path(no matter how long)
std::vector<sf::Vector2i> getFinishedPath(const DistanceMap& distanceMap,
    sf::Vector2i startPos, sf::Vector2i endPos) {

    unsigned int stepsTakenSoFar = 0;

    std::vector<sf::Vector2i> retVal;
    retVal.push_back(endPos);

    sf::Vector2i currentPos = endPos;
    sf::Vector2i lowestDistanceAdjacent;
    //checks if the algorithm reaches the desired position and also caps the agorithm from running infinitely when its impossible that it needs to
    while (currentPos != startPos && stepsTakenSoFar < (distanceMap.second.size()*distanceMap.second[0].size())) {
        
        float lastDistance = std::numeric_limits<float>::max();
        for (int i = currentPos.x - 1; i <= (currentPos.x + 1); i++)
        {
            if (i < 0 || i>= distanceMap.second.size()) {
                continue;
            }
            for (int o = currentPos.y - 1; o <= (currentPos.y + 1); o++)
            {
                //making the "cross" pattern
                if ((i == (currentPos.x - 1) && o == (currentPos.y - 1))
                    || (i == (currentPos.x + 1) && o == (currentPos.y - 1))
                    || (i == (currentPos.x - 1) && o == (currentPos.y + 1))
                    || (i == (currentPos.x + 1) && o == (currentPos.y + 1))) {
                    continue;
                }
                if (o< 0 || o >= distanceMap.second[0].size()) {
                    continue;
                }
                sf::Vector2i ioV2iRep = sf::Vector2i(i, o);
                if (ioV2iRep == currentPos) {
                    continue;
                }
                if (distanceMap.second[i][o] == 0 && ioV2iRep != startPos) {
                    continue;
                }
                
                if (distanceMap.second[i][o] < lastDistance) {
                    lowestDistanceAdjacent = ioV2iRep;
                    lastDistance = distanceMap.second[i][o];
                }

            }
        }
        stepsTakenSoFar++;
        currentPos = lowestDistanceAdjacent;
        if (lowestDistanceAdjacent != startPos) {
            retVal.push_back(lowestDistanceAdjacent);
        }
        
    }

    return retVal;
}

//returns the shortest path in tile coordinates from startPos to endPos
std::vector<sf::Vector2i> getShortestPathTo(sf::Vector2i startPos, 
    sf::Vector2i endPos, const std::vector<std::vector<Tile>> &tileMapRep) {

    DistanceMap distanceMap = getDistancesFromStartPoint(startPos, endPos, tileMapRep);
    //displayTextTileMapRep(tileMapRep);
    //displayDistanceMap(distanceMap);

    //if the pathfinding finds paths, follow the shortest one
    if (distanceMap.first && startPos != endPos) {
        return getFinishedPath(distanceMap,startPos, endPos);
    }
    //if the pathfinding finds no path, stay idle
    return std::vector<sf::Vector2i>(1,startPos);
}

//validates the current path and if it should be recalculated with a proper pathfinding algorithm
bool EnemyAI::m_checkIfPathfindingIsNeccesary(const sf::Vector2f &movePos, const std::vector<std::vector<Tile>> &tileMapRep) const
{
    sf::Vector2i controlledUnitTilePos = Tile::convertGlobalPosToTilePos(m_controlledUnit->getBody()->first);
    sf::Vector2i chosenTargetTilePos = Tile::convertGlobalPosToTilePos(movePos);
    if (controlledUnitTilePos.x < tileMapRep.size() && controlledUnitTilePos.y < tileMapRep.size()
        && controlledUnitTilePos.x >= 0 && controlledUnitTilePos.y >= 0
        && chosenTargetTilePos.x >= 0 && chosenTargetTilePos.y >= 0
        && chosenTargetTilePos != controlledUnitTilePos) {
        if (controlledUnitTilePos.x < tileMapRep[0].size() && chosenTargetTilePos.y < tileMapRep[0].size()) {
            sf::Vector2i searchBoxStart = sf::Vector2i(std::min(controlledUnitTilePos.x, chosenTargetTilePos.x), std::min(controlledUnitTilePos.y, chosenTargetTilePos.y));
            sf::Vector2i searchBoxEnd = sf::Vector2i(std::max(controlledUnitTilePos.x, chosenTargetTilePos.x), std::max(controlledUnitTilePos.y, chosenTargetTilePos.y));
            for (size_t i = searchBoxStart.x; i <= searchBoxEnd.x; i++)
            {
                for (size_t o = searchBoxStart.y; o <= searchBoxEnd.y; o++)
                {
                    if (minimum_distance(m_controlledUnit->getBody()->first, m_moveToPos, Tile::convertTilePosToGlobalPos(sf::Vector2i(i,o))) <= Tile::tileSize) {
                        //checks if the tile has ANY collideable geometry(should
                        //improve this later to check if the tile is PASSABLE!!!)
                        if (!tileIsPassable(sf::Vector2u(i,o), tileMapRep, m_controlledUnit->getBody()->second)) {
                            return true;
                        }
                    }
                }
            }
        }

    }
    return false;
}

EnemyAI::EnemyAI(const EnemyAI &other)
{
    m_controlledUnit = new unit(*other.m_controlledUnit);
    m_controlledWeapon = new Weapon(*other.m_controlledWeapon);
    m_controlledWeapon->setWieldingUnit(m_controlledUnit);
    m_followingDistance = other.m_followingDistance;
    m_pythonFunc = other.m_pythonFunc;
    
}

EnemyAI& EnemyAI::operator=(const EnemyAI&other)
{
    if (this != &other) {
        *this = EnemyAI(other);
    }
    // TODO: insert return statement here
    return *this;
}

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

void EnemyAI::onUnitKilled(std::vector<unit*> targets)
{
	for (size_t i = 0; i < targets.size(); i++)
	{
		//the current damage instance is deleted as the enemy is dead
		targets[i]->cModule.applyDamageInstance(m_controlledUnit->getID(), 0, 0, true);
	}
}

void EnemyAI::update(float timeDelta, std::vector<unit*> targets, MessageBus *gameBus, TileMap &gameTileMap)
{
    if (targets.size() == 0) {
        throw std::invalid_argument("there must be atleast one target");
    }

	//without_gil no_gil;
	with_gil gil;

    //anything you need to do once per enemyAI, where that thing happens 
    //after the object is ready to update for the first time
    if (m_firstUpdate) {
        //m_controlledUnitLastTilePos = Tile::convertGlobalPosToTilePos(m_controlledUnit->getBody()->first);
        m_firstUpdate = false;
    }

	m_currentAITimeToInterval += timeDelta;
	//code for more than one target
    unit* chosenTarget = targets[0];
    //local scope for tempLowestDistanceToAnyTarget
    {
        float tempLowestDistanceToAnyTarget = std::numeric_limits<float>::max();
        for (size_t i = 1; i < targets.size(); i++)
        {
            float tempCurrentDistanceToTarget = vectorDistance(targets[i]->getBody()->first, m_controlledUnit->getBody()->first);
            if (tempCurrentDistanceToTarget<tempLowestDistanceToAnyTarget) {
                tempLowestDistanceToAnyTarget = tempCurrentDistanceToTarget;
                chosenTarget = targets[i];
            }
        }
    }
	


    
    if(m_pythonFunc != boost::python::api::object()&& (m_currentAITimeToInterval >= m_AIUpdateInterval)){
		m_currentAITimeToInterval = 0;
        sf::Vector2f headPos = sf::Vector2f(0,0);
        float headRadius = 0;
        bool hasHead = false;
        if(m_followingHead != nullptr){
            headPos = m_followingHead->getBody()->first;
            headRadius = m_followingHead->getBody()->second;
            hasHead = true;
        }

        boost::python::list listElement;
        listElement.append(chosenTarget->getBody()->first.x);
        listElement.append(chosenTarget->getBody()->first.y);
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
			
			retVal = m_pythonFunc(playerPoss, chosenTarget->getBody()->second,
                                                    chosenTarget->getVelocity().x, chosenTarget->getVelocity().y,
                                                    m_controlledWeapon->canFire(),
                                                    m_controlledUnit->getBody()->first.x, m_controlledUnit->getBody()->first.y, m_controlledUnit->getBody()->second,
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
        shootPos.y = boost::python::extract<float>(retVal[4]);
        shootsNow = boost::python::extract<bool>(retVal[5]);
    }else if(m_followingHead != nullptr){
        m_moveToPos = m_followingHead->getBody()->first + m_followingDistance;
    }
    //the m_moveToPos after pathfinding correction
    sf::Vector2f newMoveToPos = m_moveToPos;
    //pathfinding algortihm:
    {
        std::vector<std::vector<Tile>> *tileMapRep = gameTileMap.getAllTiles();
        //here the proper pathfinding algorithm runs if needed
        if (m_checkIfPathfindingIsNeccesary(m_moveToPos, *tileMapRep)) {
            /*
            lineMessage tempLineMessage;
            tempLineMessage.messageType = lineMessageType::debugMessage;
            tempLineMessage.message = "path finding is neccesary!";
            tempLineMessage.messageColor = sf::Color(rand()%255,rand()%255,rand()%255);
            InGameMessages::getInstance().addLine(tempLineMessage);
            */
            sf::Vector2i controlledUnitTilePos = Tile::convertGlobalPosToTilePos(m_controlledUnit->getBody()->first);
            /*
            //this alligns the AI with its current tile so that it doesnt bump into corners(deprecated)
            if (vectorDistance(m_controlledUnit->getBody()->first, Tile::convertTilePosToGlobalPos(controlledUnitTilePos))>(Tile::tileSize/30)) {
                controlledUnitTilePos = m_controlledUnitLastTilePos;
            }
            else {
                m_controlledUnitLastTilePos = controlledUnitTilePos;
            }
            */
            std::vector<sf::Vector2i> shortestPath = getShortestPathTo(controlledUnitTilePos,
                Tile::convertGlobalPosToTilePos(m_moveToPos), *tileMapRep);
            newMoveToPos = Tile::convertTilePosToGlobalPos(shortestPath.back());
            
            /*
            for (size_t i = 0; i < shortestPath.size(); i++)
            {
                std::cout << shortestPath[i].x << ", "<< shortestPath[i].y << std::endl;
            }
            */
            //system("pause");
        }
    }
    //

    //

    if(shootsNow){
        m_controlledWeapon->fire(m_controlledUnit->getUnitVecTo(shootPos));
    }

	//testing "zombie melee bite" code
	for (size_t i = 0; i < targets.size(); i++)
	{
		if (vectorDistance(targets[i]->getBody()->first, m_controlledUnit->getBody()->first)
			<
			targets[i]->getBody()->second + m_controlledUnit->getBody()->second + m_controlledUnit->cModule.meleeRange) {
			//a damage instance is created so that the enemy that is within range can damage the player after a given duration
			targets[i]->cModule.applyDamageInstance(m_controlledUnit->getID(), m_controlledUnit->cModule.damage, m_controlledUnit->cModule.meleeDelay, false);
		}
		else {
			//the current damage instance is deleted as the enemy is too far away to do any damage
			targets[i]->cModule.applyDamageInstance(m_controlledUnit->getID(), 0, 0, true);
		}
	}
	

    m_controlledUnit->cModule.moveSpeed *= addedMoveSpeed;
    if(vectorDistance(newMoveToPos, m_controlledUnit->getBody()->first) > 2){
        m_controlledUnit->applyMoveSpeed(m_controlledUnit->getUnitVecTo(newMoveToPos));
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
