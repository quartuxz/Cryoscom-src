#pragma once
#include <vector>
#include "Menu.h"
#include "Weapon.h"


class HUDMenu : public Menu {
private:
	ToolTip m_availablePotions;
	void m_updatePotions(unsigned int);

	std::vector<skillParam*> m_usedSkills;
	size_t m_hotbarSlotsNumber = 10;
	float m_inGameTextCharSize = 0.02;
public:
	using Menu::Menu;

	//returns the ID of the skill slot
	size_t addSkill(skillParam*);
	//requires the ID of the skill slot
	skillParam *getSkill(size_t);

	void removeSkill(size_t);

	void onDraw(bool, sf::Vector2f)override;
	void createStaticMenuLayout()override;

	void update(updateEvent)override;

	~HUDMenu();
};
