#include "Menu.h"
#include <fstream>
#include <vector>
#include <iostream>
#include "Animator.h"

void Menu::pv_onClick(MenuItem*, size_t, const InputManager&)
{
}

Menu::Menu(sf::RenderWindow *window):
	m_window(window)
{
}

unsigned int Menu::addMenuItem(MenuItem menuItem)
{
	m_menuItems.push_back(menuItem);
	return m_menuItems.size() - 1;
}

void Menu::addBehaviour(behaviourParameters Bparam, unsigned int menuItemID)
{
	m_menuItems[menuItemID].addBehviourParameters(Bparam);
}

MenuItem Menu::createMenuItem(sf::Vector2f pos, sf::Vector2f size) const
{
	return MenuItem(sf::FloatRect(getPixelCoordinate(pos),getPixelCoordinate(size)));
}

void Menu::createMenuFromFile(std::string fileName)
{
	std::string line;
	std::ifstream fileRead(fileName);
	if (fileRead.is_open()) {
		while (getline(fileRead, line)) {
			std::vector<std::string> tokens;

			if (tokens.empty()) {
				std::string delimiter = ";";

				size_t pos = 0;
				pos = line.find(delimiter);
				while (pos != std::string::npos) {
					tokens.push_back(line.substr(0, pos));
					line.erase(0, pos + delimiter.length());
					pos = line.find(delimiter);
				}
			}

			if (tokens.empty()) {
				continue;
			}
			else if (tokens[0] == "//") {
				continue;
			}
			else if (tokens[0] == "load") {
				createMenuFromFile(tokens[1]);
				continue;
			}
			else if (tokens[0] == "whenOpened") {
				MenuItem tempMenuItem = MenuItem(MenuItem(sf::Rect<float>(sf::Vector2f(0, 0), sf::Vector2f(0, 0))));
				for (size_t i = 1; i < tokens.size(); i++)
				{
					std::vector<std::string> behaviour;

					std::string delimiter = ",";

					size_t pos = 0;
					pos = tokens[i].find(delimiter);
					while (pos != std::string::npos) {
						behaviour.push_back(tokens[i].substr(0, pos));
						tokens[i].erase(0, pos + delimiter.length());
						pos = tokens[i].find(delimiter);
					}
					tempMenuItem.addbehaviourFromString(behaviour);
				}
				//m_onOpenParams = tempMenuItem.getBehaviour();
			}
			sf::Rect<float> menuItemDim(sf::Vector2f(std::atof(tokens[0].c_str()) * m_window->getSize().x, std::atof(tokens[1].c_str()) * m_window->getSize().y),
				sf::Vector2f(0, 0));

			if (tokens[2] == "=") {
				menuItemDim.height = std::atof(tokens[3].c_str()) * m_window->getSize().y;
				menuItemDim.width = menuItemDim.height;
			}
			else if (tokens[3] == "=") {
				menuItemDim.width = std::atof(tokens[2].c_str()) * m_window->getSize().x;
				menuItemDim.height = menuItemDim.width;
			}
			else {
				menuItemDim.width = std::atof(tokens[2].c_str()) * m_window->getSize().x;
				menuItemDim.height = std::atof(tokens[3].c_str()) * m_window->getSize().y;
			}

			m_menuItems.push_back(MenuItem(menuItemDim));
			sf::Sprite tempSprite;
			if (tokens[4] != "" && tokens[4] != "-") {
				AnimatorSprite tempASprite;
				tempASprite.textureID = Animator::getInstance().getTextureID(tokens[4]);
				m_menuItems.back().setTexture(tempASprite);
			}
			//todo: make transparency work here
			else if (tokens[4] == "transparent") {
				
				//m_menuItems.back().setTexture(tempSprite);
			}


			m_menuItems.back().setButtonText(tokens[5], std::atof(tokens[6].c_str()), sf::Color(std::atoi(tokens[7].c_str()), std::atoi(tokens[8].c_str()), std::atoi(tokens[9].c_str()), std::atoi(tokens[10].c_str())), std::atoi(tokens[11].c_str()));
			for (size_t i = 12; i < tokens.size(); i++)
			{
				std::vector<std::string> behaviour;

				std::string delimiter = ",";

				size_t pos = 0;
				pos = tokens[i].find(delimiter);
				while (pos != std::string::npos) {
					behaviour.push_back(tokens[i].substr(0, pos));
					tokens[i].erase(0, pos + delimiter.length());
					pos = tokens[i].find(delimiter);
				}
				m_menuItems.back().addbehaviourFromString(behaviour);
			}

		}
		fileRead.close();
	}
}


std::vector<behaviourParameters> Menu::onClick(sf::Vector2f mousePos, const InputManager &inputs, Game)
{
	std::vector<behaviourParameters> retval;
	for (size_t i = 0; i < m_menuItems.size(); i++)
	{
		if (m_menuItems[i].isClicked(mousePos)) {
			pv_onClick(&m_menuItems[i], i, inputs);
			return m_menuItems[i].click(inputs.isInputEventActive(InputManager::shoot));
		}

	}
	return std::vector<behaviourParameters>();
}

std::vector<behaviourParameters> Menu::onInput(sf::Vector2f, const InputManager&inputs)
{
	std::vector<behaviourParameters> retval;
	for (size_t i = 0; i < m_menuItems.size(); i++)
	{
		pv_onClick(&m_menuItems[i], i, inputs);
		return m_menuItems[i].click(inputs.isInputEventActive(InputManager::shoot));

	}
	return std::vector<behaviourParameters>();
}

void Menu::onDraw(bool, sf::Vector2f)
{
}

sf::Vector2f Menu::getScaleFactor(sf::Vector2f currentScale, sf::Vector2f desired)const
{
	sf::Vector2f desiredInPixels = sf::Vector2f(desired.x * m_window->getSize().x, desired.y * m_window->getSize().y);
	return sf::Vector2f(desiredInPixels.x / currentScale.x, desiredInPixels.y / currentScale.y);
}

sf::Vector2f Menu::getPixelCoordinate(sf::Vector2f pos)const
{
	return sf::Vector2f(pos.x*m_window->getSize().x, pos.y*m_window->getSize().y);
}

void Menu::update(updateEvent uEvent)
{
	return;
	switch (uEvent.updateEventType)
	{
	case lostLife:

		break;
	default:
		break;
	}
}

void Menu::createStaticMenuLayout()
{
}

void Menu::draw(sf::Vector2f viewDisplacement)
{

	onDraw(true, viewDisplacement);
	for (size_t i = 0; i < m_menuItems.size(); i++)
	{
		m_menuItems[i].draw(m_window, viewDisplacement);
	}
	onDraw(false, viewDisplacement);
	for (size_t i = 0; i < m_menuItems.size(); i++)
	{
		m_menuItems[i].drawText(m_window, viewDisplacement);
	}
}

Menu::~Menu()
{
	for (size_t i = 0; i < m_toDeleteTextures.size(); i++)
	{
		delete m_toDeleteTextures[i];
	}
}

