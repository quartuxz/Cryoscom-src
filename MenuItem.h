#pragma once
#include <SFML/Graphics.hpp>
#include "AnimatorSprite.h"
#include "Serializable.h"
#include "Gear.h"
#include "AnimatorSprite.h"
#include "ToolTip.h"

enum itemTypes {
	emptySlot, gearPieceType, medkitType, ammoType
};

struct inventoryItem : public Serializable{
	decomposedData serialize()override;
	void createFrom(const decomposedData&)override;

	itemTypes itemType = emptySlot;
	std::string serializedObjectRep;
	GearPiece simpleRep;
	ToolTip itemToolTip;
	AnimatorSprite itemASprite;
	size_t amount = 1;
};

enum behaviourNames {
	opensMenu, resumesGame, removesGearPiece, buysItem, sendsMessage, equipsGearPiece
};

std::string behaviourNameToString(behaviourNames);
behaviourNames stringToBehaviourName(std::string);

struct behaviourParameters {
	behaviourNames behaviourName;
	std::string menuTo;

    bool activatesWhenClicked = true;

	unsigned int goldCost;

	inventoryItem* grabbedItem;
	GearPiece gearPiece;

	decomposedData messageData;

	std::string itemBought;
	std::string gearPieceRemoved;
};


class MenuItem
{
private:
	sf::Rect<float> m_item;

	sf::Vector2f moveTransform;

	sf::Text m_buttonText;

	bool m_isButtonTextSet = false;

    bool m_firstClick = true;

	bool m_hasClickTimer = true;

	sf::Sprite m_tex;
	bool m_isTextureSet =false;
	AnimatorSprite m_ASprite;

	std::vector<behaviourParameters> m_behaviours;
	sf::Clock m_clickTimer;
	float m_clickCooldown = 2;
public:
	MenuItem(sf::Rect<float>);

	void clearBehaviours();

	void setHasClickTimer(bool);
	bool getHasClickTimer()const;

	void addbehaviourFromString(std::vector<std::string>);
	void addBehviourParameters(behaviourParameters);

    void setClickCooldown(float);
    float getClickCooldown()const;

	void move(sf::Vector2f);

	void fitASpriteToItem(AnimatorSprite*);

	void setPosition(sf::Vector2f);
	sf::Vector2f getPosition()const;
	sf::Vector2f getDimension()const;

	void setIsButtonTextDisplayed(bool);
	bool getIsButtonTextDisplayed()const;
	void setButtonText(std::string, float, sf::Color, unsigned int);

	void reestablishInitialPostion();

    std::vector<behaviourParameters> click(bool);

	void setTexture(const AnimatorSprite&);
	void draw(sf::RenderWindow*, sf::Vector2f viewDisplacement = sf::Vector2f(0,0));
	void drawText(sf::RenderWindow*, sf::Vector2f viewDisplacement = sf::Vector2f(0, 0));

	bool isClicked(sf::Vector2f);
	std::vector<behaviourParameters> getBehaviour()const;
	~MenuItem();
};

