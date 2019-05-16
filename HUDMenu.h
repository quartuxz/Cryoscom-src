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
public:
	using Menu::Menu;

	//returns the ID of the passed skill
	size_t addSkill(skillParam*);
	

	void onDraw(bool, sf::Vector2f)override;
	void createStaticMenuLayout()override;

	void update(updateEvent)override;

	~HUDMenu();
};