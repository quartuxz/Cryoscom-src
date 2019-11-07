#include "ToolTip.h"
#include "Animator.h"


void ToolTip::init(std::string fileName)
{
	static bool init = true;
	if (init) {
		getFont()->loadFromFile(fileName);
		init = false;
	}
}

void ToolTip::setViewDisplacement(const sf::Vector2f &viewDisplacement)
{
	m_viewDisplacement = viewDisplacement;
}

sf::Vector2f ToolTip::getViewDisplacement() const
{
	return m_viewDisplacement;
}

void ToolTip::setIsUIElement(bool UIElement)
{
	m_isUIElement = UIElement;
}

bool ToolTip::isUIElement() const
{
	return m_isUIElement;
}

ToolTip::ToolTip()
{
}

void ToolTip::setScale(float scale)
{
	m_scale = scale;
}

void ToolTip::setPosition(sf::Vector2f pos)
{
	m_position = pos;
}

void ToolTip::setTexture(const AnimatorSprite& aSprite)
{
	m_backgroundImage = aSprite;
}

float ToolTip::getScale() const
{
	return m_scale;
}

sf::Vector2f ToolTip::getPosition() const
{
	return m_position;
}

void ToolTip::addText(sf::Text text){
    text.setFont(*getFont());
    m_text.push_back(text);
}

void ToolTip::addText(std::string textString, sf::Vector2f position, sf::Vector2f scale)
{
	sf::Text tempText;
	tempText.setString(textString);
	tempText.setPosition(position);
	tempText.setScale(scale);
	addText(tempText);
}


void ToolTip::addText(std::string textString, sf::Vector2f position, float scale)
{
	addText(textString, position, sf::Vector2f(scale, scale));
}

void ToolTip::clearText()
{
	m_text.clear();
}

AnimatorSprite ToolTip::getTexture() const
{
	return m_backgroundImage;
}

void ToolTip::makeTooltipForGear(const GearPiece& gearPiece)
{
	
	m_backgroundImage.textureID = Animator::getInstance().getTextureID("tooltip.png");
	m_backgroundImage.scale = 5;
	m_backgroundImage.originToCenter = false;
	m_text.clear();
	std::stringstream ss;
	ss << "->" << gearPiece.type << std::endl << "HP: " << gearPiece.cModule.hitpointCap << std::endl << "Move Speed: " << gearPiece.cModule.moveSpeed << std::endl
	   << "fire rate: " << gearPiece.cModule.fireRate << std::endl
	   << "inaccuracy: " << gearPiece.cModule.inaccuracy << std::endl
	   << "bullet speed: " << gearPiece.cModule.bulletSpeed << std::endl
	   << "bullet duration: " << gearPiece.cModule.bulletDuration << std::endl
	   << "bullet size: " << gearPiece.cModule.bulletSize << std::endl
	   << "magazine size: " << gearPiece.cModule.magSize << std::endl
	   << "reload time(s): " << gearPiece.cModule.reloadTime << std::endl
	   << "gold value: " << gearPiece.goldValue << std::endl;
	sf::Text tempText;
	tempText.setFillColor(sf::Color::Green);
	tempText.setFont(*getFont());
	tempText.setString(ss.str());
	tempText.setPosition(sf::Vector2f(15,15));
	tempText.setScale(sf::Vector2f(0.35, 0.35));
	m_text.push_back(tempText);
	tempText.setString(ss.str());
}

void ToolTip::draw(sf::RenderWindow &window)
{
	if (m_backgroundImage.isActive) {
		AnimatorSprite tempBackgroundImage = m_backgroundImage;
		tempBackgroundImage.scale *= m_scale;
		tempBackgroundImage.position += m_position;
		if (m_isUIElement) {
			tempBackgroundImage.isUI = true;
			tempBackgroundImage.UIDisplacement = m_viewDisplacement;
			Animator::getInstance().instantDraw(tempBackgroundImage);
		}
		else {
			Animator::getInstance().instantDraw(tempBackgroundImage);
		}
		
		

	}
	for (size_t i = 0; i < m_text.size(); i++)
	{
		sf::Text tempText = m_text[i];
		tempText.scale(sf::Vector2f(m_scale, m_scale));
		tempText.move(m_position);
		if (m_isUIElement) {
			window.draw(zoomText(tempText, &window, m_viewDisplacement));
		}
		else {
			window.draw(tempText);
		}
		
		//m_text[i].move(-m_position);
		//m_text[i].scale(sf::Vector2f(1/(m_scale*AnimatorSprite::zoom), (1/m_scale*AnimatorSprite::zoom)));
	}
}

ToolTip::~ToolTip()
{
}
