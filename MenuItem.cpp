#include "MenuItem.h"
#include <iostream>
#include "ToolTip.h"
#include "Animator.h"

std::string ReplaceString(std::string subject, const std::string& search,
	const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

std::string behaviourNameToString(behaviourNames behaviourName)
{
	switch (behaviourName)
	{
	case opensMenu:
		return "opensMenu";
		break;
	case resumesGame:
		return "resumesGame";
		break;
	default:
	    return "";
		break;
	}
}

behaviourNames stringToBehaviourName(std::string name)
{
	if (name == "opensMenu") {
		return opensMenu;
	}
	else if (name == "resumesGame") {
		return resumesGame;
	}
	else if (name == "sendsMessage") {
		return sendsMessage;
	}
	return resumesGame;
}

void MenuItem::setClickCooldown(float clickCooldown){
    m_clickCooldown = clickCooldown;
}

float MenuItem::getClickCooldown()const{
    return m_clickCooldown;
}

std::vector<behaviourParameters> MenuItem::click(bool clicked){
    std::vector<behaviourParameters> retVal;
    for(size_t i = 0; i < m_behaviours.size();i++){
        if(m_behaviours[i].activatesWhenClicked && clicked){
            retVal.push_back(m_behaviours[i]);
            continue;
        }
        if(!m_behaviours[i].activatesWhenClicked && !clicked){
            retVal.push_back(m_behaviours[i]);
        }
    }
    if(!clicked){
        return retVal;
    }
    if(m_clickTimer.getElapsedTime().asSeconds() > m_clickCooldown || m_firstClick || !m_hasClickTimer){
        m_firstClick = false;
        m_clickTimer.restart();
        return retVal;
    }
    return std::vector<behaviourParameters>();
}

MenuItem::MenuItem(sf::Rect<float> dim) :
	m_item(dim)
{
        m_clickTimer.restart();
}

void MenuItem::clearBehaviours()
{
	m_behaviours.clear();
}

void MenuItem::setIsButtonTextDisplayed(bool isButtonTextSet)
{
	m_isButtonTextSet = isButtonTextSet;
}

bool MenuItem::getIsButtonTextDisplayed() const
{
	return m_isButtonTextSet;;
}

void MenuItem::setButtonText(std::string text, float margins, sf::Color textColor, unsigned int style) {
	text = ReplaceString(text, "@", "\n");
	float minMarginsInPixels = std::min(margins * m_item.width, margins * m_item.height);
	m_isButtonTextSet = true;
	m_buttonText.setStyle(style);
	m_buttonText.setFillColor(textColor);
	m_buttonText.setFont(*ToolTip::getFont());
	m_buttonText.setString(text);
	m_buttonText.setPosition(sf::Vector2f(m_item.left + minMarginsInPixels, m_item.top + minMarginsInPixels));

	size_t lines = std::count(text.begin(), text.end(), '\n');
	lines++;
	m_buttonText.setCharacterSize(std::min((m_item.width - (minMarginsInPixels * 2)) / (text.length() / lines), (m_item.height - (minMarginsInPixels * 2)) / lines));
}

void MenuItem::setHasClickTimer(bool hasClickTimer)
{
	m_hasClickTimer = hasClickTimer;
}

bool MenuItem::getHasClickTimer() const
{
	return m_hasClickTimer;
}

void MenuItem::addbehaviourFromString(std::vector<std::string> behaviours)
{
	behaviourParameters tempBehaviourParam;
	tempBehaviourParam.behaviourName = stringToBehaviourName(behaviours[0]);

	switch (tempBehaviourParam.behaviourName)
	{
	case opensMenu:
		tempBehaviourParam.menuTo = behaviours[1];
		break;
	case resumesGame:
		break;
	case sendsMessage:
		//tempBehaviourParam.messageData.createFrom(behaviours[1]);
		break;
	default:
		break;
	}

	m_behaviours.push_back(tempBehaviourParam);
}

void MenuItem::addBehviourParameters(behaviourParameters behaviourParam)
{
	m_behaviours.push_back(behaviourParam);
}

void MenuItem::move(sf::Vector2f delta)
{
	moveTransform += delta;
}

void MenuItem::fitASpriteToItem(AnimatorSprite *aSprite)
{
	sf::Vector2f textureSize = sf::Vector2f(Animator::getInstance().getTexture(aSprite->textureID)->getSize());
	aSprite->position = getPosition() + sf::Vector2f(getDimension().x / 2, getDimension().y / 2);
	aSprite->originToCenter = true;
	aSprite->scale = std::min(getDimension().x / textureSize.x, getDimension().y / textureSize.y) / 1.2;
}

void MenuItem::setPosition(sf::Vector2f pos)
{
	moveTransform.x = -m_item.left;
	moveTransform.y = -m_item.top;
	moveTransform += pos;
}

sf::Vector2f MenuItem::getPosition() const
{
	return sf::Vector2f(m_item.left, m_item.top);
}

sf::Vector2f MenuItem::getDimension() const
{
	return sf::Vector2f(m_item.width, m_item.height);
}

void MenuItem::reestablishInitialPostion()
{
	moveTransform = sf::Vector2f(0,0);
}


void MenuItem::setTexture(const AnimatorSprite& aSprite)
{
	m_ASprite = aSprite;
	m_ASprite.originToCenter = false;
	m_ASprite.usesVectorScale = true;
	m_ASprite.position = sf::Vector2f(m_item.left, m_item.top);
	sf::Vector2f textureSize = sf::Vector2f(Animator::getInstance().getTexture(aSprite.textureID)->getSize());
	m_ASprite.vectorScale = sf::Vector2f(m_item.width / textureSize.x, m_item.height / textureSize.y);
	m_isTextureSet = true;
}

void MenuItem::draw(sf::RenderWindow* window, sf::Vector2f viewDisplacement)
{
	//temporary texture for menuItem, a blue square the size of the target bounding rectangle.
	if (!m_isTextureSet) {
		sf::RectangleShape tempRectS;
		tempRectS.setFillColor(sf::Color(55, 10, 0, 150));
		tempRectS.setSize(sf::Vector2f(m_item.width, m_item.height));
		tempRectS.setPosition(sf::Vector2f(m_item.left, m_item.top));
		tempRectS.setOutlineThickness(5);
		tempRectS.setOutlineColor(sf::Color(10, 10, 10, 100));
		tempRectS.move(viewDisplacement + moveTransform);
		window->draw(tempRectS);
	}
	else{
		m_ASprite.position += viewDisplacement + moveTransform;
		Animator::getInstance().instantDraw(m_ASprite);
		m_ASprite.position -= viewDisplacement + moveTransform;
	}

}

void MenuItem::drawText(sf::RenderWindow *window, sf::Vector2f viewDisplacement)
{
	if (m_isButtonTextSet) {
		m_buttonText.move(viewDisplacement + moveTransform);
		window->draw(m_buttonText);
		m_buttonText.move(-(viewDisplacement + moveTransform));
	}
}

bool MenuItem::isClicked(sf::Vector2f mousePos)
{
	m_item.left += moveTransform.x;
	m_item.top += moveTransform.y;
    bool retVal = m_item.contains(mousePos);
	m_item.left -= moveTransform.x;
	m_item.top -= moveTransform.y;
	return retVal;
}

std::vector<behaviourParameters> MenuItem::getBehaviour()const
{
	return m_behaviours;
}

MenuItem::~MenuItem()
{
}

void inventoryItem::createFrom(const decomposedData& DData)
{
	decomposedData tempDData = DData;
	if (tempDData.getChildByName("itemType") != nullptr) {
		itemType = itemTypes(ma_deserialize_uint(tempDData.getChildByName("itemType")->data[0]));
	}
	if (tempDData.getChildByName("gear") != nullptr) {
		simpleRep.createFrom(*tempDData.getChildByName("gear"));
	}
	if (tempDData.getChildByName("itemASprite") != nullptr) {
		itemASprite.createFrom(*tempDData.getChildByName("itemASprite"));
	}
	if (tempDData.getChildByName("itemToolTipTex") != nullptr) {
		itemToolTip.makeTooltipForGear(simpleRep);
		AnimatorSprite tempASprite;
		tempASprite.createFrom(*tempDData.getChildByName("itemToolTipTex"));
		itemToolTip.setTexture(tempASprite);
	}
}

void inventoryItem::setTexturesFromGearPiece()
{
	itemASprite = simpleRep.tex;
	itemToolTip.makeTooltipForGear(simpleRep);
}

decomposedData inventoryItem::serialize()
{
	decomposedData retDData;
	retDData.type = "inventoryItem";
	retDData.addChildrenObject(decomposedData().setName("itemType").addData(std::to_string(int(itemType))));
	retDData.addChildrenObject(simpleRep.serialize().setName("gear"));
	retDData.addChildrenObject(itemASprite.serialize().setName("itemASprite"));
	retDData.addChildrenObject(itemToolTip.getTexture().serialize().setName("itemToolTipTex"));
	return retDData;
}
