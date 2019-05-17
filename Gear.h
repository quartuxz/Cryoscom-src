#pragma once
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "AnimatorSprite.h"
#include "Serializable.h"
#include "cryoscom_defsAndUtils.h"

enum effectTypes {
	staminaRegenEffect, damageOverTimeEffect, rootEffect
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
struct combatModule : public Serializable {
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
	float ammo = 100;
	combatModule(bool);
	combatModule();

	//applies some caps and calculates some other things
	void update(float);
	void processEffects(float);
	std::vector<effect> effects;
	std::vector<std::pair<sf::Vector2f, float>> pushes;

	float damage = 20;
	bool isMelee = false;
	float bulletSpeed = 200;
	float bulletSize = 2;
	float bulletDuration = 4;
	float fireRate = 10;
	float inaccuracy = 10;
	unsigned int magSize = 5;
	float reloadTime = 2;
};
//todo: implement serialization and and proper equip/unequip
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
	std::string toolTipBackGroundTex;
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

