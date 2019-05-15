#pragma once
#include <SFML/Graphics.hpp>
#include "unit.h"
#include "Map.h"
#include "Weapon.h"
#include "EnemyAI.h"
#include "Gear.h"
#include <queue>
#include <list>
#include <fstream>
#include <thread>
#include "Menu.h"
#include "ToolTip.h"
#include "MessageBus.h"
#include "Parsable.h"
#include "Gear.h"



enum endLevelTypes {
	levelActive, playerDied, goalReached
};

class MarketMenu : public Menu {
private:
	std::vector<ToolTip> m_playerGear;
	std::map<std::string, unsigned int> m_playerGearCost;
public:
	using Menu::Menu;
	void createStaticMenuLayout()override;

	void onDraw(bool, sf::Vector2f)override;


	void update(updateEvent)override;
};

struct interactable {
	Menu* menu = nullptr;
	sf::Vector2f position;
	bool isMessaging = false;
	float activationRadius = 50;
	std::string sentMessage;
	AnimatorSprite ASprite;
};

//TODO create animation system that integrates with the render layer system
class UnitManager : public Parsable
{
private:
	Map *m_map;
	Weapon* m_playerWeapon = nullptr;
	unit *m_player = nullptr;
	std::vector<EnemyAI*> m_AIs;

	std::vector<std::string> m_startingMessages;

	std::vector<std::pair<sf::Vector2f, GearPiece>> m_mapGearPieces;

	std::vector<std::pair<sf::Vector2f, inventoryItem>> m_mapItems;

	float m_pickUpDistance = 100;

	std::vector<std::pair<GearPiece, unsigned int>> m_lootTable;

	std::vector<sf::Sprite> m_worldTextures;
	
	std::vector<sf::Texture*> m_toDeleteTextures;

	sf::RectangleShape m_levelEnd;
	bool m_needsAnUpdate = true;
	const float renderDistance = 1000;

	const float m_interactDistance = 50;
	std::vector<interactable> m_interactables;

	sf::Clock m_itemPickupClock;
	static const float m_itemPickupCooldown;

	endLevelTypes m_levelHasEnded = levelActive;
	std::string m_progressionFileName;

	const float m_toolTipReadDistance = 100;
	bool m_showToolTips = true;
	std::vector<std::pair<ToolTip*, bool>> m_toolTips;

	std::map<GearPiece, unsigned int, GearPiece::GearPieceCompare> m_itemToolTipID;

	unsigned int m_healthPotions = 10;
	unsigned int m_gold = 0;

	float m_levelScale = 1;

	void m_updateAIWeapons(std::vector<Weapon*>);
	void m_updateAI(float ,std::vector<unit*>, std::vector<unit*>);
	void m_updatePlayer(float, std::vector<unit*>);

    unit *m_closestAIUnit = nullptr;


	void pv_parseStep(std::vector<std::string>)override;

public:


    unit *getClosestAIUnit()const;

    unsigned int getHealthPotions()const;

	void addHealthPotions(unsigned int);
	void addGold(unsigned int);
	bool subtractGold(int);

	bool removeGearPiece(std::string);
	void drinkPotion();

	//sets wether the tooltips are shown or not
	void setToolTipsShow(bool);
	bool areToolTipsShown()const;

	void removeToolTip(unsigned int);

	//todo: implement item pick up and storage
	void placeItem(const inventoryItem&, sf::Vector2f);
	inventoryItem pickUpItem();

	void setProgressionFile(std::string);

	//void loadGearProgression();
	void saveGearProgression();

	endLevelTypes hasLevelEnded()const;

	Map* getMap()const;

	void startLevel();
	void endLevel();

	float getDistanceToPlayer(sf::Vector2f)const;

	void addInteractable(Menu*, sf::Vector2f);
	void addInteractable(const interactable&);
	Menu* interact(MessageBus*);

	UnitManager();
	UnitManager(Map*);
	void setMap(Map*);
	void setPlayerWeapon(Weapon*);
	//TODO: create a layer render system, to combine effects over the player/bullet etc, integrate with worldTextures and animation
	//DONE!^; reffer to the Animator singleton, in Animator.h
	void update(float, sf::RenderWindow&, MessageBus*);

	//TODO: create tooltip system that shows useful information once the player comes close;
	//interacts with the place gear on map function, such that when a player comes near a piece gear, a tooltip with its stats shows up
	unsigned int addToolTip(ToolTip*);

	//void loadLootTable(std::string);

	Weapon *getWeapon();

	unit* getPlayer();

	void placeGearOnMap(sf::Vector2f, GearPiece);
	void pickUpGear();
	//add a gearPiece to the player arsenal, if the gearPiece added has the same name as preexisting one, it is overwritten by the new one(i.e cant have two chestpieces equipped at once)
	//void addPlayerGear(GearPiece);
	//sets all of the player stats to the current gear, overwritting damage and other conditional ailments
	//void assignPlayerGear(bool heal = true);

	void addAI(EnemyAI*);
	void setPlayer(unit*);

	float getLevelScale() const;

	void setLevelScale(float);

	//void createFromFile(std::string);

	~UnitManager();
};


