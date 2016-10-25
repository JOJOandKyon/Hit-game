#include "enemy/enemy.h"
#include "cocos2d.h"
#include "cocostudio\CocoStudio.h"
#include "JoyStick\JoyStick.h"

USING_NS_CC;
#define MOVE_SPEED		2
#define RUN_SPEED		2

bool Enemy::init()
{
	bool ret = false;
	auto joyStick = JoyStick::getInstance();
	my_hero = joyStick->getHero();
	Size enemyShowSize = this->getDisplayFrame()->getRect().size;
	Size enemySize = this->getContentSize();
	Size rect = Size(50,100);
	this->m_bodyBox = this->createBoundingBox(Point(-enemyShowSize.width / 2, -enemyShowSize.height / 2), rect);
	auto abc = this->m_bodyBox.actual.size;
	this->m_hitBox = this->createBoundingBox(Point(enemyShowSize.width / 2, -5), Size(25, 20));
	m_hp = 50;
	m_attack = 25;
	do
	{
		this->initWithSpriteFrameName("robot_idle_00.png");
		Animate* pIdleAnim = Animate::create(createAnimation("robot_idle_%02d.png", 5, 8, -1));
		setIdleAction(RepeatForever::create(pIdleAnim));
		Animate* pWalkAnim = Animate::create(createAnimation("robot_walk_%02d.png", 6, 9, 1));
		setWalkAction(Sequence::create(pWalkAnim, createIdleCallbackFunc(), NULL));

		Animate* pAttackAnim = Animate::create(createAnimation("robot_attack_%02d.png", 5, 9, 1));
		setAttackAction(Sequence::create(pAttackAnim, createIdleCallbackFunc(), NULL));
		Animate* pHurtAnim = Animate::create(createAnimation("robot_hurt_%02d.png", 3, 15, 1));
		setHurtAction(Sequence::create(pHurtAnim, createIdleCallbackFunc(), NULL));
		Animate* pDeadAnim = Animate::create(createAnimation("robot_knockout_%02d.png", 5, 9, 1));
		setDeadAction(Sequence::create(pDeadAnim, createIdleCallbackFunc(),
			Blink::create(0.5f,3),
			CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, this)),
			NULL));
		ret = true;
	} while (0);
	schedule(schedule_selector(Enemy::update), 0.07);
	return true;
}

Enemy::Enemy() :
m_nextDecisionTime(0),
time(0),
flag(true)
{
}

Enemy::~Enemy()
{
}

void Enemy::execute(const Point& target, float targetBodyWidth)
{
	if (m_nextDecisionTime == 0)
	{
		this->decide(target, targetBodyWidth);
	}
	else
	{
		m_nextDecisionTime--;
	}
}

void Enemy::decide(const Point& target, float targetBodyWidth)
{


}

void Enemy::updateEnemy(float dt)
{

}

void Enemy::JudegeAttack()
{
	srand((UINT)GetCurrentTime());
	int x = rand() % 100;
	if (x < 60)
	{
		this->runAttackAction();
	}

	Object *enemyObj = NULL;
	{
		Enemy *pEnemy = (Enemy*)enemyObj;
		if (pEnemy->getCurrActionState() == ACTION_STATE_ATTACK)
		{
			pEnemy->setPositionY(my_hero->getPositionY());
			BoundingBox heroBodyBox = my_hero->getBodyBox();
			BoundingBox enemyHitBox = pEnemy->getHitBox();
			if (collisionDetection(enemyHitBox, heroBodyBox))
			{
				my_hero->runHurtAction();
			}
		}
	}
}

void Enemy::FollowRun(Player* m_hero, Node* m_map)
{
	my_hero = m_hero;
	my_map = m_map;
	auto m_heroX = m_hero->getPositionX();
	auto m_heroY = m_hero->getPositionY();
	auto m_heroPoint = m_hero->getPosition();

	float x = m_heroX - this->getPositionX();
	float y = m_heroY - this->getPositionY();

	dis = sqrt(pow(x, 2) + pow(y, 2));

	if (dis < 150 && dis>45)
	{
		if (m_heroX + 20 < this->getPositionX())
		{
			this->setFlippedX(true);
			this->runWalkAction();
			this->setPosition(Point(getPositionX() - MOVE_SPEED, getPositionY()));
		}
		else if (m_heroX - 20 > this->getPositionX())
		{
			this->setFlippedX(false);
			this->runWalkAction();
			this->setPosition(Point(getPositionX() + MOVE_SPEED, getPositionY()));
		}
		if (m_heroY + 10 < this->getPositionY())
		{
			this->runWalkAction();
			this->setPosition(Point(getPositionX(), getPositionY() - MOVE_SPEED));
		}
		else if (m_heroY - 10 > this->getPositionY())
		{
			this->runWalkAction();
			this->setPosition(Point(getPositionX(), getPositionY() + MOVE_SPEED));
		}
	}
	else if(dis<45)
	{
		if ((dis < 45 && (this->getPositionY() < m_heroY + 2 || this->getPositionY() > m_heroY - 2)))
		{
			if (!(getCurrActionState() == ACTION_STATE_HURT))
			{
				this->runAttackAction();
			}
		}
	}
}

void Enemy::MonsterSeeRun(Player* m_hero, Node* m_map)
{
	my_hero = m_hero;
	my_map = m_map;
	auto m_heroX = m_hero->getPositionX();
	auto m_heroY = m_hero->getPositionY();
	auto m_heroPoint = m_hero->getPosition();

	float x = m_heroX - this->getPositionX();
	float y = m_heroY - this->getPositionY();

	dis = sqrt(pow(x, 2) + pow(y, 2));

	if (dis >= 250)
	{
		if (time >= 2)
		{
			if (isFlippedX())
			{
				setFlippedX(false);
				time = 0;
			}
			else if (!isFlippedX())
			{
				setFlippedX(true);
				time = 0;
			}
		}
		if (isFlippedX())
		{
			this->runWalkAction();
			setPosition(Point(this->getPositionX() - RUN_SPEED, this->getPositionY()));
		}
		else if (!isFlippedX())
		{
			this->runWalkAction();
			setPosition(Point(this->getPositionX() + RUN_SPEED, this->getPositionY()));
		}
	}
}

void Enemy::update(float dt)
{
	if (m_currActionState == ACTION_STATE_DEAD)
	{
		unschedule(schedule_selector(Enemy::update));
	}
	time += 0.07;
	if (m_currActionState != ACTION_STATE_HURT)
	{
		FollowRun(my_hero, my_map);
	}
	MonsterSeeRun(my_hero, my_map);
}