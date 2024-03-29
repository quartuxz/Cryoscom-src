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
#include "Menu.h"
#include "ToolTip.h"
#include "TileMap.h"
#include "MessageBus.h"
#include "Parsable.h"
#include "Gear.h"
#include "lootTable.h"

struct spawnPoint {
	sf::Vector2f pos;
	std::string enemyTemplateName;
	//the quotient percentage chance this spawn location is chosen
	unsigned int spawnChanceNumerator;
	unsigned int spawnChanceDenominator;

};

enum endLevelTypes {
	levelActive, playerDied, goalReached
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

	LootTable m_lootTable;

	TileMap m_tileMap;
	std::vector<sf::Sprite> m_worldTextures;
	
	std::vector<spawnPoint> m_spawnPoints;

	std::vector<sf::Texture*> m_toDeleteTextures;

	sf::RectangleShape m_levelEnd;
	bool m_needsAnUpdate = true;
	static const float renderDistance;

	const float m_interactDistance = 50;
	std::vector<interactable> m_interactables;

	sf::Clock m_itemPickupClock;
	static const float m_itemPickupCooldown;

	endLevelTypes m_levelHasEnded = levelActive;
	std::string m_progressionFileName;

	const float m_toolTipReadDistance = 250;
	bool m_showToolTips = true;
	std::vector<std::pair<ToolTip*, bool>> m_toolTips;

	std::map<GearPiece, unsigned int, GearPiece::GearPieceCompare> m_itemToolTipID;

	float m_levelScale = 1;

	void m_updateAIWeapons(std::vector<Weapon*>);
	void m_updateAI(float ,std::vector<unit*>, std::vector<unit*>);
	void m_updatePlayer(float, std::vector<unit*>);

    unit *m_closestAIUnit = nullptr;

	unsigned int m_remainingEnemiesToSpawn = 0;
	unsigned int m_totalActiveEnemies = 0;
	unsigned int m_currentRound = 0;
	unsigned int m_activeEnemyCap = 30;
	float m_timeBetweenEnemySpawn = 0.5;
	float m_timeAfterLastSpawn = 0;
	//spawns enemies according to the above variables
	void m_enemySpawnRun(float);

	std::map<std::string, EnemyAI*> m_enemyTemplates;

	void pv_parseStep(std::vector<std::string>)override;

public:

	std::map<std::string, EnemyAI*> getEnemyTemplates()const;

	unsigned int getRemainingEnemiesTotal()const;
	unsigned int getCurrentRound()const;
	//starts a new round with the specified number of enemies
	void startNewRound(unsigned int);

    unit *getClosestAIUnit()const;

	bool removeGearPiece(std::string);

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

	void update(float, sf::RenderWindow&, MessageBus*);

	//TODO: create tooltip system that shows useful information once the player comes close;
	//interacts with the place gear on map function, such that when a player comes near a piece gear, a tooltip with its stats shows up
	unsigned int addToolTip(ToolTip*);

	//void loadLootTable(std::string);

	Weapon *getWeapon()const;
	void setWeapon(Weapon*);

	unit* getPlayer()const;
	void setPlayer(unit*);

	//add a gearPiece to the player arsenal, if the gearPiece added has the same name as preexisting one, it is overwritten by the new one(i.e cant have two chestpieces equipped at once)
	//void addPlayerGear(GearPiece);
	//sets all of the player stats to the current gear, overwritting damage and other conditional ailments
	//void assignPlayerGear(bool heal = true);

	TileMap getTileMap()const;
	void setTileMap(const TileMap&);

	void addAI(EnemyAI*);

	//first argument is position second is the names of the player files
	void spawnEnemies(int, sf::Vector2f, std::vector<std::string>);


	float getLevelScale() const;

	void setLevelScale(float);

	//void createFromFile(std::string);

	~UnitManager();
};


