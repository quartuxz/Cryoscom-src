#include "GameMain.h"
#include <iostream>
#include <sstream>
#include "Animator.h"
#include "ToolTip.h"
#include "Serializable.h"



#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.






void GameMain::m_loadLevelGameMainBits(std::string fileName) {

	std::string line;
	std::ifstream fileRead(fileName);
	if (fileRead.is_open()) {
		while (getline(fileRead, line)) {
			std::vector<std::string> tokens;

			std::string tempLine;

			if (tokens.empty()) {
				std::string delimiter = ";";

				size_t pos = 0;
				pos = line.find(delimiter);
				while (pos != std::string::npos) {
					tokens.push_back(line.substr(0, pos));
					line.erase(0, pos + delimiter.length());
					pos = line.find(delimiter);
				}
			}

			if (tokens.empty()) {
				continue;
			}
			else if (tokens[0] == "//") {
				continue;
			}
			else if (tokens[0] == "interactable") {
				if (tokens[1] == "market") {
					MarketMenu* tempMarketMenu = new MarketMenu(m_window);
					tempMarketMenu->createStaticMenuLayout();
					m_currentLevel->addInteractable(tempMarketMenu, sf::Vector2f(std::atof(tokens[2].c_str()) * m_currentLevel->getLevelScale(), std::atof(tokens[3].c_str()) * m_currentLevel->getLevelScale()));
				}
				

			}
			else if (tokens[0] == "gameScript") {
				GameScript* tempMC = new GameScript();
				tempMC->createFromFile(tokens[1]);
				m_gameBus.addMessagingComponent(tempMC, true);
			}
		}
	}
	fileRead.close();
}

void GameMain::m_setActiveLevel()
{
	m_gameBus = MessageBus();
	m_gameBus.addMessagingComponent(this);
	m_gameBus.addMessagingComponent(&m_gameLogger);

	m_currentLevel->endLevel();
	delete m_currentLevel;
	m_currentLevel = new UnitManager();

	m_currentLevel->createFromFile(m_gameLevels[m_activeLevel]);
	m_currentLevel->setProgressionFile(m_progressionFile);
	//m_currentLevel->loadGearProgression();
	m_loadLevelGameMainBits(m_gameLevels[m_activeLevel]);

	MessageData *mData = new MessageData();
	mData->messageType = "levelStart";
	mData->messageContents.push_back(decomposedData().setName("activeLevelName").setType("std::string").addData(m_gameLevels[m_activeLevel])
		.addChildrenObject(decomposedData().setType("float").setName("levelScale").addData(ma_serialize(m_currentLevel->getLevelScale()))));
	m_gameBus.addMessage(mData);
	//m_currentLevel->getPlayer()->cModule.isMelee = false;
}

void GameMain::spawnWindow(std::string fontFile)
{

	//m_window = new sf::RenderWindow();
	//m_window->create(sf::VideoMode(1200, 1000), "SFML works!");
	//m_window->setFramerateLimit(60);

	m_inputManager.launchWindowIOThread();
	m_window = m_inputManager.getWindow();
	m_window->setActive(true);

	m_playerInventory = new PlayerInventory(m_window);
	m_gameMenus["inventory"] = m_playerInventory;
	m_gameMenus["inventory"]->createStaticMenuLayout();
	GearPiece tempGearPiece;
	tempGearPiece.cModule = combatModule(true);
	tempGearPiece.cModule.moveSpeed = 500;
	AnimatorSprite gearASprite;
	gearASprite.textureID = Animator::getInstance().getTextureID("chestpiece.png");
	tempGearPiece.tex = gearASprite;
	tempGearPiece.type = chestPiece;
	m_playerInventory->addItemToInventory(tempGearPiece);
	m_playerInventory->addAmmo(100);
	gearASprite.textureID = Animator::getInstance().getTextureID("player.png");
	tempGearPiece.tex = gearASprite;
	tempGearPiece.cModule.moveSpeed = -100;
	m_playerInventory->addItemToInventory(tempGearPiece);
	//Animator::getInstance().setWindow(&m_window);
	//Animator::getInstance().addTexture("player.png");
	m_HUDMenu = new HUDMenu(m_window);
	m_gameMenus["HUD"] = m_HUDMenu;
	m_gameMenus["HUD"]->createStaticMenuLayout();
	Animator::getInstance().setWindow(m_window);
	ToolTip::init(fontFile);
}

void GameMain::startLevel(unsigned int activeLevel)
{

	MessageData* mData = new MessageData();
	mData->messageType = "levelEnd";
	mData->messageContents.push_back(decomposedData().setName("activeLevelName").setType("string").addData(m_gameLevels[m_activeLevel]));
	m_gameBus.addMessage(mData);



	m_activeLevel = activeLevel;
	m_setActiveLevel();
	MarketMenu *tempMenu = new MarketMenu(m_window);
	tempMenu->createStaticMenuLayout();
	m_currentLevel->addInteractable(tempMenu, sf::Vector2f(0,0));
	m_currentLevel->startLevel();
}

void GameMain::m_create(std::vector<std::string> gameLevels)
{
	srand(static_cast <unsigned> (time(0)));
	m_gameLevels = gameLevels;
	m_gameBus.addMessagingComponent(this);
}

GameMain::GameMain(std::vector<std::string> gameLevels)
{
	m_create(gameLevels);
}

GameMain::GameMain(std::string fileName)
{
std::string line;
std::ifstream fileRead(fileName);
std::vector<std::string> gameLevelNames;
if (fileRead.is_open()) {
	while (getline(fileRead, line)) {
		std::vector<std::string> tokens;
		std::string tempLine = line;
		if (tokens.empty()) {
			std::string delimiter = ";";

			size_t pos = 0;
			pos = line.find(delimiter);
			while (pos != std::string::npos) {
				tokens.push_back(line.substr(0, pos));
				line.erase(0, pos + delimiter.length());
				pos = line.find(delimiter);
			}
		}

		if (tokens.empty()) {

		}
		else if (tokens[0] == "//") {
			continue;
		}
		gameLevelNames.push_back(tempLine);
	}
	m_create(gameLevelNames);
	//std::cout << "READ FILE, SIZE: " << m_gameLevels.size() << std::endl;
	//system("pause");
	fileRead.close();
}
}

void GameMain::createUIFromFile(std::string fileName)
{
	std::string line;
	std::ifstream fileRead(fileName);
	if (fileRead.is_open()) {
		while (getline(fileRead, line)) {
			std::vector<std::string> tokens;

			if (tokens.empty()) {
				std::string delimiter = ";";

				size_t pos = 0;
				pos = line.find(delimiter);
				while (pos != std::string::npos) {
					tokens.push_back(line.substr(0, pos));
					line.erase(0, pos + delimiter.length());
					pos = line.find(delimiter);
				}
			}

			if (tokens.empty()) {
				continue;
			}
			else if (tokens[0] == "//") {
				continue;
			}


			m_gameMenus[tokens[1]] = new Menu(m_window);
			m_gameMenus[tokens[1]]->createMenuFromFile(tokens[0]);
		}
		fileRead.close();
	}
}

void GameMain::onProgramStart() {
	MessageData* mData = new MessageData();
	mData->messageType = "onProgramStart";
	//mData->messageContents.push_back(decomposedData().setName("activeLevelName").setType("string").addData(m_gameLevels[m_activeLevel]));
	m_gameBus.addMessage(mData);
	m_gameBus.notify();

}

void GameMain::onProgramEnd()
{
	MessageData* mData = new MessageData();
	mData->messageType = "beforeProgramEnd";
	//mData->messageContents.push_back(decomposedData().setName("activeLevelName").setType("string").addData(m_gameLevels[m_activeLevel]));
	m_gameBus.addMessage(mData);
	mData = new MessageData();
	mData->messageType = "onProgramEnd";
	m_gameBus.addMessage(mData);
	m_gameBus.notify();
	m_gameBus.onDestroy();


	//m_currentLevel->saveGearProgression();
	Animator::getInstance().draw();

	fstream outFile;
	outFile.open("progression.txt");
	outFile << m_playerInventory->serialize().serialize() << std::endl;
	outFile.close();

	m_window->close();
}

void GameMain::setProgressionFile(std::string fileName)
{
	m_progressionFile = fileName;
}

//TODO: fix possible crashes with namedWalls
void GameMain::pv_processMessage(const MessageData & tempMessage, MessageBus * bus) {
	//std::cout << tempMessage.messageContents[0].name << std::endl;

	if (tempMessage.messageType == "editNamedWall") {
		Map* currentLevelmap = m_currentLevel->getMap();
		
		currentLevelmap->getWalls()->operator[](currentLevelmap->getWallNames()[tempMessage.messageContents[0].data[0]]).isActive = ma_deserialize_uint(tempMessage.messageContents[0].data[1]);
	}
	else if(tempMessage.messageType == "addNamedWall"){
		m_currentLevel->getMap()->addNamedWall(
			sf::Vector2f(ma_deserialize_float(tempMessage.messageContents[0].data[1]), ma_deserialize_float(tempMessage.messageContents[0].data[2])),
			sf::Vector2f(ma_deserialize_float(tempMessage.messageContents[0].data[2]), ma_deserialize_float(tempMessage.messageContents[0].data[3])),
			tempMessage.messageContents[0].data[0]);
	}


	else if (tempMessage.messageType == "editNamedAnimatorSprite") {
		auto tempAnimatorSprites = Animator::getInstance().getNamedAnimatorSprites();
		auto mapItr = tempAnimatorSprites->find(tempMessage.messageContents[0].data[0]);
		AnimatorSprite tempASprite;
		if (mapItr != tempAnimatorSprites->end()) {
			tempASprite = mapItr->second;
		}
		
		if (tempMessage.messageContents[0].data[1] != "") {
			
			tempASprite.textureID = Animator::getInstance().getTextureID(tempMessage.messageContents[0].data[1]);
		}
		if (tempMessage.messageContents[0].data[2] != "") {
			tempASprite.isActive = std::atoi(tempMessage.messageContents[0].data[2].c_str());
		}

		if (tempMessage.messageContents[0].data[3] != "" && tempMessage.messageContents[0].data[4] != "") {
			tempASprite.position = sf::Vector2f(std::atof(tempMessage.messageContents[0].childrenObjects[0].data[3].c_str()), std::atof(tempMessage.messageContents[0].childrenObjects[0].data[4].c_str()));
		}
		
		tempAnimatorSprites->operator[](tempMessage.messageContents[0].data[0]) = std::move(tempASprite);

	}
	else if (tempMessage.messageType == "requestUniqueIDs") {
		MessageData* retMessage = new MessageData();
		retMessage->intendedReceiverID = tempMessage.senderID;
		retMessage->senderID = p_uniqueID;
		retMessage->messageType = "sendUniqueIDs";
		size_t numOfUUIDS = std::atoi(tempMessage.messageContents[0].data[0].c_str());
		boost::uuids::random_generator generator;
		for (size_t i = 0; i < numOfUUIDS; i++)
		{
			boost::uuids::uuid uuid = generator();
			decomposedData tempDData;
			tempDData.type = "boost::lexical_cast<std::string>(boost::uuids::uuid())";
			tempDData.name = "requestedUUID";
			tempDData.addData(boost::lexical_cast<std::string>(uuid));
			retMessage->addMessageContents(tempDData);
		}
		retMessage->whoReceives = justIntended;
		bus->addMessage(retMessage);
	}
	else if (tempMessage.messageType == "updateMap") {  
		m_loadLevelGameMainBits(tempMessage.messageContents[0].data[0]);
		m_currentLevel->createFromFile(tempMessage.messageContents[0].data[0]);
	}
	else if (tempMessage.messageType == "openMenu") {
		if (tempMessage.messageContents[0].data[1] != "inventory") {
			Menu* tempMenu = new Menu(m_window);
			tempMenu->createMenuFromFile(tempMessage.messageContents[0].data[0]);
			m_activeMenu = tempMessage.messageContents[0].data[1];
			delete m_gameMenus[m_activeMenu];
			m_gameMenus[m_activeMenu] = tempMenu;
		}
	}
	else if (tempMessage.messageType == "spawnItemOnPlayer") {
		inventoryItem tempInvItem;
		tempInvItem.createFrom(tempMessage.messageContents[0]);
		m_currentLevel->placeItem(tempInvItem, m_currentLevel->getPlayer()->getBody()[0].first);
	}

	 else if (tempMessage.messageType == "displayDecal") {
		AnimatorSprite tempASprite;
		tempASprite.createFrom(tempMessage.messageContents[0]);
		Animator::getInstance().addDecal(tempASprite);
	}

	else if (tempMessage.messageType == "applyEffectOnPlayer") {
		effect tempEffect;
		if (tempMessage.messageContents[0].data[0] == "damageOverTime") {
			tempEffect.eType = damageOverTimeEffect;
		}
		else if (tempMessage.messageContents[0].data[0] == "staminaRegen") {
			tempEffect.eType = staminaRegenEffect;
		}
		else if (tempMessage.messageContents[0].data[0] == "root") {
			tempEffect.eType = rootEffect;
		}
		tempEffect.amount = std::atof(tempMessage.messageContents[0].data[1].c_str());
		tempEffect.duration = std::atof(tempMessage.messageContents[0].data[2].c_str());
		m_currentLevel->getPlayer()->cModule.effects.push_back(tempEffect);
	}
}

skillParam makeDashSkill(sf::Vector2f dir) {
	skillParam retSParam;
	retSParam.sType = dash;
	retSParam.castDelay = 0;
	retSParam.dirUnitVec = dir;
	retSParam.staminaCost = 20;
	return retSParam;
}

void GameMain::gameLoop()
{
	MarketMenu tempMenu =  MarketMenu(m_window);
	tempMenu.createStaticMenuLayout();
	m_window->setFramerateLimit(120);

	sf::Vector2f lastPlayerPos = m_currentLevel->getPlayer()->getPosition();
	sf::View view;

	view.setSize(sf::Vector2f(m_window->getSize()));
	view.setCenter(sf::Vector2f(m_window->getSize().x / 2, m_window->getSize().y / 2));

	view.move(-sf::Vector2f(m_window->getSize().x / 2, m_window->getSize().y / 2));
	view.move(m_currentLevel->getPlayer()->getBody()[0].first);

	m_viewDisplacement += -sf::Vector2f(m_window->getSize().x / 2, m_window->getSize().y / 2);
	m_viewDisplacement += m_currentLevel->getPlayer()->getBody()[0].first;
	sf::Vector2i mousePosition;

	sf::Time currentTime;

	sf::Image lastWindowState;

	float lastDash = 0;

	sf::Texture screenShot;
	screenShot.create(m_window->getSize().x, m_window->getSize().y);

	inventoryItem mapItem;
	mapItem.itemASprite.textureID = Animator::getInstance().getTextureID("chestpiece.png");
	mapItem.itemType = gearPieceType;
	mapItem.simpleRep.type = chestPiece;
	mapItem.simpleRep.tex = mapItem.itemASprite;
	mapItem.itemToolTip.makeTooltipForGear(mapItem.simpleRep);
	m_currentLevel->placeItem(mapItem, sf::Vector2f(300,300));

	skillParam *HUDSParam = new skillParam();
	HUDSParam->skillIcon.textureID = Animator::getInstance().getTextureID("Psi_Ability-Pyrokinesis.png");
	m_HUDMenu->addSkill(HUDSParam);
	//m_window = m_inputManager.getWindow();

	m_window->setFramerateLimit(160);

	std::cout << "mainLoop" << std::endl;
	while (m_window->isOpen())
	{
		m_gameBus.startFrame(currentTime.asSeconds());
		if (m_gameBus.canMessage()) {
			MessageData* playerData = new MessageData();
			playerData->messageType = "playerData";
			playerData->messageContents.push_back(decomposedData().setName("playerPosition").setType("sf::Vector2f").addData(std::to_string(m_currentLevel->getPlayer()->getBody()[0].first.x)).addData(std::to_string(m_currentLevel->getPlayer()->getBody()[0].first.y)));
			m_gameBus.addMessage(playerData);
		}
		

		mousePosition = sf::Mouse::getPosition(*m_window);
		bool mouseClick = false;

		sf::Vector2f dist = sf::Vector2f(mousePosition + sf::Vector2i(m_viewDisplacement)) - (m_currentLevel->getPlayer()->getBody()[0].first);
		lastPlayerPos = m_currentLevel->getPlayer()->getPosition();
		float mag = sqrt(pow(dist.x, 2) + pow(dist.y, 2));
		//std::cout << newUnit.getBody()[0].first.x << ", " << newUnit.getBody()[0].first.y << std::endl;
		sf::Vector2f mouseUnitVec = sf::Vector2f(dist.x / mag, dist.y / mag);


		if (m_isPaused) {
			currentTime = sf::Time::Zero;
			m_clock.restart();
		}
		else {
			currentTime = m_clock.restart();
		}
		lastDash += currentTime.asSeconds();



		InputManager::InputEvent inputEvent;
		while ((inputEvent = m_inputManager.getEvent()).InputEventType != InputManager::noInputEvent) {
			if (inputEvent.InputEventType <= InputManager::hotbar5 && inputEvent.InputEventType >= InputManager::hotbar1) {
				skillParam *tempSParam = m_HUDMenu->getSkill(inputEvent.InputEventType - InputManager::hotbar1);
				if (tempSParam != nullptr) {
					skillParam* actualSkillParam = new skillParam(*tempSParam);
					m_currentLevel->getWeapon()->addSkillToQueue(actualSkillParam);
				}
				
			}
			
			skillParam* tempSParam;
			Menu* tempMenu;
			inventoryItem pickedUpItem;
			switch (inputEvent.InputEventType)
			{
			case InputManager::dashBackward:
				if (lastDash >= 2) {
					lastDash = 0;
					tempSParam = new skillParam();
					*tempSParam = makeDashSkill(rotate90(sf::Vector2f(0,0), rotate90(sf::Vector2f(0, 0), mouseUnitVec)));
					m_currentLevel->getWeapon()->addSkillToQueue(tempSParam);
				}
				break;
			case InputManager::dashForward:
				if (lastDash >= 2) {
					lastDash = 0;
					tempSParam = new skillParam();
					*tempSParam = makeDashSkill(mouseUnitVec);
					m_currentLevel->getWeapon()->addSkillToQueue(tempSParam);
				}
				break;
			case InputManager::dashRight:
				lastDash = 0;
				tempSParam = new skillParam();
				if (mouseUnitVec.y > 0) {
					*tempSParam = makeDashSkill(rotate90(sf::Vector2f(0, 0), rotate90(sf::Vector2f(0, 0), rotate90(sf::Vector2f(0, 0), mouseUnitVec))));
				}
				else {
					*tempSParam = makeDashSkill(rotate90(sf::Vector2f(0, 0), mouseUnitVec));
				}
				m_currentLevel->getWeapon()->addSkillToQueue(tempSParam);
				break;
			case InputManager::dashLeft:
				if (lastDash >= 2) {
					lastDash = 0;
					tempSParam = new skillParam();
					if (mouseUnitVec.y < 0) {
						*tempSParam = makeDashSkill(rotate90(sf::Vector2f(0, 0), rotate90(sf::Vector2f(0, 0), rotate90(sf::Vector2f(0, 0), mouseUnitVec))));
					}
					else {
						*tempSParam = makeDashSkill(rotate90(sf::Vector2f(0, 0), mouseUnitVec));
					}
					m_currentLevel->getWeapon()->addSkillToQueue(tempSParam);
				}
				break;
			case InputManager::pickUpKeyPressed:
				pickedUpItem = m_currentLevel->pickUpItem();
				if (pickedUpItem.itemType != emptySlot) {
					m_playerInventory->addItemToInventory(pickedUpItem);
				}
				break;
			case InputManager::dash:
				if (lastDash >= 2) {
					lastDash = 0;
					tempSParam = new skillParam();
					*tempSParam = makeDashSkill(mouseUnitVec);
					m_currentLevel->getWeapon()->addSkillToQueue(tempSParam);
				}
				break;
			case InputManager::showToolTipsKeyPressed:
				m_currentLevel->setToolTipsShow(!m_currentLevel->areToolTipsShown());
				break;
			case InputManager::interactKeyPressed:
				if (m_activeMenu != "mainMenu") {
					tempMenu = m_currentLevel->interact(&m_gameBus);
					if (tempMenu != nullptr) {
						if (m_activeMenu == "interactable") {
							m_activeMenu = "";
						}
						else {
							m_activeMenu = "interactable";
							m_gameMenus[m_activeMenu] = tempMenu;
							//lastWindowState = m_window->capture();

						}
					}
				}
				break;
			case InputManager::inventoryKeyPressed:
				if (m_activeMenu != "inventory") {
					screenShot.update(*m_window);
					m_activeMenu = "inventory";
				}
				else {
					m_activeMenu = "";
				}
				break;
			case InputManager::generalKeyReleased:
				m_currentLevel->getPlayer()->stopMovement();
				break;
			case InputManager::windowClosed:
				m_inputManager.stop();
				m_window->close();
				break;
			case InputManager::escapeKeyPressed:
				if (m_activeMenu != "mainMenu") {
					screenShot.update(*m_window);
					m_activeMenu = "mainMenu";
					//lastWindowState = m_window->capture();
				}
				else {
					m_activeMenu = "";
				}
				break;
			default:
				break;
			}
		}

		if (m_inputManager.isInputEventActive(InputManager::moveForward)) {
			//std::cout << "w" << std::endl;
			m_currentLevel->getPlayer()->applyMoveSpeed(mouseUnitVec);
		}
		else
		if (m_inputManager.isInputEventActive(InputManager::moveLeft)) {
			//std::cout << "a" << std::endl;
			if (mouseUnitVec.y > 0) {
				m_currentLevel->getPlayer()->applyMoveSpeed(rotate90(sf::Vector2f(0, 0), mouseUnitVec));
			}
			else {
				m_currentLevel->getPlayer()->applyMoveSpeed(rotate90(sf::Vector2f(0, 0), rotate90(sf::Vector2f(0, 0), rotate90(sf::Vector2f(0, 0), mouseUnitVec))));
			}
		}
		else
		if (m_inputManager.isInputEventActive(InputManager::moveBackward)) {
			//std::cout << "s" << std::endl;
			m_currentLevel->getPlayer()->applyMoveSpeed(sf::Vector2f(-mouseUnitVec.x, -mouseUnitVec.y));
		}
		else
		if (m_inputManager.isInputEventActive(InputManager::moveRight)) {
			//std::cout << "d" << std::endl;
			if (mouseUnitVec.y > 0) {
				m_currentLevel->getPlayer()->applyMoveSpeed(rotate90(sf::Vector2f(0, 0), rotate90(sf::Vector2f(0, 0), rotate90(sf::Vector2f(0, 0), mouseUnitVec))));
			}
			else {
				m_currentLevel->getPlayer()->applyMoveSpeed(rotate90(sf::Vector2f(0, 0), mouseUnitVec));
			}

			
		}


		if (m_inputManager.isInputEventActive(InputManager::shoot)) {
			if (!m_isPaused) {
				if (m_currentLevel->getPlayer()->cModule.isMelee) {
					m_currentLevel->getWeapon()->fire(sf::Vector2f(mousePosition) + m_viewDisplacement);
				}
				else {
					if (m_currentLevel->getWeapon()->fire(mouseUnitVec)) {
						m_playerInventory->removeAmmo(1);
					}
				}

			}
			mouseClick = true;
		}

		if (m_inputManager.isInputEventActive(InputManager::useItem)) {
			//UnitManagerSingleton::getInstance().requestUpdate();
			m_currentLevel->drinkPotion();
		}



		//DEPRECATED INPUT MANAGEMENT:

		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		//	//std::cout << "gearPickedUP!" << std::endl;


		//}
  ////      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		////    unit *tempUnit = m_currentLevel->getClosestAIUnit();
		////    if(tempUnit != nullptr){
  ////              m_currentLevel->getWeapon()->fire(m_currentLevel->getPlayer()->getUnitVecTo(tempUnit->getBody()[0].first));
		////    }

		////}
		//
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && lastDash >= 2) {
		//	lastDash = 0;
		//	skillParam *tempSParam = new skillParam();
		//	tempSParam->sType = dash;
		//	tempSParam->castDelay = 0;
		//	tempSParam->dirUnitVec = mouseUnitVec;
		//	tempSParam->staminaCost = 50;
		//	m_currentLevel->getWeapon()->addSkillToQueue(tempSParam);
		//}

		//else
		//{
		//	//unitManager.getPlayer()->stopMovement();
		//}


		//if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		//	//UnitManagerSingleton::getInstance().requestUpdate();
		//	
		//}





		//sf::Event event;
		//while (m_window->pollEvent(event))
		//{

		//	if (event.type == sf::Event::Closed) {
		//		m_inputManager.stop();
		//		m_window->close();
		//	}
		//		
		//	if (event.type == sf::Event::KeyPressed) {
		//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		//			//std::cout << "gearPickedUP!" << std::endl;
		//			MessageData* playerData = new MessageData();
		//			playerData->messageType = "interactionKeyPressed";
		//			playerData->messageContents.push_back(decomposedData().setName("playerPosition").setType("sf::Vector2f").addData(std::to_string(m_currentLevel->getPlayer()->getBody()[0].first.x)).addData(std::to_string(m_currentLevel->getPlayer()->getBody()[0].first.y)));
		//			//playerData->messageContents.push_back();
		//			m_gameBus.addMessage(playerData);

		//			//m_currentLevel->pickUpGear();
		//		}if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		//			if (m_activeMenu != "mainMenu") {
		//				screenShot.update(*m_window);
		//				m_activeMenu = "mainMenu";
		//				//lastWindowState = m_window->capture();
		//			}
		//			else {
		//				m_activeMenu = "";
		//			}

		//		}
		//		
		//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
		//			if (m_activeMenu != "inventory") {
		//				screenShot.update(*m_window);
		//				m_activeMenu = "inventory";
		//			}
		//			else {
		//				m_activeMenu = "";
		//			}
		//		}
		//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
		//			if (m_activeMenu != "mainMenu") {
		//				Menu *tempMenu = m_currentLevel->interact(&m_gameBus);
		//				if (tempMenu != nullptr) {
		//					if (m_activeMenu == "interactable") {
		//						m_activeMenu = "";
		//					}
		//					else {
		//						m_activeMenu = "interactable";
		//						m_gameMenus[m_activeMenu] = tempMenu;
		//						//lastWindowState = m_window->capture();

		//					}
		//				}
		//			}

		//		}
		//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
		//			//std::cout << "gearPickedUP!" << std::endl;
		//			m_currentLevel->setToolTipsShow(!m_currentLevel->areToolTipsShown());
		//		}
		//	}

  //          if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
  //              //UnitManagerSingleton::getInstance().requestUpdate();
  //              m_currentLevel->drinkPotion();
  //          }


		//	if (event.type == sf::Event::MouseButtonPressed) {

		//	}

		//	if (event.type == sf::Event::KeyReleased) {
		//		m_currentLevel->getPlayer()->stopMovement();
		//	}
		//}
        std::cout << 1/currentTime.asSeconds() << std::endl;

		//m_gameBus.notify();

		m_window->clear(sf::Color::White);

		if (m_isPaused) {

			sf::Sprite capturedBackground;
			capturedBackground.move(m_viewDisplacement);
			capturedBackground.setTexture(screenShot);
			sf::Texture tempTexture;
			tempTexture.loadFromImage(lastWindowState);
			sf::Sprite tempSprite;
			tempSprite.move(m_viewDisplacement);
			tempSprite.setTexture(tempTexture);
			sf::RectangleShape tempRect;
			tempRect.setFillColor(sf::Color(100,100,100, 100));
			tempRect.setSize(sf::Vector2f(m_window->getSize()));
			tempRect.move(m_viewDisplacement);
			m_window->draw(tempSprite);
			m_window->draw(capturedBackground);
			m_window->draw(tempRect);
		}
		
		if (!m_isPaused) {
			Animator::getInstance().update(currentTime.asSeconds());
			Animator::getInstance().draw();
		}

		m_isPaused = runOnce(currentTime.asSeconds(), mousePosition, m_inputManager);



		view.move(m_currentLevel->getPlayer()->getPosition() - lastPlayerPos);
		m_viewDisplacement += (m_currentLevel->getPlayer()->getPosition() - lastPlayerPos);
		m_window->setView(view);

		//AnimatorSprite tempSprite = AnimatorSprite::AnimatorSprite(0);
		//tempSprite.scale = 10;
		//tempSprite.rotation = 60;

		//Animator::getInstance().addOneFrameSprite(tempSprite);

		//std::cout << currentTime.asSeconds() << std::endl;

		



		//m_gameMenus["HUD"]->draw(m_viewDisplacement);


		m_window->display();
		m_gameBus.endFrame();
	}
	

}

void GameMain::updateUI(std::string UIName, sf::Vector2i mousePos, const InputManager& inputs){
    updateEvent tempEvent;
    tempEvent.updateEventType = lostLife;
    tempEvent.currentLife = m_currentLevel->getPlayer()->cModule.hitpoints;
    tempEvent.maxLife = m_currentLevel->getPlayer()->cModule.hitpointCap;
    m_gameMenus[UIName]->update(tempEvent);
    tempEvent.updateEventType = lostPotions;
    tempEvent.availablePotions = m_currentLevel->getHealthPotions();
    m_gameMenus[UIName]->update(tempEvent);

    m_gameMenus[UIName]->draw(m_viewDisplacement);

    std::vector<behaviourParameters> tempBehaviourParam = m_gameMenus[UIName]->onClick(sf::Vector2f(mousePos), inputs, &m_gameBus);
    for (size_t i = 0; i < tempBehaviourParam.size(); i++)
    {
		MessageData* tempMessageData;
		inventoryItem tempInvItem;
        switch (tempBehaviourParam[i].behaviourName)
        {
        case opensMenu:
            m_activeMenu = tempBehaviourParam[i].menuTo;

            break;
        case resumesGame:
            m_activeMenu = "";
            break;
		case sendsMessage:
			tempMessageData = new MessageData();
			*tempMessageData = tempBehaviourParam[i].messageData;
			m_gameBus.addMessage(tempMessageData);
			break;
        case buysItem:
            if (tempBehaviourParam[i].itemBought == "healthPotion" && inputs.isInputEventActive(InputManager::shoot)) {
                if (m_currentLevel->subtractGold(tempBehaviourParam[i].goldCost)) {
                    m_currentLevel->addHealthPotions(1);
                }
            }
            break;
        case removesGearPiece:
			tempBehaviourParam[i].gearPiece.unequipGearPiece(&m_currentLevel->getPlayer()->cModule);
			std::cout << "unequips:" << tempBehaviourParam[i].gearPiece.cModule.moveSpeed << std::endl;
			break;
		case equipsGearPiece:
			tempBehaviourParam[i].gearPiece.equipGearPiece(&m_currentLevel->getPlayer()->cModule);
			std::cout << "equips:" << tempBehaviourParam[i].gearPiece.cModule.moveSpeed << std::endl;
			break;
		case dropsItemOnPlayer:
			m_currentLevel->placeItem(tempBehaviourParam[i].itemDropped, m_currentLevel->getPlayer()->getBody()[0].first);
			break;
        default:
            break;
        }
    }
}
//runs one frame of the game, updating timewise game and menus(HUD or whatever other menu is active)
bool GameMain::runOnce(float timeDelta, sf::Vector2i mousePos, const InputManager &inputs)
{
	if (m_activeMenu.empty()) {
		m_currentLevel->update(timeDelta, *m_window, &m_gameBus);
		if (m_currentLevel->hasLevelEnded() == playerDied) {
			m_currentLevel->setProgressionFile(m_progressionFile);
			m_currentLevel->saveGearProgression();
			delete m_currentLevel;
			m_currentLevel = new UnitManager();
			if (m_levelJump) {
				m_setActiveLevel();
				m_levelJump = false;
			}
			else {
				m_setActiveLevel();
			}

		}
		if (m_HUDActive) {
            updateUI("HUD", mousePos, inputs);
		}

		return false;
	}
	else {

		updateUI(m_activeMenu, mousePos, inputs);
		return true;
	}
}

//old UI update code
//m_gameMenus[m_activeMenu]->draw(m_viewDisplacement);
//		std::vector<behaviourParameters> tempBehaviourParam = m_gameMenus[m_activeMenu]->onClick(sf::Vector2f(mousePos), mouseClicked);
//		for (size_t i = 0; i < tempBehaviourParam.size(); i++)
//		{
//			switch (tempBehaviourParam[i].behaviourName)
//			{
//			case opensMenu:
//                m_activeMenu = tempBehaviourParam[i].menuTo;
//
//				break;
//			case resumesGame:
//                m_activeMenu = "";
//				break;
//			case buysItem:
//				if (tempBehaviourParam[i].itemBought == "healthPotion" && mouseClicked) {
//					if (m_currentLevel->subtractGold(tempBehaviourParam[i].goldCost)) {
//						m_currentLevel->addHealthPotions(1);
//					}
//				}
//				break;
//			case removesGearPiece:
//				if (mouseClicked) {
//					if (m_currentLevel->removeGearPiece(tempBehaviourParam[i].gearPieceRemoved)) {
//						m_currentLevel->addGold(tempBehaviourParam[i].goldCost);
//						m_currentLevel->assignPlayerGear(false);
//					}
//				}
//			default:
//				break;
//			}
//		}


GameMain::~GameMain()
{

	for (auto const& x : m_gameMenus)
	{
		delete x.second;
	}
	delete m_currentLevel;
}
