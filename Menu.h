#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "MessageBus.h"
#include "InputManager.h"
#include "MenuItem.h"

enum updateType {
	lostLife, lostMana, click, lostPotions
};

struct updateEvent {
	updateType updateEventType;
	float currentLife;
	float maxLife;

	unsigned int availablePotions;

	sf::Vector2f clickPos;

	float currentMana;
	float maxMana;
};

class Menu
{
protected:
	std::vector<MenuItem*> m_menuItems;
	sf::RenderWindow *m_window;
	std::vector<sf::Texture*> m_toDeleteTextures;
	virtual void pv_onClick(MenuItem*, size_t, const InputManager&, MessageBus*);
public:

	explicit Menu(sf::RenderWindow*);
	//adds a menu item x and y screens away from the top left, of size provided by second argument(also measured relatively to the screen)
	unsigned int addMenuItem(MenuItem*);
	void addBehaviour(behaviourParameters, unsigned int);
	MenuItem createMenuItem(sf::Vector2f, sf::Vector2f)const;

	void createMenuFromFile(std::string);
	std::vector<behaviourParameters> onClick(sf::Vector2f, const InputManager&, MessageBus*);
	
	//the bool indicates wether its the beforeDraw or afterDraw, meaning that the first time this
	//function is called with a false value(menuItems not yet drawn)
	//then with a true value(menuItems drawn)
	virtual void onDraw(bool, sf::Vector2f);

	//first argument is current size in pixels, second argument is desired size in window coverage%.
	sf::Vector2f getScaleFactor(sf::Vector2f, sf::Vector2f)const;
	//returns the actual pixel coordinate of a window%(minimum of 2 dimensions for square)
	//second argument determines wether the first argument is a global position or a dimension
	sf::Vector2f getPixelCoordinate(sf::Vector2f, bool isPos = false)const;

	virtual void update(updateEvent);
	virtual void createStaticMenuLayout();

	void draw(sf::Vector2f viewDisplacement = sf::Vector2f(0,0));

	virtual ~Menu();
};
