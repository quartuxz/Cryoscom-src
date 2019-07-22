#include "AnimatorSprite.h"
#include "Animator.h"



AnimatorSprite::AnimatorSprite() :
	textureID(0)
{
}

void AnimatorSprite::createFrom(const decomposedData &DData)
{
	decomposedData tempDData = DData;
	if (tempDData.getChildByName("textureName") != nullptr) {
		textureID = Animator::getInstance().getTextureID(tempDData.getChildByName("textureName")->data[0]);
	}
	if (tempDData.getChildByName("textureID") != nullptr) {
		textureID = ma_deserialize_uint(tempDData.getChildByName("textureID")->data[0]);
	}
	if (tempDData.getChildByName("drawLayer") != nullptr) {
		drawLayer = ma_deserialize_uint(tempDData.getChildByName("drawLayer")->data[0]);
	}
	if (tempDData.getChildByName("originToCenter") != nullptr) {
		originToCenter = ma_deserialize_uint(tempDData.getChildByName("originToCenter")->data[0]);
	}
	if (tempDData.getChildByName("rotation") != nullptr) {
		rotation = ma_deserialize_float(tempDData.getChildByName("rotation")->data[0]);
	}
	if (tempDData.getChildByName("scale") != nullptr) {
		scale = ma_deserialize_float(tempDData.getChildByName("scale")->data[0]);
	}
	if (tempDData.getChildByName("positionX") != nullptr) {
		position = sf::Vector2f( ma_deserialize_float(tempDData.getChildByName("positionX")->data[0]), position.y);
	}
	if (tempDData.getChildByName("positionY") != nullptr) {
		position = sf::Vector2f(position.x, ma_deserialize_float(tempDData.getChildByName("positionY")->data[0]));
	}

}

decomposedData AnimatorSprite::serialize()
{
	return decomposedData();
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
