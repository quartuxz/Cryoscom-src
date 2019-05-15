#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Gear.h"
#include <sstream>

class ToolTip
{
private:
	std::vector<sf::Text> m_text;

	

	AnimatorSprite m_backgroundImage;



	float m_scale = 1;
	sf::Vector2f m_position;
public:
	ToolTip();
	static void init(std::string);

	void setScale(float);
	void setPosition(sf::Vector2f);

	static sf::Font* getFont() {
		static sf::Font m_font;
		return &m_font;
		
	}

	void setTexture(const AnimatorSprite&);

	float getScale()const;
	sf::Vector2f getPosition()const;

	void addText(sf::Text);
    void addText(std::string, sf::Vector2f, float);
	void addText(std::string, sf::Vector2f, sf::Vector2f);
	void clearText();
	AnimatorSprite getTexture()const;

	void makeTooltipForGear(const GearPiece&);

	void draw(sf::RenderWindow&);
	~ToolTip();
};

