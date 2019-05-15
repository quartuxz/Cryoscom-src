//#pragma once

#ifndef GAMEMAIN_H_EXISTS
#define GAMEMAIN_H_EXISTS
#include <vector>
#include <SFML/Graphics.hpp>
#include <map>
#include <stack>
#include <mutex>
#include "Menu.h"
#include "unitManager.h"
#include "MessageBus.h"
#include "Logger.h"
#include "PlayerInventory.h"
#include "InputManager.h"



class HUDMenu : public Menu {
private:
    ToolTip m_availablePotions;
    void m_updatePotions(unsigned int);

public:
	using Menu::Menu;


    void onDraw(bool, sf::Vector2f)override;
	void createStaticMenuLayout()override;

	void update(updateEvent)override;
};

class GameMain : public MessagingComponent
{
private:
	std::vector<std::string> m_gameLevels;
	unsigned int m_activeLevel = 0;
	sf::Clock m_clock;
	sf::RenderWindow *m_window;
	std::map<std::string ,Menu*> m_gameMenus;
	PlayerInventory* m_playerInventory;
	std::string m_activeMenu = "mainMenu";
	UnitManager * m_currentLevel = new UnitManager();

	InputManager m_inputManager;

	std::recursive_mutex m_dataLock;
	//MockMutex m_dataLock;

	Logger m_gameLogger;
	MessageBus m_gameBus;
	void pv_processMessage(const MessageData&, MessageBus*)override;

	std::stack<updateEvent> m_updateEvents;

	bool m_isPaused = false;

	Menu *m_openGameMenu = nullptr;

	std::string m_progressionFile;

	sf::Vector2f m_viewDisplacement = sf::Vector2f(0, 0);

	bool m_levelJump = false;

	void m_create(std::vector<std::string>);

	void m_setActiveLevel();
	void m_loadLevelGameMainBits(std::string);

	bool m_HUDActive = true;

public:
	GameMain(std::vector <std::string>);

	//creates a GameMain instance from a file containing all the game level file names
	GameMain(std::string);
	//
	//TODO: create a file format that holds the directories of all menu definition files, together with the name of each individual one- this will be passed to the function--DONE
	//TODO: create a file format for the menuItems, specifying the behaviour of the each, the text, the size(in multiples of the scren size), the position(as the latter),
	//the parameters for the behaviour and the texture file name--DONE
	void createUIFromFile(std::string);

	//create HUD, make it non-scriptable but hardcoded instead(HUD is part of essence of the game)

    void updateUI(std::string, sf::Vector2i, bool);

	void onProgramEnd();

	void setProgressionFile(std::string);

	void onProgramStart();
	void spawnWindow(std::string fontFile = "good times rg.ttf");
	void startLevel(unsigned int activeLevel = 0);

	void gameLoop();
	//returns wether the game is paused or not
	bool runOnce(float, sf::Vector2i, bool mouseClicked = false);
	~GameMain();
};

#endif
