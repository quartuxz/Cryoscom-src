#include "Animator.h"
#include <Python.h>
#include <boost/python.hpp>
#include <cstdlib>
#include <fstream>
#include "Serializable.h"
#include "InGameMessages.h"

Animator::~Animator()
{
	m_isProgramEnded = false;
	if (m_multithreaded) {
		m_renderThread->join();
		delete m_renderThread;
	}   

	std::map<unsigned int, sf::Texture*>::iterator it;
	for (it = m_textures.begin(); it != m_textures.end(); ++it)
	{
		delete it->second;
	}

}


sf::RenderWindow* Animator::getWindow()const {

	return m_window;
}

void Animator::setPlayerPos(const sf::Vector2f &pos)
{
	m_playerXPos = pos.x;
	m_playerYPos = pos.y;
}

void Animator::m_workOneFrameSprite(const AnimatorSprite &aSprite)
{
	if (m_spritesToDraw.size() < (aSprite.drawLayer + 1)) {
		m_spritesToDraw.resize(aSprite.drawLayer + 1);
	}
	//lineMessage tempLineMessage;
	//tempLineMessage.isImportantMessage = true;
	//tempLineMessage.message = ma_serialize(m_spritesToDraw.size());
	//tempLineMessage.messageColor = sf::Color::Blue;
	//InGameMessages::getInstance().addLine(tempLineMessage);
	if (aSprite.isActive) {
		if (aSprite.isUI || vectorDistance(aSprite.position,sf::Vector2f(m_playerXPos,m_playerYPos))<=renderDistance) {
			m_spritesToDraw[aSprite.drawLayer].push(m_getSprite(aSprite));
		}
		
	}
}

void Animator::m_renderThreadFunction()
{
	while (!m_isProgramEnded) {
		if (m_startWorking) {
			
			//m_renderDataMutex.lock();
			auto m_tempThreadWork = m_renderThreadWork;
			m_renderThreadWork = std::queue<AnimatorSprite>();
			//m_renderDataMutex.unlock();
			m_startWorking = false;
			
			m_renderWorkMutex.lock();
			while (!m_tempThreadWork.empty()) {
				
				m_workOneFrameSprite(m_tempThreadWork.front());
				m_tempThreadWork.pop();
			}
			m_renderWorkMutex.unlock();
			
		}
		else {
			//std::this_thread::yield();
		}

	}
}

void Animator::pv_parseStep(std::vector<std::string> tokens)
{
	if (tokens[0] == "texture") {
		addTexture(tokens[1]);
	}
	else if (tokens[0] == "texturesheet") {
		sf::Texture* tempTex = new sf::Texture();
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

sf::Sprite Animator::m_getSprite(const AnimatorSprite &aSprite)
{
	sf::Sprite tempSprite;
	if (aSprite.isInvisible) {
		return tempSprite;
	}
	m_getSpritesLock.lock();
	tempSprite.setTexture(*m_textures[aSprite.textureID]);
	if (aSprite.originToCenter) {
		tempSprite.setOrigin(sf::Vector2f(tempSprite.getLocalBounds().width / 2, tempSprite.getLocalBounds().height / 2));
	}
	
	tempSprite.setPosition(aSprite.position);
	tempSprite.setRotation(aSprite.rotation);
	

	if (aSprite.usesVectorScale) {
		tempSprite.setScale(aSprite.vectorScale);
	}
	else {
		tempSprite.setScale(sf::Vector2f(aSprite.scale, aSprite.scale));
	}
	if (aSprite.isUI){
		sf::Vector2f tempOrigin = tempSprite.getOrigin();
		                                    
		sf::Vector2f newWindowZeroPoint = -((sf::Vector2f(m_window->getSize()) * AnimatorSprite::zoom) - sf::Vector2f(m_window->getSize())) / 2.f;
		tempSprite.setPosition((tempSprite.getPosition()) * AnimatorSprite::zoom + aSprite.UIDisplacement);
		tempSprite.setOrigin(newWindowZeroPoint);
		tempSprite.move(newWindowZeroPoint);
		tempSprite.scale(sf::Vector2f(AnimatorSprite::zoom, AnimatorSprite::zoom));
		
		tempSprite.setOrigin(tempOrigin);
	}
	m_getSpritesLock.unlock();

	return tempSprite;
}

unsigned int Animator::m_addRawTexture(sf::Texture *tempTex, std::string fileName)
{
	m_fileIDs[fileName] = m_TextureIDCounter;
	m_fileNames[m_TextureIDCounter] = fileName;
	m_textures[m_TextureIDCounter++] = tempTex;
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
}

unsigned int Animator::addTexture(std::string fileName)
{
	sf::Texture *tempTex = new sf::Texture();
	tempTex->loadFromFile(fileName);
	unsigned int retVal = m_addRawTexture(tempTex, fileName);
	return retVal;
}

void Animator::addOneFrameSprite(const AnimatorSprite &aSprite)
{
	//multithreaded solution, works but buggy:
	if (m_multithreaded) {
		//m_renderWorkMutex.lock();
		while (m_startWorking) {

		}
		m_renderThreadWork.push(aSprite);
		//m_renderWorkMutex.unlock();
	}
	else {
		m_workOneFrameSprite(aSprite);
	}

}

void Animator::clearNamedAnimatorSprites()
{
	m_namedAnimatorSprites.clear();
}

std::map<std::string, AnimatorSprite>* Animator::getNamedAnimatorSprites()
{
	return &m_namedAnimatorSprites;
}

void Animator::addOneFrameSprite(const ToolTip &toolTip)
{
	m_toolTipsToDraw.push_back(toolTip);
}

void Animator::update(float timeDelta)
{
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
}

void Animator::addDecal(const AnimatorSprite &decal)
{
	m_decals.push_back(decal);
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
	sf::Sprite tempSprite;
	tempSprite.setTexture(*m_textures[textureID]);
	return tempSprite.getLocalBounds();
}

void Animator::playAnimation(std::queue<AnimatorSprite> animation, AnimatorSprite *controller)
{
	controller->offsets = true;
	controller->isActive = true;
	m_animations.push_back(std::pair<std::queue<AnimatorSprite>, AnimatorSprite*>(animation, controller));
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
	m_animationPresets[m_animationPresetIDCounter] = animationPreset;
	m_namedAnimations[animationName] = m_animationPresetIDCounter++;
	return m_animationPresetIDCounter;
}

void Animator::draw()
{
	
	m_renderWorkMutex.lock();
	m_startWorking = true;
	for (size_t i = 0; i < m_spritesToDraw.size(); i++)
	{
		while (!m_spritesToDraw[i].empty()) {
			m_window->draw(m_spritesToDraw[i].top());
			m_spritesToDraw[i].pop();
		}
	}
	


	for (auto const& x : m_namedAnimatorSprites)
	{
		addOneFrameSprite(x.second);
	}


	std::list<AnimatorSprite>::iterator it;
	for (it = m_decals.begin(); it != m_decals.end(); ++it)
	{
		addOneFrameSprite(*it);
		if (it->timeDisplayed == 0 || it->timeElapsed >= it->timeDisplayed) {
			m_decals.erase(it);
		}
	}
	m_renderWorkMutex.unlock();
	

	for (size_t i = 0; i < m_toolTipsToDraw.size(); i++)
	{
		m_toolTipsToDraw[i].draw(*m_window);
	}
	
	m_toolTipsToDraw.clear();
	

}

void Animator::instantDraw(const AnimatorSprite &aSprite)
{
	sf::Sprite tempSprite = m_getSprite(aSprite);
	m_window->draw(tempSprite);

}

