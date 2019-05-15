#include "AnimatorSprite.h"




AnimatorSprite::AnimatorSprite() :
	textureID(0)
{
}

AnimatorSprite AnimatorSprite::create(bool p_offsets)
{
	AnimatorSprite tempASprite;
	tempASprite.offsets = p_offsets;
	return tempASprite;
}

AnimatorSprite::AnimatorSprite(unsigned int ID) :
	textureID(ID)
{
}

AnimatorSprite::AnimatorSprite(unsigned int ID, unsigned int layer) :
	textureID(ID), drawLayer(layer)
{
}
