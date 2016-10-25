#ifndef _BASESPRITE_H_
#define _BASESPRITE_H_

#include "cocos2d.h"
#include "cocostudio\CocoStudio.h"

using namespace cocos2d;

typedef enum 
{
	ACTION_STATE_NONE=0,
	ACTION_STATE_IDLE,
	ACTION_STATE_WALK,
	ACTION_STATE_ATTACK,
	ACTION_STATE_HURT,
	ACTION_STATE_DEAD
}ActionState;

typedef struct
{
	Rect actual;
	Rect original;
}BoundingBox;

class BaseSprite :public Sprite
{
public:
	BaseSprite();
	~BaseSprite();
	void runIdleAction();
	void runWalkAction();
	void runAttackAction();
	void runKnockoutAction();
	void runHurtAction();
	void runDeadAction();
private:
	CC_SYNTHESIZE_RETAIN(cocos2d::Action*, m_pIdleAction, IdleAction);
	CC_SYNTHESIZE_RETAIN(cocos2d::Action*, m_pWalkAction, WalkAction);
	CC_SYNTHESIZE_RETAIN(cocos2d::Action*, m_pAttackAction, AttackAction);
	CC_SYNTHESIZE_RETAIN(cocos2d::Action*, m_pHurtAction, HurtAction);
	CC_SYNTHESIZE_RETAIN(cocos2d::Action*, m_pDeadAction, DeadAction);
	CC_SYNTHESIZE(unsigned int, m_hp, HP);
	CC_SYNTHESIZE(unsigned int, m_attack, Attack);
	CC_SYNTHESIZE(Point,m_velocity,Velocity);
	CC_SYNTHESIZE(ActionState, m_currActionState, CurrActionState);
	CallFunc* createIdleCallbackFunc();	
private:
	CC_SYNTHESIZE(BoundingBox, m_bodyBox, BodyBox);
	CC_SYNTHESIZE(BoundingBox, m_hitBox, HitBox);


protected:
	static Animation* createAnimation(const char* formatStr, int frameCount, int fps,int loop);
private:
	bool changeState(ActionState actionState);
public:
	BoundingBox createBoundingBox(Point origin, Size size);
	void updateBoxes();
	virtual void setPosition(const Point &position);
public:
	virtual bool init();
	CREATE_FUNC(BaseSprite);
public:
	bool PD(BoundingBox hitBox);
	bool collisionDetection(const BoundingBox &hitBox, const BoundingBox &bodyBox);

};
#endif