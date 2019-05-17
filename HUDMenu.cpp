#include "HUDMenu.h"
#include <stdexcept>
#include "Animator.h"

struct skillsFullException: public std::exception
{
	const char* what() const noexcept
	{
		return "skill slots are full, please remove a skill and try again!";
	}
};


void HUDMenu::createStaticMenuLayout()
{

	sf::Vector2f individualHotbarSize = sf::Vector2f(0.06, 0.06);


	for (size_t i = 0; i < m_hotbarSlotsNumber; i++)
	{
		MenuItem hotbarMenuItem(sf::Rect<float>(getPixelCoordinate(sf::Vector2f(individualHotbarSize.x * i + (0.5 - (individualHotbarSize.x * m_hotbarSlotsNumber) / 2), 0)), getPixelCoordinate(individualHotbarSize)));
		AnimatorSprite hotbarMenuItemTex;
		hotbarMenuItemTex.textureID = Animator::getInstance().getTextureID("Inventory-Slot (Empty).png");
		hotbarMenuItem.setTexture(hotbarMenuItemTex);
		hotbarMenuItem.setButtonText(std::to_string(i+1).append("  "), 0.1, sf::Color::Red, 0);
		m_menuItems.push_back(hotbarMenuItem);
	}
	m_usedSkills.resize(m_hotbarSlotsNumber, nullptr);
	

	MenuItem tempMenuItem(sf::Rect<float>(getPixelCoordinate(sf::Vector2f(0, 1)), getPixelCoordinate(sf::Vector2f(0.2, 0.5))));
	m_menuItems.push_back(tempMenuItem);

	
	//m_availablePotions.setTexture();
	m_updatePotions(0);
}

void HUDMenu::m_updatePotions(unsigned int availablePotions) {
	sf::Text tempText;
	std::stringstream ss;
	m_availablePotions.clearText();
	ss << "available potions: " << availablePotions << std::endl;
	tempText.setString(ss.str());
	tempText.setPosition(sf::Vector2f(100, 100));
	tempText.setScale(sf::Vector2f(1, 1));
	tempText.setFillColor(sf::Color(0, 0, 0, 255));
	m_availablePotions.addText(tempText);
}

size_t HUDMenu::addSkill(skillParam *sParam)
{
	for (size_t i = 0; i < m_usedSkills.size(); i++)
	{
		if (m_usedSkills[i] == nullptr) {
			m_usedSkills[i] = sParam;
			m_menuItems[i].fitASpriteToItem(&m_usedSkills[i]->skillIcon);
			return i;
		}
	}
	throw skillsFullException();
}

skillParam* HUDMenu::getSkill(size_t skillSlotID)
{
	return m_usedSkills[skillSlotID];
}

void HUDMenu::onDraw(bool beforeDraw, sf::Vector2f viewDisplacement) {
	if (beforeDraw) {
		sf::Vector2f initialPosition = m_availablePotions.getPosition();
		m_availablePotions.setPosition(initialPosition + viewDisplacement);
		m_availablePotions.draw(*m_window);
		m_availablePotions.setPosition(initialPosition);
		
	}
	else {
		for (size_t i = 0; i < m_usedSkills.size(); i++)
		{
			if (m_usedSkills[i] != nullptr) {
				m_usedSkills[i]->skillIcon.position += viewDisplacement;
				Animator::getInstance().instantDraw(m_usedSkills[i]->skillIcon);
				m_usedSkills[i]->skillIcon.position -= viewDisplacement;
			}
		}
	}
}

void HUDMenu::update(updateEvent uEvent)
{
	switch (uEvent.updateEventType)
	{
	case lostLife:
		m_menuItems[m_hotbarSlotsNumber].reestablishInitialPostion();
		//std::cout << "currentLife: " << uEvent.currentLife << ", maxLife: " << uEvent.maxLife << ", percentage: " << (uEvent.currentLife / uEvent.maxLife) << std::endl;
		m_menuItems[m_hotbarSlotsNumber].move(sf::Vector2f(0, -m_menuItems[0].getDimension().y * (uEvent.currentLife / uEvent.maxLife)));
		//m_menuItems[0].move(sf::Vector2f(0, (m_menuItems[0].getDimension().y/1) * (uEvent.currentLife/uEvent.maxLife)));
		break;
	case lostPotions:
		m_updatePotions(uEvent.availablePotions);
		break;
	default:
		break;
	}
}


HUDMenu::~HUDMenu()
{
	for (size_t i = 0; i < m_usedSkills.size(); i++)
	{
		if (m_usedSkills[i] != nullptr) {
			delete m_usedSkills[i];
		}
	}
}
