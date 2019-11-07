#include "AnimatorSprite.h"
#include "Animator.h"

float AnimatorSprite::zoom = 1;

AnimatorSprite::AnimatorSprite() :
	textureID(0)
{
}

void AnimatorSprite::scaleToMatch(rescaleTypes rescaleType, float size)
{
	switch (rescaleType)
	{
	case fitX:
		usesVectorScale = false;
		scale = size / Animator::getInstance().getTexture(textureID)->getSize().x;
		break;
	case fitY:
		usesVectorScale = false;
		scale = size / Animator::getInstance().getTexture(textureID)->getSize().y;
		break;
	case fitMin:
		usesVectorScale = false;
		scale = std::min(size / Animator::getInstance().getTexture(textureID)->getSize().x, size / Animator::getInstance().getTexture(textureID)->getSize().y);
		break;
	case fitDistorted:
		usesVectorScale = true;
		vectorScale.x = size / Animator::getInstance().getTexture(textureID)->getSize().x;
		vectorScale.y = size / Animator::getInstance().getTexture(textureID)->getSize().y;
		break;
	default:
		break;
	}
}

void AnimatorSprite::scaleToMatch(sf::Vector2f size)
{
	usesVectorScale = true;
	vectorScale.x = size.x / Animator::getInstance().getTexture(textureID)->getSize().x;
	vectorScale.y = size.y / Animator::getInstance().getTexture(textureID)->getSize().y;
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
	if (tempDData.getChildByName("timeDisplayed") != nullptr) {
		timeDisplayed = ma_deserialize_float(tempDData.getChildByName("timeDisplayed")->data[0]);
	}

}

decomposedData AnimatorSprite::serialize()
{
	decomposedData retDData;
	retDData.addChildrenObject(decomposedData().setName("textureID").addData(std::to_string(textureID)));
	retDData.addChildrenObject(decomposedData().setName("drawLayer").addData(std::to_string(drawLayer)));
	retDData.addChildrenObject(decomposedData().setName("originToCenter").addData(std::to_string(originToCenter)));
	retDData.addChildrenObject(decomposedData().setName("rotation").addData(std::to_string(rotation)));
	retDData.addChildrenObject(decomposedData().setName("scale").addData(std::to_string(scale)));
	retDData.addChildrenObject(decomposedData().setName("positionX").addData(std::to_string(position.x)));
	retDData.addChildrenObject(decomposedData().setName("positionY").addData(std::to_string(position.y)));
	retDData.addChildrenObject(decomposedData().setName("timeDisplayed").addData(std::to_string(timeDisplayed)));
	return retDData;
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
