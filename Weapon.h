#pragma once
#include <list>
#include <vector>
#include <queue>
#include <SFML/Graphics.hpp>
#include "AnimatorSprite.h"
#include "unit.h"
#include "Bullet.h"

enum failCastTypes {
	notFail, outOfStamina, outOfPsiPoints, outOfMeleeRange
};

enum skillTypes {
	dash, meleeAttack, circularDamage
};

struct skillParam {
	float castDelay = 0;
	AnimatorSprite skillIcon;
	AnimatorSprite skillTex;
	sf::Vector2f dirUnitVec;
	skillTypes sType;
	std::vector<sf::Vector2f> skillEffecArea = std::vector<sf::Vector2f>();
	float maxRange = 1000;
	float strenght = 3;
	float manaCost = 0;
	float staminaCost = 0;
};



class Weapon
{
private:
	sf::Sprite m_bulletTex;

	AnimatorSprite m_bulletATex;

	std::queue<skillParam*> m_activeSkills;

	std::list<Bullet*> m_bullets;
	unit *m_wieldingUnit;
	float m_fireRate = 5;
	float m_bulletSpeed = 1;

    unsigned int m_magBulletsUsed = 0;

	void m_fire(sf::Vector2f);


	void m_useSkill(skillParam*, std::vector<unit*>);

	float m_accumulatedFireTime = 0;
public:

	void setBulletTex(sf::Sprite&);
	void setbulletAnimatorTex(AnimatorSprite);

    void reload();

    bool canFire();

	sf::Vector2f getUnitVecTo(sf::Vector2f);

	Weapon(unit*);
	void setWieldingUnit(unit*);

	bool fire(sf::Vector2f);


	failCastTypes addSkillToQueue(skillParam*);

	std::vector<unsigned int> update(float, std::vector<unit*>);
	std::vector<unsigned int> update(float, std::queue<unit*>);

	std::list<Bullet*> getBullets()const;
	void displayToWindow(sf::RenderWindow&);
	~Weapon();
};

