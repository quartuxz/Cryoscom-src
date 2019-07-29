#include "Animator.h"
#include <Python.h>
#include <boost/python.hpp>
#include <cstdlib>
#include <fstream>
#include "Serializable.h"

Animator::~Animator()
{
	std::map<unsigned int, sf::Texture*>::iterator it;
	for (it = m_textures.begin(); it != m_textures.end(); ++it)
	{
		delete it->second;
	}

}

sf::RenderWindow* Animator::getWindow()const {

	return m_window;
}

void Animator::loadTexturesFromFile(std::string fileName) {
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
			if (tokens[0] == "texture") {
				addTexture(tokens[1]);
			}
			else if(tokens[0] == "texturesheet"){
				sf::Texture *tempTex = new sf::Texture();
				tempTex->loadFromFile(tokens[1], sf::IntRect(std::atoi(tokens[2].c_str()), std::atoi(tokens[3].c_str()), std::atoi(tokens[4].c_str()), std::atoi(tokens[5].c_str())));
				m_addRawTexture(tempTex, tokens[6]);
			}
			else if (tokens[0] == "loadAnimation") {
				std::queue<AnimatorSprite> tempAnimation;
				float allAnimScale = std::atof(tokens[2].c_str());
				for (size_t i = 3; i < tokens.size(); i++)
				{
					AnimatorSprite tempASprite;
					tempASprite.createFrom(decomposedData().createFrom(tokens[i]));
					tempASprite.scale *= allAnimScale;
					tempAnimation.push(tempASprite);
				}
				addAnimationPreset(tempAnimation, tokens[1]);
			}

		}
		fileRead.close();
	}
}

std::queue<AnimatorSprite> Animator::m_updateAnimRecursive(std::queue<AnimatorSprite> animation, float timeDelta, AnimatorSprite *output)
{

	AnimatorSprite tempASprite = animation.front();

	if (tempASprite.timeDisplayed > tempASprite.timeElapsed && tempASprite.isActive) {
		//addOneFrameSprite(tempASprite);
		tempASprite.timeElapsed += timeDelta;
		animation.front() = tempASprite;
		*output = tempASprite;
	}
	else {
		animation.pop();
		if (!animation.empty()) {
			animation = m_updateAnimRecursive(animation, tempASprite.timeElapsed - tempASprite.timeDisplayed, output);
		}
	}



	return animation;
}

sf::Sprite Animator::m_getSprite(AnimatorSprite aSprite)
{
	sf::Sprite tempSprite;
	tempSprite.setTexture(*m_textures[aSprite.textureID]);
	if (aSprite.originToCenter) {
		tempSprite.setOrigin(sf::Vector2f(tempSprite.getLocalBounds().width / 2, tempSprite.getLocalBounds().height / 2));
	}
	tempSprite.setPosition(aSprite.position);
	if (aSprite.usesVectorScale) {
		tempSprite.setScale(aSprite.vectorScale);
	}
	else {
		tempSprite.setScale(sf::Vector2f(aSprite.scale, aSprite.scale));
	}
	
	tempSprite.setRotation(aSprite.rotation);

	return tempSprite;
}

unsigned int Animator::m_addRawTexture(sf::Texture *tempTex, std::string fileName)
{
	m_allLock.lock();
	m_fileIDs[fileName] = m_TextureIDCounter;
	m_fileNames[m_TextureIDCounter] = fileName;
	m_textures[m_TextureIDCounter++] = tempTex;
	m_allLock.unlock();
	return (m_TextureIDCounter - 1);
}

void Animator::clearTextures()
{
	m_textures.clear();
}

void Animator::setWindow(sf::RenderWindow *window)
{
	m_window = window;
}

sf::Texture* Animator::getTexture(unsigned int ID)
{
	return m_textures[ID];
}

void Animator::eraseInactiveAnimatorPresets()
{
	m_allLock.lock();
	std::list<std::pair<std::queue<AnimatorSprite>, AnimatorSprite*>>::iterator it = m_animations.begin();
	while (it != m_animations.end())
	{
		if (!it->second->isActive) {
			m_animations.erase(it++);
		}
		else {
			++it;
		}
		
	}
	m_allLock.unlock();
}

unsigned int Animator::addTexture(std::string fileName)
{
	m_allLock.lock();
	sf::Texture *tempTex = new sf::Texture();
	tempTex->loadFromFile(fileName);
	unsigned int retVal = m_addRawTexture(tempTex, fileName);
	m_allLock.unlock();
	return retVal;
}

void Animator::addOneFrameSprite(const AnimatorSprite &aSprite)
{
	m_allLock.lock();
    if (m_spritesToDraw.size() < (aSprite.drawLayer + 1)) {
		m_spritesToDraw.resize(aSprite.drawLayer + 1);
	}
	if (aSprite.isActive) {
		m_spritesToDraw[aSprite.drawLayer].push(m_getSprite(aSprite));
	}
	m_allLock.unlock();
}

void Animator::clearNamedAnimatorSprites()
{
	m_allLock.lock();
	m_namedAnimatorSprites.clear();
	m_allLock.unlock();
}

std::map<std::string, AnimatorSprite>* Animator::getNamedAnimatorSprites()
{
	return &m_namedAnimatorSprites;
}

void Animator::addOneFrameSprite(ToolTip *toolTip)
{
	m_allLock.lock();
	m_toolTipsToDraw.push_back(toolTip);
	m_allLock.unlock();
}

void Animator::update(float timeDelta)
{
	m_allLock.lock();
	std::list<AnimatorSprite>::iterator it2;
	for (it2 = m_decals.begin(); it2 != m_decals.end(); ++it2)
	{
		if (it2->timeDisplayed != 0 && it2->timeElapsed < it2->timeDisplayed) {
			it2->timeElapsed += timeDelta;
		}
	}

	std::list<std::pair<std::queue<AnimatorSprite>, AnimatorSprite*>>::iterator it = m_animations.begin();
	while (it != m_animations.end())
	{
		if (it->first.empty()) {
			it->second->isActive = false;
			m_animations.erase(it++);
		}
		else {
			it->first = m_updateAnimRecursive(it->first, timeDelta, it->second);
			++it;
		}
	}
	m_allLock.unlock();
}

void Animator::addDecal(const AnimatorSprite &decal)
{
	m_allLock.lock();
	m_decals.push_back(decal);
	m_allLock.unlock();
}

std::string Animator::getTextureFileName(unsigned int textureID)
{
	return m_fileNames[textureID];
}

unsigned int Animator::getTextureID(std::string fileName)
{
	return m_fileIDs[fileName];
}

sf::FloatRect Animator::getTextureLocalBounds(unsigned int textureID)
{
	m_allLock.lock();
	sf::Sprite tempSprite;
	tempSprite.setTexture(*m_textures[textureID]);
	m_allLock.unlock();
	return tempSprite.getLocalBounds();
}

void Animator::playAnimation(std::queue<AnimatorSprite> animation, AnimatorSprite *controller)
{
	m_allLock.lock();
	controller->offsets = true;
	controller->isActive = true;
	m_animations.push_back(std::pair<std::queue<AnimatorSprite>, AnimatorSprite*>(animation, controller));
	m_allLock.unlock();
}

void Animator::playAnimation(unsigned int animationPresetID, AnimatorSprite *controller)
{
	return playAnimation(m_animationPresets[animationPresetID], controller);
}

std::queue<AnimatorSprite> Animator::getAnimationPreset(unsigned int animationPresetID)
{
	return m_animationPresets[animationPresetID];
}

AnimatorSprite* Animator::playAndGetAnimationState(unsigned int animationPresetID)
{
	AnimatorSprite *retASprite = new AnimatorSprite();
	m_simpleAnimations.push_back(std::pair<std::queue<AnimatorSprite>, AnimatorSprite*>(getAnimationPreset(animationPresetID), retASprite));
	return retASprite;
}

unsigned int Animator::getAnimationPresetID(std::string animationName)
{
	return m_namedAnimations[animationName];
}

unsigned int Animator::addAnimationPreset(std::queue<AnimatorSprite> animationPreset, std::string animationName)
{
	m_allLock.lock();
	m_animationPresets[m_animationPresetIDCounter] = animationPreset;
	m_namedAnimations[animationName] = m_animationPresetIDCounter++;
	m_allLock.unlock();
	return m_animationPresetIDCounter;
}

void Animator::draw()
{



	for (size_t i = 0; i < m_spritesToDraw.size(); i++)
	{
		while (!m_spritesToDraw[i].empty()) {
			m_window->draw(m_spritesToDraw[i].top());
			m_spritesToDraw[i].pop();
		}
	}

	m_allLock.lock();
	for (auto const& x : m_namedAnimatorSprites)
	{
		addOneFrameSprite(x.second);
	}

	m_spritesToDraw.clear();
	std::list<AnimatorSprite>::iterator it;
	for (it = m_decals.begin(); it != m_decals.end(); ++it)
	{
		addOneFrameSprite(*it);
		if (it->timeDisplayed == 0 || it->timeElapsed >= it->timeDisplayed) {
			m_decals.erase(it);
		}
	}


	for (size_t i = 0; i < m_toolTipsToDraw.size(); i++)
	{
		m_toolTipsToDraw[i]->draw(*m_window);
	}
	m_toolTipsToDraw.clear();
	m_allLock.unlock();
}

void Animator::instantDraw(AnimatorSprite aSprite)
{
	m_window->draw(m_getSprite(aSprite));
}

