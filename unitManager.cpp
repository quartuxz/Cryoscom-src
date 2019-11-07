#include "unitManager.h"
#include <iostream>
#include <map>
#include <mutex>
#include "Animator.h"
#include "tileMap.h"
#include "cryoscom_defsAndUtils.h"




const float UnitManager::m_itemPickupCooldown = 2;

#if MULTITHREADED_SCRIPTING_AND_MESSAGING
std::recursive_mutex mutexLock;
#else
MockMutex mutexLock;
#endif

void MarketMenu::createStaticMenuLayout()
{
	//

	MenuItem tempMenuItem = createMenuItem(sf::Vector2f(0.15, 0.05), sf::Vector2f(0.1, 0.05));
	behaviourParameters tempBParam;
	tempBParam.behaviourName = removesGearPiece;
	tempBParam.gearPieceRemoved = "chestPiece";
	tempBParam.goldCost = m_playerGearCost[tempBParam.gearPieceRemoved];
	tempMenuItem.addBehviourParameters(tempBParam);
	addMenuItem(tempMenuItem);

	tempMenuItem = createMenuItem(sf::Vector2f(0.35, 0.05), sf::Vector2f(0.1, 0.05));
	tempBParam.behaviourName = removesGearPiece;
	tempBParam.gearPieceRemoved = "helmet";
	tempBParam.goldCost = m_playerGearCost[tempBParam.gearPieceRemoved];
	tempMenuItem.addBehviourParameters(tempBParam);
	addMenuItem(tempMenuItem);

	tempMenuItem = createMenuItem(sf::Vector2f(0.55, 0.05), sf::Vector2f(0.1, 0.05));
	tempBParam.behaviourName = removesGearPiece;
	tempBParam.gearPieceRemoved = "boots";
	tempBParam.goldCost = m_playerGearCost[tempBParam.gearPieceRemoved];
	tempMenuItem.addBehviourParameters(tempBParam);
	addMenuItem(tempMenuItem);

	tempMenuItem = createMenuItem(sf::Vector2f(0.75, 0.05), sf::Vector2f(0.1, 0.05));
	tempBParam.behaviourName = removesGearPiece;
	tempBParam.gearPieceRemoved = "weapon";
	tempBParam.goldCost = m_playerGearCost[tempBParam.gearPieceRemoved];
	tempMenuItem.addBehviourParameters(tempBParam);
	addMenuItem(tempMenuItem);

	tempMenuItem = createMenuItem(sf::Vector2f(0.15, 0.7),sf::Vector2f(0.1, 0.1));
	tempBParam.behaviourName = buysItem;
	tempBParam.itemBought = "healthPotion";
	tempBParam.goldCost = 10;
	tempMenuItem.addBehviourParameters(tempBParam);
	addMenuItem(tempMenuItem);
}

void MarketMenu::onDraw(bool beforeDraw, sf::Vector2f viewDisplacement)
{
	if (beforeDraw) {
		for (size_t i = 0; i < m_playerGear.size(); i++)
		{
			sf::Vector2f initialPosition = m_playerGear[i].getPosition();
			m_playerGear[i].setPosition(initialPosition + viewDisplacement);
			m_playerGear[i].draw(*m_window);
			m_playerGear[i].setPosition(initialPosition);
		}
	}
}

//deprecated
//void addToolTips(gearPiece playerGear)
//{
//	mutexLock.lock();
//	m_playerGear.clear();
//
//	AnimatorSprite tempASprite;
//	tempASprite.textureID = Animator::getInstance().getTextureID("tooltip.png");
//	tempASprite.originToCenter = false;
//	tempASprite.scale = 6;
//
//	sf::Vector2f scaleFactor =  getScaleFactor(sf::Vector2f(Animator::getInstance().getTexture(Animator::getInstance().getTextureID("tooltip.png"))->getSize()) * tempASprite.scale, sf::Vector2f(0.2, 0.4));
//
//	ToolTip tempToolTip;
//
//
//
//	tempToolTip.setScale(scaleFactor.x);
//	tempToolTip.setTexture(tempASprite);
//
//	auto gearItems = playerGear.getGearItems();
//	std::map<std::string, GearPiece>::iterator it;
//	for (it = gearItems.begin(); it != gearItems.end(); it++){
//		tempToolTip.clearText();
//		tempToolTip.makeTooltipForGear(it->second);
//		m_playerGearCost[it->first] = it->second.goldValue;
//		if (it->first == "chestPiece") {
//			tempToolTip.setPosition(getPixelCoordinate(sf::Vector2f(0.1, 0.15)));
//		}
//
//		if (it->first == "helmet") {
//			tempToolTip.setPosition(getPixelCoordinate(sf::Vector2f(0.3, 0.15)));
//		}
//
//		if (it->first == "boots") {
//			tempToolTip.setPosition(getPixelCoordinate(sf::Vector2f(0.5, 0.15)));
//		}
//
//		if (it->first == "weapon") {
//			tempToolTip.setPosition(getPixelCoordinate(sf::Vector2f(0.7, 0.15)));
//		}
//		m_playerGear.push_back(tempToolTip);
//	}
//	mutexLock.unlock();
//}

void MarketMenu::update(updateEvent)
{
}

void UnitManager::m_updateAIWeapons(std::vector<Weapon*> AIWeapons)
{
	mutexLock.lock();
	for (size_t i = 0; i < AIWeapons.size(); i++)
	{
		std::list<Bullet*> AIWeaponBullets = AIWeapons[i]->getBullets();
		std::list<Bullet*>::iterator it2;
		for (it2 = AIWeaponBullets.begin(); it2 != AIWeaponBullets.end(); ++it2)
		{
			if (vectorDistance((*it2)->getBody()->getBody()[0].first, m_player->getBody()[0].first) < renderDistance) {
				if (m_map->collides((*it2)->getBody())) {

					(*it2)->onWallHit();
				}
			}
		}
	}
	mutexLock.unlock();
}

void UnitManager::m_updateAI(float timeDelta, std::vector<unit*> AIsSubset, std::vector<unit*> nonPlayerUnits)
{
	mutexLock.lock();
	for (size_t i = 0; i < AIsSubset.size(); i++)
	{
		AIsSubset[i]->update(timeDelta, nonPlayerUnits);
		m_map->update(AIsSubset[i]);
	}
	mutexLock.unlock();
}

void UnitManager::m_updatePlayer(float timeDelta ,std::vector<unit*> nonPlayerUnits)
{
	mutexLock.lock();
	m_player->update(timeDelta, nonPlayerUnits);
	m_map->update(m_player);
	mutexLock.unlock();
}

void UnitManager::pv_parseStep(std::vector<std::string> tokens)
{
	mutexLock.lock();
	if (tokens[0] == "spawnPoint") {
		std::vector<std::string> enemyFileName;
		for (size_t i = 4; i < tokens.size(); i++)
		{
			enemyFileName.push_back(tokens[i]);
		}

		spawnPoint(ma_deserialize_int(tokens[1]), sf::Vector2f(ma_deserialize_float(tokens[2]), ma_deserialize_float(tokens[3])), enemyFileName);

	}

	else if (tokens[0] == "scaleFactor") {
		m_levelScale = std::atof(tokens[1].c_str());
	}
	else if (tokens[0] == "requireTileSizeOrScale") {
		if (ma_deserialize_uint(tokens[1]) != TileMap::tileSize) {
			m_levelScale = TileMap::tileSize/ ma_deserialize_uint(tokens[1]);

		}
	}

	else if (tokens[0] == "wall") {
		//std::cout << tokens.size() << std::endl;
		//system("pause");
		m_map->addWall(sf::Vector2f(std::atof(tokens[1].c_str()) * m_levelScale, std::atof(tokens[2].c_str()) * m_levelScale), (sf::Vector2f(std::atof(tokens[3].c_str()) * m_levelScale, std::atof(tokens[4].c_str()) * m_levelScale)));
	}
	else if (tokens[0] == "namedWall") {
		m_map->addNamedWall(sf::Vector2f(std::atof(tokens[1].c_str()) * m_levelScale, std::atof(tokens[2].c_str()) * m_levelScale), (sf::Vector2f(std::atof(tokens[3].c_str()) * m_levelScale, std::atof(tokens[4].c_str()) * m_levelScale)), tokens[5]);
	}
	else if (tokens[0] == "namedAnimatorSprite") {
		AnimatorSprite tempASprite;
		tempASprite.textureID = Animator::getInstance().getTextureID(tokens[2]);
		tempASprite.position = sf::Vector2f(std::atof(tokens[3].c_str())*m_levelScale, std::atof(tokens[4].c_str())*m_levelScale);
		tempASprite.scale = std::atof(tokens[5].c_str())*m_levelScale;
		tempASprite.drawLayer = std::atoi(tokens[6].c_str());
		Animator::getInstance().getNamedAnimatorSprites()->insert(std::make_pair(tokens[1], std::move(tempASprite)));
	}
	else if (tokens[0] == "unit") {
		EnemyAI* tempAI = new EnemyAI();
		std::vector<std::pair<sf::Vector2f, float>> unitBody;
		unitBody.push_back(std::pair<sf::Vector2f, float>(sf::Vector2f(std::atof(tokens[1].c_str()) * m_levelScale, std::atof(tokens[2].c_str())) * m_levelScale, std::atof(tokens[3].c_str())));
		unit* tempUnit = new unit(unitBody);
		tempUnit->typeOfUnit = defaultType;
		tempUnit->setWeight(std::atof(tokens[4].c_str()));

		tempUnit->cModule.moveSpeed = 100;
		tempUnit->cModule.damage = 5;

		tempAI->setUnit(tempUnit);
		m_AIs.push_back(tempAI);
		if (tokens.size() > 5) {
			AnimatorSprite tempASprtie;
			tempASprtie.textureID = Animator::getInstance().getTextureID(tokens[5]);

			tempAI->getUnit()->setAnimatorSprite(tempASprtie);
		}
		if (tokens.size() > 6) {
			tempAI->createFromFile(tokens[6]);
		}
		if (tokens.size() > 7) {
			tempAI->setHead(m_AIs[(m_AIs.size() - 1) - std::atoi(tokens[7].c_str())]->getUnit(), sf::Vector2f(std::atof(tokens[8].c_str()), std::atof(tokens[9].c_str())));
		}

	}else if (tokens[0] == "interactable") {
		
		if (tokens[1] == "messageSender") {
			interactable tempInteractable;
			tempInteractable.isMessaging = true;
			tempInteractable.sentMessage = tokens[4];
			tempInteractable.position = sf::Vector2f(std::atof(tokens[2].c_str()) * m_levelScale, std::atof(tokens[3].c_str())*m_levelScale);
			tempInteractable.activationRadius = std::atof(tokens[5].c_str())*m_levelScale;
			addInteractable(tempInteractable);
		}

	}
	else if (tokens[0] == "tooltip") {
		ToolTip* toolTip = new ToolTip();
		AnimatorSprite tempASprite;
		if (tokens[1] == "") {
			tempASprite.isActive = false;
		}
		else {
			tempASprite.textureID = Animator::getInstance().getTextureID(tokens[1]);
		}

		toolTip->setTexture(tempASprite);
		toolTip->setPosition(sf::Vector2f(std::atof(tokens[2].c_str()) * m_levelScale, std::atof(tokens[3].c_str()) * m_levelScale));
		toolTip->setScale(std::atof(tokens[4].c_str()));
		sf::Text tempTooltipText;
		tempTooltipText.setFont(*ToolTip::getFont());
		tempTooltipText.setString(tokens[5]);
		tempTooltipText.setScale(sf::Vector2f(0.1, 0.1));
		tempTooltipText.setFillColor(sf::Color::Green);
		toolTip->addText(tempTooltipText);
		toolTip->readDistance = ma_deserialize_uint(tokens[6])*m_levelScale;
		addToolTip(toolTip);

	}
	else
	if (tokens[0] == "player") {

		m_player = makePlayer(sf::Vector2f(ma_deserialize_float(tokens[1]), ma_deserialize_float(tokens[2])), ma_deserialize_float(tokens[3]), ma_deserialize_float(tokens[4]));

		

	}
	else
	if (tokens[0] == "bulletTexture") {
		AnimatorSprite tempASprtie;
		tempASprtie.textureID = Animator::getInstance().getTextureID(tokens[1]);

		if (m_playerWeapon == nullptr) {
			m_playerWeapon = new Weapon(m_player);
		}

		for (size_t i = 2; i < tokens.size(); i++)
		{
			if (std::atoi(tokens[i].c_str()) == -1) {
				m_playerWeapon->setbulletAnimatorTex(tempASprtie);
			}
			else {
				if (m_AIs[(m_AIs.size() - 1) - std::atoi(tokens[i].c_str())]->getWeapon() == nullptr) {
					m_AIs[(m_AIs.size() - 1) - std::atoi(tokens[i].c_str())]->setWeapon(new Weapon(m_AIs[(m_AIs.size() - 1) - std::atoi(tokens[i].c_str())]->getUnit()));
				}
				m_AIs[(m_AIs.size() - 1) - std::atoi(tokens[i].c_str())]->getWeapon()->setbulletAnimatorTex(tempASprtie);
			}
		}


	}else
	if (tokens[0] == "worldTextureTile") {
		sf::Texture* tempTexture = new sf::Texture();
		tempTexture->loadFromFile(tokens[7]);
		m_toDeleteTextures.push_back(tempTexture);
		sf::Sprite tempSprite;
		tempSprite.setTexture(*tempTexture);
		if (std::atof(tokens[1].c_str()) < 0) {
			tempSprite.setOrigin(sf::Vector2f(tempSprite.getLocalBounds().width / 2, tempSprite.getLocalBounds().height / 2));
		}
		else {
			tempSprite.setOrigin(sf::Vector2f(std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str())));
		}

		tempSprite.setPosition(sf::Vector2f(std::atof(tokens[3].c_str()), std::atof(tokens[4].c_str())));
		tempSprite.setRotation(std::atof(tokens[5].c_str()));
		tempSprite.scale(std::atof(tokens[6].c_str()) * m_levelScale, std::atof(tokens[6].c_str()) * m_levelScale);
		m_worldTextures.push_back(tempSprite);
	}
	else if (tokens[0] == "") {

	}
	else if (tokens[0] == "loadLootTable") {
		m_lootTable.createFromFile(tokens[1]);
	}
	else if (tokens[0] == "tileMap") {
		
		m_tileMap.createFromFile(tokens[1]);
		setTileMap(m_tileMap);
		
		
	}
	mutexLock.unlock();
}

void UnitManager::addHealthPotions(unsigned int amount)
{
	mutexLock.lock();
	m_healthPotions += amount;
	mutexLock.unlock();
}

void UnitManager::addGold(unsigned int amount)
{
	mutexLock.lock();
	m_gold += amount;
	mutexLock.unlock();
}

bool UnitManager::subtractGold(int amount)
{
	mutexLock.lock();
	if (m_gold >= amount) {
		m_gold -= amount;
		mutexLock.unlock();
		return true;
	}
	mutexLock.unlock();
	return false;
}

bool UnitManager::removeGearPiece(std::string gearPieceName)
{
	mutexLock.lock();
	//bool retVal = m_gear.removeGearPiece(gearPieceName);
	bool retVal = false;
	mutexLock.unlock();
	return retVal;
}

void UnitManager::drinkPotion()
{
	mutexLock.lock();
	if (m_healthPotions > 0) {
		m_player->cModule.hitpoints += 100;
		m_healthPotions--;
	}
	mutexLock.unlock();
}

void UnitManager::setToolTipsShow(bool toolTipsShown)
{
	mutexLock.lock();
	m_showToolTips = toolTipsShown;
	mutexLock.unlock();
}

bool UnitManager::areToolTipsShown() const
{
	return m_showToolTips;
}

void UnitManager::removeToolTip(unsigned int ID)
{
	mutexLock.lock();
	m_toolTips[ID].second = false;
	mutexLock.unlock();
}

void UnitManager::placeItem(const inventoryItem& invItem, sf::Vector2f pos)
{
	m_mapItems.push_back(std::make_pair(pos, invItem));
}

inventoryItem UnitManager::pickUpItem()
{
	for (size_t i = 0; i < m_mapItems.size(); i++)
	{
		if (vectorDistance(m_mapItems[i].first, m_player->getBody()[0].first) < m_pickUpDistance) {
			auto retVal = std::move(m_mapItems[i].second);
			m_mapItems.erase(m_mapItems.begin() + i);
			return retVal;
		}
	}
	return inventoryItem();
}


//--eventually will be deprecated as well:
void UnitManager::setProgressionFile(std::string fileName)
{
	mutexLock.lock();
	m_progressionFileName = fileName;
	mutexLock.unlock();
}

void UnitManager::saveGearProgression()
{
	mutexLock.lock();
	std::ofstream editFile(m_progressionFileName, std::ios::app);
	editFile << "bag;" << m_healthPotions << ";" << m_gold << ";" << std::endl;
	editFile.close();
	mutexLock.unlock();
}
//--

endLevelTypes UnitManager::hasLevelEnded() const
{
	return m_levelHasEnded;
}

Map* UnitManager::getMap() const
{
	return m_map;
}

void UnitManager::startLevel()
{
	mutexLock.lock();
	//assignPlayerGear();
	GearPiece tempGearPiece;
	tempGearPiece.tex.textureID = Animator::getInstance().getTextureID("chestPiece.png");
	tempGearPiece.tex.textureID = 2;
	tempGearPiece;
	//m_lootTable.addLootEntry(tempGearPiece);
	m_needsAnUpdate = true;
	mutexLock.unlock();
}

void UnitManager::endLevel()
{
	mutexLock.lock();
	Animator::getInstance().clearNamedAnimatorSprites();
	m_needsAnUpdate = false;
	mutexLock.unlock();
}

float UnitManager::getDistanceToPlayer(sf::Vector2f pos) const
{
	mutexLock.lock();
	float retVal = vectorDistance(pos, m_player->getBody()[0].first);
	mutexLock.unlock();
	return retVal;
}

void UnitManager::addInteractable(Menu *menu, sf::Vector2f pos)
{
	mutexLock.lock();
	interactable tempInteractable;
	tempInteractable.menu = menu;
	tempInteractable.position = pos;
	m_interactables.push_back(tempInteractable);
	mutexLock.unlock();
}

void UnitManager::addInteractable(const interactable &tempInteractable)
{
	m_interactables.push_back(tempInteractable);
}

Menu * UnitManager::interact(MessageBus *bus)
{
	mutexLock.lock();
	for (size_t i = 0; i < m_interactables.size(); i++)
	{
		if (vectorDistance(m_interactables[i].position, m_player->getBody()[0].first) < m_interactables[i].activationRadius) {
			
			if (m_interactables[i].isMessaging) {
				MessageData* tempMessage = new MessageData();
				tempMessage->createFrom(decomposedData().createFrom(m_interactables[i].sentMessage));
				bus->addMessage(tempMessage);
				mutexLock.unlock();
				return nullptr;
			}
            MarketMenu *tempMarketMenu = dynamic_cast<MarketMenu*>(m_interactables[i].menu);
            if(tempMarketMenu != nullptr){
                //tempMarketMenu->addToolTips(m_gear);
            }
			mutexLock.unlock();
			return m_interactables[i].menu;
		}
	}
	mutexLock.unlock();
	return nullptr;
}

UnitManager::UnitManager()
{
	mutexLock.lock();
	m_map = new Map();
	mutexLock.unlock();
	
}

UnitManager::UnitManager(Map *map):
	m_map(map)
{
}

void UnitManager::setMap(Map *map)
{
	mutexLock.lock();
	m_map = map;
	mutexLock.unlock();
}

void UnitManager::setPlayerWeapon(Weapon *weapon)
{
	mutexLock.lock();
	m_playerWeapon = weapon;
	mutexLock.unlock();
}

unsigned int UnitManager::getHealthPotions()const{
    return m_healthPotions;
}

unit *UnitManager::getClosestAIUnit()const{

    return m_closestAIUnit;
}

//TODO: create a layer render system, to combine effects over the player/bullet etc, integrate with worldTextures and animation
void UnitManager::update(float timeDelta, sf::RenderWindow &window, MessageBus *gameBus)
{

	mutexLock.lock();
    std::vector<unit*> killedUnits;
	if (m_player->isDead) {
		m_levelHasEnded = playerDied;
		mutexLock.unlock();
		return;
	}

	for (size_t i = 0; i < m_worldTextures.size(); i++)
	{
		window.draw(m_worldTextures[i]);
	}
	//std::cout << "bullet speed: " << m_player->Amodule->bulletSpeed << ", bullet duration: " << m_player->Amodule->bulletDuration << ", fire rate: " << m_player->Amodule->fireRate << std::endl;

	std::vector<std::thread*> threads;
	int numberOfThreads = 1;

	std::vector<unit*> nonPlayerUnits;
	std::vector<Weapon*> AIWeapons;
	std::vector<std::vector<unit*>> nonPlayerUnitsThreaded;
	nonPlayerUnitsThreaded.resize(numberOfThreads);
	std::vector<std::vector<Weapon*>> AIWeaponsThreaded;
	AIWeaponsThreaded.resize(numberOfThreads);

    float minDistance = std::numeric_limits<float>::max();


	for (size_t i = 0; i < m_AIs.size(); i++)
	{
	    float tempDistance = vectorDistance(m_AIs[i]->getUnit()->getBody()[0].first, m_player->getBody()[0].first);
		if ( (tempDistance < renderDistance) && !m_AIs[i]->getUnit()->isDead) {
            if(minDistance > tempDistance){
                m_closestAIUnit = m_AIs[i]->getUnit();
                minDistance = tempDistance;
            }
			m_AIs[i]->update(timeDelta, std::vector<unit*>(1, m_player), gameBus);

			m_AIs[i]->drawBullets(window);
			//window.draw(m_AIs[i]->getUnit()->getTexture());
			nonPlayerUnitsThreaded[i%numberOfThreads].push_back(m_AIs[i]->getUnit());
			nonPlayerUnits.push_back(m_AIs[i]->getUnit());
			AIWeaponsThreaded[i%numberOfThreads].push_back(m_AIs[i]->getWeapon());
			AIWeapons.push_back(m_AIs[i]->getWeapon());
		}else if(m_AIs[i]->getUnit()->recentlyDead && m_AIs[i]->getUnit()->isDead){
            m_AIs[i]->getUnit()->recentlyDead = false;
            killedUnits.push_back(m_AIs[i]->getUnit());
        }
	}

	//std::thread tempThread(&UnitManager::m_updateAI, this, timeDelta, nonPlayerUnitsThreaded[0], nonPlayerUnits);

	for (size_t i = 0; i < numberOfThreads; i++)
	{
		//threads.push_back(new std::thread(&UnitManager::m_updateAI, this, timeDelta, nonPlayerUnitsThreaded[i], nonPlayerUnits));
		//threads.push_back(new std::thread(&UnitManager::m_updateAIWeapons, this, AIWeaponsThreaded[i]));
	}



	for (size_t i = 0; i < threads.size(); i++)
	{
		//threads[i]->join();
	}

	m_updateAI(timeDelta, nonPlayerUnits, nonPlayerUnits);
    for (size_t i = 0; i < m_AIs.size(); i++){

    }
	m_updateAIWeapons(AIWeapons);
	m_updatePlayer(timeDelta, nonPlayerUnits);

    m_playerWeapon->update(timeDelta, nonPlayerUnits);


	if (!m_lootTable.isEmpty()) {
		for (size_t i = 0; i < killedUnits.size(); i++)
		{
			inventoryItem tempInvItem;
			tempInvItem.itemType = gearPieceType;
			tempInvItem.simpleRep = m_lootTable.getRandomGearPiece();
			tempInvItem.setTexturesFromGearPiece();
			placeItem(tempInvItem, killedUnits[i]->lastLastPos);
		}
	}
	std::list<Bullet*> playerWeaponBullets = m_playerWeapon->getBullets();
	std::list<Bullet*>::iterator it2;
	for (it2 = playerWeaponBullets.begin(); it2 != playerWeaponBullets.end(); ++it2)
	{
		if (vectorDistance((*it2)->getBody()->getBody()[0].first, m_player->getBody()[0].first) < renderDistance) {
			if (m_map->collides((*it2)->getBody())) {
				(*it2)->onWallHit();
			}
		}
	}

	//tempThread.join();

	for (size_t i = 0; i < threads.size(); i++)
	{
		//threads[i]->join();
	}


	for (size_t i = 0; i < nonPlayerUnits.size(); i++)
	{
		Animator::getInstance().addOneFrameSprite(nonPlayerUnits[i]->getAnimatorSprite());
		//window.draw(nonPlayerUnits[i]->getTexture());
	}
	//window.draw(m_player->getTexture());

	//drawing map items
	Animator::getInstance().addOneFrameSprite(m_player->getAnimatorSprite());
	m_playerWeapon->displayToWindow(window);
	for (size_t i = 0; i < m_mapItems.size(); i++)
	{

		m_mapItems[i].second.itemASprite.position = m_mapItems[i].first;
		if (vectorDistance(m_mapItems[i].first, m_player->getBody()[0].first) < m_toolTipReadDistance) {
			m_mapItems[i].second.itemToolTip.setPosition(m_mapItems[i].first);
			m_mapItems[i].second.itemToolTip.draw(*Animator::getInstance().getWindow());
		}
		//m_mapGearPieces[i].second.tex.textureID = Animator::getInstance().getTextureID("chestpiece.png");
		//m_mapGearPieces[i].second.tex.drawLayer = 0;
		//m_mapGearPieces[i].second.tex.position = m_mapGearPieces[i].first;
		//m_mapGearPieces[i].second.tex.scale = 0.5;
		Animator::getInstance().addOneFrameSprite(m_mapItems[i].second.itemASprite);
	}

	if (m_showToolTips) {
		for (size_t i = 0; i < m_toolTips.size(); i++)
		{
			if (m_toolTips[i].second && (vectorDistance(m_toolTips[i].first->getPosition(), m_player->getBody()[0].first) < m_toolTips[i].first->readDistance)) {
				Animator::getInstance().addOneFrameSprite(*m_toolTips[i].first);
			}
		}
	}

	m_tileMap.addAllSprites();

	mutexLock.unlock();
	////map updating

}

unsigned int UnitManager::addToolTip(ToolTip *toolTip)
{
	mutexLock.lock();
	m_toolTips.push_back(std::pair<ToolTip*, bool>(toolTip, true));
	size_t retVal = m_toolTips.size() - 1;
	mutexLock.unlock();
	return retVal;
}

Weapon * UnitManager::getWeapon()const
{
	return m_playerWeapon;
}

void UnitManager::setWeapon(Weapon *weapon)
{
	m_playerWeapon = weapon;
	m_playerWeapon->setWieldingUnit(m_player);
}

unit * UnitManager::getPlayer()const
{
	return m_player;
}

//deprecated gear code:
//void UnitManager::placeGearOnMap(sf::Vector2f pos, GearPiece gearPiece)
//{
//	mutexLock.lock();
//	gearPiece.tex.rotation = rand() % 360;
//	m_mapGearPieces.push_back(std::pair<sf::Vector2f, GearPiece>(pos, gearPiece));
//	ToolTip *tempToolTip = new ToolTip();
//	AnimatorSprite tempAnimatorSprite;
//	tempAnimatorSprite.textureID = Animator::getInstance().getTextureID("tooltip.png");
//	tempAnimatorSprite.scale = 5;
//	tempAnimatorSprite.originToCenter = false;
//	tempToolTip->setTexture(tempAnimatorSprite);
//	tempToolTip->makeTooltipForGear(gearPiece);
//	tempToolTip->setPosition(pos);
//	m_itemToolTipID[gearPiece] = addToolTip(tempToolTip);
//	mutexLock.unlock();
//	//m_toolTips.back().first.position.y += 20;
//}
//
//void UnitManager::pickUpGear()
//{
//	mutexLock.lock();
//	if (m_itemPickupClock.getElapsedTime().asSeconds() > m_itemPickupCooldown) {
//		for (size_t i = 0; i < m_mapGearPieces.size(); i++)
//		{
//			//std::cout << "size of gear: " << m_mapGearPieces.size() << std::endl;
//			if (vectorDistance(m_mapGearPieces[i].first, m_player->getBody()[0].first) < m_pickUpDistance) {
//				m_mapGearPieces[i].second.tex.rotation = 0;
//				//addPlayerGear(m_mapGearPieces[i].second);
//				m_toolTips[m_itemToolTipID[m_mapGearPieces[i].second]].second = false;
//				m_mapGearPieces.erase(m_mapGearPieces.begin() + i);
//				break;
//			}
//		}
//		m_itemPickupClock.restart();
//	}
//	mutexLock.unlock();
//}

TileMap UnitManager::getTileMap() const
{
	return m_tileMap;
}

void UnitManager::setTileMap(const TileMap &tileMap)
{
	m_tileMap = tileMap;
	std::vector<Wall> tempTileWalls = m_tileMap.getWallRep();
	for (size_t i = 0; i < tempTileWalls.size(); i++)
	{
		m_map->addWall(tempTileWalls[i]);
	}
}

void UnitManager::addAI(EnemyAI *newAI)
{
	mutexLock.lock();
	m_AIs.push_back(newAI);
	mutexLock.unlock();
}

void UnitManager::spawnPoint(int enemies, sf::Vector2f pos, std::vector<std::string> enemyFileName)
{
	constexpr unsigned int displacementScale = 1;


	for (size_t i = 0; i < enemies; i++)
	{
		size_t lastEnemyUnitID = m_AIs.size();
		createFromFile(enemyFileName[rand() % enemyFileName.size()]);
		for (size_t o = lastEnemyUnitID; o < m_AIs.size(); o++)
		{
			//((rand() % (displacementScale * 10)) / displacementScale) * ((rand() % 2) ? -1 : 1)
			const float microDisplacementX = (rand() % enemies);
			const float microDisplacementY = (rand() % enemies);
			m_AIs[o]->getUnit()->move(sf::Vector2f(pos.x + microDisplacementX, pos.y + microDisplacementY));
			for (size_t p = 0; p < m_AIs.size(); p++) {
				m_AIs[o]->getUnit()->collideOne(m_AIs[p]->getUnit());
			}

		}
	}
}

void UnitManager::setPlayer(unit *playerUnit)
{
	mutexLock.lock();
	m_player = playerUnit;
	mutexLock.unlock();
}

float UnitManager::getLevelScale()const {
	return m_levelScale;
}

void UnitManager::setLevelScale(float val) {
	mutexLock.lock();
	m_levelScale = val;
	mutexLock.unlock();
}

UnitManager::~UnitManager()
{
	mutexLock.lock();
	delete m_map;
	for (size_t i = 0; i < m_AIs.size(); i++)
	{
		delete m_AIs[i];
	}
	for (size_t i = 0; i < m_toDeleteTextures.size(); i++)
	{
		delete m_toDeleteTextures[i];
	}
	for (size_t i = 0; i < m_interactables.size(); i++)
	{
		if (m_interactables[i].menu != nullptr) {
			delete m_interactables[i].menu;
		}
		
	}
	for (size_t i = 0; i < m_toolTips.size(); i++)
	{
		delete m_toolTips[i].first;
	}
	mutexLock.unlock();
}

unit *makePlayer(sf::Vector2f pos, float radius, float weight)
{
	std::vector<std::pair<sf::Vector2f, float>> unitBody;
	unitBody.push_back(std::pair<sf::Vector2f, float>(pos, radius));
	unit *retVal = new unit(unitBody);
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
