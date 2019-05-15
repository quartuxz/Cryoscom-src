#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <stack>
#include <vector>
#include <tuple>
#include <queue>
#include <list>
#include <mutex>
#include "cryoscom_defsAndUtils.h"
#include "AnimatorSprite.h"
#include "ToolTip.h"



class Animator
{
public:
	static Animator& getInstance()
	{
		static Animator instance; // Guaranteed to be destroyed.
								// Instantiated on first use.
		return instance;
	}
private:
	Animator() {}                    // Constructor? (the {} brackets) are needed here.

	sf::RenderWindow *m_window;

	std::map<std::string, AnimatorSprite> m_namedAnimatorSprites;

	std::vector<std::stack<sf::Sprite>> m_spritesToDraw;
	std::vector<ToolTip*> m_toolTipsToDraw;

	unsigned int m_animationPresetIDCounter = 0;

	std::map<unsigned int, std::queue<AnimatorSprite>> m_animationPresets;

	//member description
	//each list element is an animation sequence, represented by a queue of tuples,
	//of which the first element is the texture currently being displayed,
	//the second the time its displayed for,
	//the third the position,
	//the fourth is the layer it is to be rendered on and
	//the fifth the scale of the object
	//the sixth the time elapsed since the texture was first displayed.
	//if(the second < the sixth) then the animation should pass to the next frame.
	std::list<std::pair<std::queue<AnimatorSprite>, AnimatorSprite*>> m_animations;

	std::list<std::pair<std::queue<AnimatorSprite>, AnimatorSprite*>> m_simpleAnimations;
	//member description

	std::list<AnimatorSprite> m_decals;

	std::map<unsigned int, sf::Texture*> m_textures;

	std::map<std::string, unsigned int> m_fileIDs;
	std::map<unsigned int, std::string> m_fileNames;

	//updates the passed animation, discarding any expired frames and the displaying the next non-expired frame; returns the new animation at the according time advancement
	std::queue<AnimatorSprite> m_updateAnimRecursive(std::queue<AnimatorSprite>, float, AnimatorSprite *);

	unsigned int m_TextureIDCounter = 0;

	sf::Sprite m_getSprite(AnimatorSprite);

	std::map<std::string, unsigned int> m_namedAnimations;

	unsigned int m_addRawTexture(sf::Texture*, std::string);

	std::recursive_mutex m_allLock;
	//MockMutex m_allLock;
public:
	Animator(Animator const&) = delete;
	void operator=(Animator const&) = delete;

	sf::RenderWindow *getWindow()const;

	void clearTextures();

	void setWindow(sf::RenderWindow *);

	sf::Texture* getTexture(unsigned int);

	void eraseInactiveAnimatorPresets();

	void loadTexturesFromFile(std::string);
	//pass the name of the texture file, returns the unique ID of the texture
	unsigned int addTexture(std::string);
	//pass the ID of the texture you wish to instanciate, together with the position you want to draw it on, its draw layer and its scale
	void addOneFrameSprite(const AnimatorSprite &);

	// clear all the namedAnimatorSprites for when passing to the enxt level
	void clearNamedAnimatorSprites();
	// named sprites for runtime editing
	std::map<std::string, AnimatorSprite> *getNamedAnimatorSprites();

	void addOneFrameSprite(ToolTip*);

	//takes float(time) as an argument, updates the animation state
	void update(float);

	void addDecal(const AnimatorSprite &);

	std::string getTextureFileName(unsigned int);
	unsigned int getTextureID(std::string);

	sf::FloatRect getTextureLocalBounds(unsigned int);

	//master play animation, all other play animation overloads call this
	//returns 
	void playAnimation(std::queue<AnimatorSprite>, AnimatorSprite*);

	//plays the selected animationPreset
	void playAnimation(unsigned int, AnimatorSprite*);

	std::queue<AnimatorSprite> getAnimationPreset(unsigned int);
	//not working right now
	AnimatorSprite *playAndGetAnimationState(unsigned int);

	unsigned int getAnimationPresetID(std::string);
	//pass an animation preset(the texture IDs selected, played for the specified amount time each; on the passed position for all, at the selected layer for all, with the selected scale(should be set to 1 for 1:1 ratio);
	//the last number should be set to 0(scrolls through the animation))
	//returns the animation preset unique ID
	unsigned int addAnimationPreset(std::queue<AnimatorSprite>, std::string);

	//draws all curent sprites, with render layer consideration 
	void draw();

	void instantDraw(AnimatorSprite);

	~Animator();
};

