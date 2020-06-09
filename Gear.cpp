#include "Gear.h"
#include <fstream>
#include <random>
#include "Animator.h"

size_t bodyPartOrMove::m_uniqueIDCounter = 0;

void combatModule::createFrom(const decomposedData& DData)
{
	decomposedData tempDData = DData;
	if (tempDData.getChildByName("hitPointCap") != nullptr) {
		hitpointCap = ma_deserialize_float(tempDData.getChildByName("hitPointCap")->data[0]);
	}
	if (tempDData.getChildByName("psiPointCap") != nullptr) {
		psiPointCap = ma_deserialize_float(tempDData.getChildByName("psiPointCap")->data[0]);
	}
	if (tempDData.getChildByName("staminaCap") != nullptr) {
		staminaCap = ma_deserialize_float(tempDData.getChildByName("staminaCap")->data[0]);
	}
	if (tempDData.getChildByName("staminaRegen") != nullptr) {
		staminaRegen = ma_deserialize_float(tempDData.getChildByName("staminaRegen")->data[0]);
	}
	if (tempDData.getChildByName("moveSpeed") != nullptr) {
		moveSpeed = ma_deserialize_float(tempDData.getChildByName("moveSpeed")->data[0]);
	}
	if (tempDData.getChildByName("damage") != nullptr) {
		damage = ma_deserialize_float(tempDData.getChildByName("damage")->data[0]);
	}
	if (tempDData.getChildByName("bulletSpeed") != nullptr) {
		bulletSpeed = ma_deserialize_float(tempDData.getChildByName("bulletSpeed")->data[0]);
	}
	if (tempDData.getChildByName("bulletSize") != nullptr) {
		bulletSize = ma_deserialize_float(tempDData.getChildByName("bulletSize")->data[0]);
	}
	if (tempDData.getChildByName("bulletDuration") != nullptr) {
		bulletDuration = ma_deserialize_float(tempDData.getChildByName("bulletDuration")->data[0]);
	}
	if (tempDData.getChildByName("fireRate") != nullptr) {
		fireRate = ma_deserialize_float(tempDData.getChildByName("fireRate")->data[0]);
	}
	if (tempDData.getChildByName("magSize") != nullptr) {
		magSize = ma_deserialize_uint(tempDData.getChildByName("magSize")->data[0]);
	}
	if (tempDData.getChildByName("reloadTime") != nullptr) {
		reloadTime = ma_deserialize_float(tempDData.getChildByName("reloadTime")->data[0]);
	}
	if (tempDData.getChildByName("weight") != nullptr) {
		weight = ma_deserialize_float(tempDData.getChildByName("weight")->data[0]);
	}
}

decomposedData combatModule::serialize()
{
	decomposedData tempDData;
	tempDData.type = "combatModule";
	decomposedData tempChildren;
	tempDData.addChildrenObject(decomposedData().addData(std::to_string(hitpointCap)).setName("hitpointCap"));
	tempDData.addChildrenObject(decomposedData().addData(std::to_string(psiPointCap)).setName("psiPointCap"));
	tempDData.addChildrenObject(decomposedData().addData(std::to_string(staminaCap)).setName("staminaCap"));
	tempDData.addChildrenObject(decomposedData().addData(std::to_string(staminaRegen)).setName("staminaRegen"));
	tempDData.addChildrenObject(decomposedData().addData(std::to_string(moveSpeed)).setName("moveSpeed"));
	tempDData.addChildrenObject(decomposedData().addData(std::to_string(damage)).setName("damage"));
	tempDData.addChildrenObject(decomposedData().addData(std::to_string(bulletSpeed)).setName("bulletSpeed"));
	tempDData.addChildrenObject(decomposedData().addData(std::to_string(bulletSize)).setName("bulletSize"));
	tempDData.addChildrenObject(decomposedData().addData(std::to_string(bulletDuration)).setName("bulletDuration"));
	tempDData.addChildrenObject(decomposedData().addData(std::to_string(fireRate)).setName("fireRate"));
	tempDData.addChildrenObject(decomposedData().addData(std::to_string(magSize)).setName("magSize"));
	tempDData.addChildrenObject(decomposedData().addData(std::to_string(reloadTime)).setName("reloadTime"));
	tempDData.addChildrenObject(decomposedData().addData(std::to_string(weight)).setName("weight"));

	return tempDData;
}

//
//
//
//
//TODO, make combat more indepth
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

size_t combatModule::addBodyPartOrMove(const bodyPartOrMove &BPOM)
{
	m_bodyPartsOrMoves.push_back(std::pair<size_t, bodyPartOrMove>(bodyPartOrMove::m_uniqueIDCounter, BPOM));
	return bodyPartOrMove::m_uniqueIDCounter++;
}

void combatModule::removeBodyPartOrMove(size_t ID)
{
	for (size_t i = 0; i < m_bodyPartsOrMoves.size(); i++)
	{
		if (m_bodyPartsOrMoves[i].first == ID) {
			m_bodyPartsOrMoves.erase(m_bodyPartsOrMoves.begin()+i);
		}
	}
}

std::vector<std::pair<size_t, bodyPartOrMove>> combatModule::getBodyPartsOrMoves() const
{
	return m_bodyPartsOrMoves;
}

void combatModule::update(float timeDelta)
{
	for (auto const& x : m_damageTimers)
	{
		m_damageTimers[x.first].duration -= timeDelta;
		if (m_damageTimers[x.first].duration <= 0) {
			hitpoints -= m_damageTimers[x.first].amount;
			m_damageTimers.erase(x.first);
		}
	}
	stamina += staminaRegen * timeDelta;
	if (stamina > 100) {
		stamina = 100;
	}
	hitpoints -= (damageTakenPerSecond * timeDelta);
	if (hitpoints > hitpointCap) {
		hitpoints = hitpointCap;
	}
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

void combatModule::applyDamageInstance(unsigned int ID, float damageInflicted, float delay, bool stopTimer)
{
	map<unsigned int, effect>::iterator it = m_damageTimers.find(ID);
	//elements that both exist and dont need to be removed are let to run
	//naturally until their time expires and they are applied as damage
	if (it != m_damageTimers.end())
	{
		//if element exists and needs to be removed:
		if (stopTimer) {
			m_damageTimers.erase(ID);
		}
	}//if element doesnt exist and needs to be created:
	else if(!stopTimer){
		effect tempEffect;
		tempEffect.duration = delay;
		tempEffect.amount = damageInflicted;
		m_damageTimers[ID] = tempEffect;
	}
	
}

void GearPiece::createFrom(const decomposedData& DData)
{
	decomposedData tempDData = DData;
	if (tempDData.getChildByName("cModule") != nullptr) {
		cModule.createFrom(*tempDData.getChildByName("cModule"));
	}
	if (tempDData.getChildByName("gearType") != nullptr) {
		type = gearTypes(ma_deserialize_uint(tempDData.getChildByName("gearType")->data[0]));
	}
	if (tempDData.getChildByName("tex") != nullptr) {
		tex.createFrom(*tempDData.getChildByName("tex"));
	}
	if (tempDData.getChildByName("toolTipTexture") != nullptr) {
		toolTipTexture.createFrom(*tempDData.getChildByName("toolTipTexture"));
	}
}

decomposedData GearPiece::serialize()
{
	decomposedData tempDData;
	tempDData.type = "gearPiece";
	tempDData.addChildrenObject(decomposedData().setName("gearType").addData(std::to_string(type)));
	tempDData.addChildrenObject(cModule.serialize().setName("cModule"));
	tempDData.addChildrenObject(tex.serialize().setName("tex"));
	tempDData.addChildrenObject(toolTipTexture.serialize().setName("toolTipTexture"));
	
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

	
	target->m_bodyPartsOrMoves.insert(target->m_bodyPartsOrMoves.begin(),cModule.m_bodyPartsOrMoves.begin(), cModule.m_bodyPartsOrMoves.end());
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

	for (size_t i = 0; i < target->m_bodyPartsOrMoves.size(); i++)
	{
		for (size_t o = 0; o < cModule.m_bodyPartsOrMoves.size(); o++)
		{
			if (target->m_bodyPartsOrMoves[i].first == cModule.m_bodyPartsOrMoves[o].first) {
				target->m_bodyPartsOrMoves.erase(target->m_bodyPartsOrMoves.begin()+i);
			}
		}
	}
}
  
GearPiece::~GearPiece()
{
}

