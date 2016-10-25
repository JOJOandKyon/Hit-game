#include "BaseSprite/BaseSprite.h"
USING_NS_CC;

BaseSprite::BaseSprite() :
m_pIdleAction(NULL),
m_pWalkAction(NULL),
m_pAttackAction(NULL),
m_pHurtAction(NULL),
m_pDeadAction(NULL),
m_currActionState(ACTION_STATE_NONE)
{
}

BaseSprite::~BaseSprite()
{
	CC_SAFE_RELEASE_NULL(m_pIdleAction);
	CC_SAFE_RELEASE_NULL(m_pWalkAction);
	CC_SAFE_RELEASE_NULL(m_pAttackAction);
	CC_SAFE_RELEASE_NULL(m_pHurtAction);
	CC_SAFE_RELEASE_NULL(m_pDeadAction);
}

bool BaseSprite::init()
{
	Sprite::init();
	return true;
}

void BaseSprite::runIdleAction()
{
	if (changeState(ACTION_STATE_IDLE))
	{
		runAction(m_pIdleAction);
	}
}

void BaseSprite::runWalkAction()
{
	if (changeState(ACTION_STATE_WALK))
	{
		this->runAction(m_pWalkAction);
	}
}

void BaseSprite::runAttackAction()
{
	if (changeState(ACTION_STATE_ATTACK))
	{
		this->runAction(m_pAttackAction);
	}
}

void BaseSprite::runHurtAction()
{
	if (changeState(ACTION_STATE_HURT))
	{
		this->runAction(m_pHurtAction);
	}
}

void BaseSprite::runDeadAction()
{
	if (changeState(ACTION_STATE_DEAD))
	{
		this->runAction(m_pDeadAction);
	}
}

Animation* BaseSprite::createAnimation(const char* formatStr, int frameCount, int fps,int loop)
{
	SpriteFrameCache* pFrameCache = SpriteFrameCache::sharedSpriteFrameCache();
	pFrameCache->addSpriteFramesWithFile("pd_sprites.plist");
	SpriteFrame* frame = NULL;
	Vector<SpriteFrame*> array;
	for (int i = 0; i < frameCount; i++)
	{
		frame = pFrameCache->spriteFrameByName(String::createWithFormat(formatStr,i)->getCString());
		array.pushBack(frame);
	}
	return Animation::createWithSpriteFrames(array, 1.0f / fps, loop);
}

bool BaseSprite::changeState(ActionState actionState)
{
	if (m_currActionState == ACTION_STATE_DEAD || m_currActionState == actionState)
	{
		return false;
	}
	this->stopAllActions();
	m_currActionState = actionState;
	return true;
}

CallFunc* BaseSprite::createIdleCallbackFunc()
{
	return CallFunc::create(CC_CALLBACK_0(BaseSprite::runIdleAction, this));
}

BoundingBox BaseSprite::createBoundingBox(Point origin, Size size)
{
	BoundingBox boundingBox;
	boundingBox.original.origin = origin;
	boundingBox.original.size = size;
	boundingBox.actual.origin = this->getPosition() + boundingBox.original.origin;
	boundingBox.actual.size = size;
	return boundingBox;
}

void BaseSprite::updateBoxes()
{
	bool isFlippedX = this->isFlippedX();
	float x = 0.0f;
	if (isFlippedX)
	{
		x = this->getPosition().x - m_hitBox.original.origin.x;
	}
	else
	{		
		x = this->getPosition().x + m_hitBox.original.origin.x;
	}
	m_hitBox.actual.origin = Point(x, this->getPosition().y + m_hitBox.original.origin.y);
	m_bodyBox.actual.origin = this->getPosition() + m_bodyBox.original.origin;
}

void BaseSprite::setPosition(const Point &position)
{
	Sprite::setPosition(position);
	this->updateBoxes();
}


bool BaseSprite::PD(BoundingBox hitBox)
{
	if (m_bodyBox.actual.intersectsRect(hitBox.actual))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool BaseSprite::collisionDetection(const BoundingBox &hitBox, const BoundingBox &bodyBox)
{
	Rect hitRect = hitBox.actual;
	Rect bodyRect = bodyBox.actual;
	if (hitRect.intersectsRect(bodyRect))
	{
		return true;
	}	
	return false;
}