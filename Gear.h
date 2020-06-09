#pragma once
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "AnimatorSprite.h"
#include "Serializable.h"
#include "cryoscom_defsAndUtils.h"

struct combarModule;


enum bodyPartTypes {
	head,neck,chest,belly,arms,legs,feet
};

struct bodyPartOrMove {
private:
	static size_t m_uniqueIDCounter;
public:
	float armourThickness;
	float dodgeChance;
	bodyPartTypes bodyPartType;    
	std::string name;
	friend struct combatModule;
};

enum effectTypes {
	staminaRegenEffect, damageOverTimeEffect, rootEffect, damageAfterTimeEffect
};

struct effect {
	effectTypes eType;
	float duration = 0;
	float amount = 0;
	float originalAmount = 0;
	bool active = false;
};

enum gearTypes {
	helmet, chestPiece, jumpsuit, boots
};

//todo: implement serialization

struct GearPiece;

struct combatModule : public Serializable {
private:
	std::vector<std::pair<size_t, bodyPartOrMove>> m_bodyPartsOrMoves;
	std::map<unsigned int, effect> m_damageTimers;
public:

	float hitpoints = 100;
	float hitpointCap = 100;
	float hitpointRegen = 0;
	float psiPoints = 100;
	float psiPointCap = 100;
	float psiPointRegen = 0;
	float stamina = 100;
	float staminaCap = 100;
	float staminaRegen = 5;
	float moveSpeed = 400;

	float damageTakenPerSecond;
	void createFrom(const decomposedData&);
	decomposedData serialize();

	void attack(combatModule*);

	float weight = 0;
	float ammo = 1000;
	combatModule(bool);
	combatModule();

	size_t addBodyPartOrMove(const bodyPartOrMove&);
	void removeBodyPartOrMove(size_t);
	std::vector<std::pair<size_t, bodyPartOrMove>> getBodyPartsOrMoves()const;

	//applies some caps and calculates some other things
	void update(float);
	void processEffects(float);
	std::vector<effect> effects;
	std::vector<std::pair<sf::Vector2f, float>> pushes;

	//initiates a time counter for some form of damage
	//(unique id of damage source, total damage, duration before damage(in seconds), stop timer[true if so])
	void applyDamageInstance(unsigned int, float, float, bool);

	float damage = 20;
	bool isMelee = false;
	//the range from the outer collision unit edge to the target`s
	float meleeRange = 50;
	float meleeDelay = 0.5;
	float bulletSpeed = 200;
	float bulletSize = 2;
	float bulletDuration = 4;
	float fireRate = 10;
	float inaccuracy = 10;
	unsigned int magSize = 20;
	float reloadTime = 1;


	friend struct GearPiece;
};
//todo: implement serialization and proper equip/unequip
struct GearPiece : public Serializable {
	size_t ID = reinterpret_cast<size_t>(this);

	unsigned int goldValue = 10;
	gearTypes type;
	std::string name;

	void createFrom(const decomposedData&);
	decomposedData serialize();

	combatModule cModule;
	AnimatorSprite tex;
	AnimatorSprite toolTipTexture;
	bool healsOnPickup = false;

	//adds the stats to a combatModule
	void equipGearPiece(combatModule*)const;
	//subtracts the stats from a combatModule
	void unequipGearPiece(combatModule*)const;

	~GearPiece();

	struct GearPieceCompare
	{
		bool operator() (const GearPiece& lhs, const GearPiece& rhs) const
		{
			return lhs.ID < rhs.ID;
		}
	};

};

