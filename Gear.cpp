#include "Gear.h"
#include <fstream>
#include "Animator.h"


void combatModule::createFrom(const decomposedData& DData)
{

}

decomposedData combatModule::serialize()
{
	decomposedData tempDData;
	tempDData.type = "combatModule";
	return tempDData;
}

void combatModule::attack(combatModule *cModule)
{
	cModule->hitpoints -= damage;
}

combatModule::combatModule(bool initToZero): Serializable()
{
	hitpoints = 0;
	hitpointCap = 0;
	psiPoints = 0;
	psiPointCap = 0;
	stamina = 0;
	staminaCap = 0;
	staminaRegen = 0;
	moveSpeed = 0;
	damage = 0;
	bulletSpeed = 0;
	bulletSize = 0;
	bulletDuration = 0;
	ammo = 0;
	fireRate = 0;
	inaccuracy = 0;
	magSize = 0;
	reloadTime = 0;
}

combatModule::combatModule(): Serializable()
{
}

void combatModule::processEffects(float timeDelta) {
	for (size_t i = 0; i < effects.size(); i++)
	{
		float minDuration = std::min(timeDelta, effects[i].duration);
		switch (effects[i].eType)
		{
		case staminaRegenEffect:
			stamina += effects[i].amount * minDuration;
			if (stamina > 100) {
				stamina = 100;
			}
			break;
		case damageOverTimeEffect:
			hitpoints -= effects[i].amount * minDuration;
			break;
		case rootEffect:
			if (!effects[i].active) {
				effects[i].originalAmount = moveSpeed;
				moveSpeed -= effects[i].amount;
				effects[i].active = true;
			}
			break;
		default:
			break;
		}
		effects[i].duration -= timeDelta;
		if (effects[i].duration <= 0) {
			if (effects[i].eType == rootEffect) {
				moveSpeed = effects[i].originalAmount;
			}
			effects.erase(effects.begin() + i);
		}
	}

}

void GearPiece::createFrom(const decomposedData&)
{
}

decomposedData GearPiece::serialize()
{
	decomposedData tempDData;
	tempDData.type = "GearPiece";
	tempDData.name = name;
	tempDData.data.push_back(std::to_string(type));
	return tempDData;
}

void GearPiece::equipGearPiece(combatModule *target) const
{
	target->hitpointCap += cModule.hitpointCap;
	target->hitpointRegen += cModule.hitpointRegen;
	target->psiPointCap += cModule.psiPointCap;
	target->psiPointRegen += cModule.psiPointRegen;
	target->staminaCap += cModule.staminaCap;
	target->staminaRegen += cModule.staminaRegen;
	target->moveSpeed += cModule.moveSpeed;
	target->weight += cModule.weight;
	target->damage += cModule.damage;
	if (cModule.isMelee) {
		target->isMelee = true;
	}
	target->bulletSpeed += cModule.bulletSpeed;
	target->bulletSize += cModule.bulletSize;
	target->bulletDuration += cModule.bulletDuration;
	target->fireRate += cModule.fireRate;
	target->inaccuracy += cModule.inaccuracy;
	target->magSize += cModule.magSize;
	target->reloadTime += cModule.reloadTime;
}

void GearPiece::unequipGearPiece(combatModule *target) const
{
	target->hitpointCap -= cModule.hitpointCap;
	target->hitpointRegen -= cModule.hitpointRegen;
	target->psiPointCap -= cModule.psiPointCap;
	target->psiPointRegen -= cModule.psiPointRegen;
	target->staminaCap -= cModule.staminaCap;
	target->staminaRegen -= cModule.staminaRegen;
	target->moveSpeed -= cModule.moveSpeed;
	target->weight -= cModule.weight;
	target->damage -= cModule.damage;
	if (cModule.isMelee) {
		target->isMelee = false;
	}
	target->bulletSpeed -= cModule.bulletSpeed;
	target->bulletSize -= cModule.bulletSize;
	target->bulletDuration -= cModule.bulletDuration;
	target->fireRate -= cModule.fireRate;
	target->inaccuracy -= cModule.inaccuracy;
	target->magSize -= cModule.magSize;
	target->reloadTime -= cModule.reloadTime;
}

GearPiece::~GearPiece()
{
}

GearPiece GearPiece::makeGearPiece(std::vector<std::string> tokens)
{

	
}
