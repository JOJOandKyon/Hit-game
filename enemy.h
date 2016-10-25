#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "BaseSprite/BaseSprite.h"
#include "Player\Player.h"
#include "cocos2d.h"

using namespace cocos2d;
typedef enum{
	AI_IDLE = 0,
	AI_WALK,
	AI_PATROL,
	AI_ATTACK,
	AI_PURSUIT
}AiState;
class Enemy :public BaseSprite
{
public:
	Enemy();
	~Enemy();
	bool init();
	CREATE_FUNC(Enemy);
public:
	void execute(const cocos2d::Point& target, float targetBodyWidth);
	void updateEnemy(float dt);
	void FollowRun(Player* m_hero, Node* m_map);
	void JudegeAttack();
	void MonsterSeeRun(Player* m_hero, Node* m_map);
	void update(float dt);
private:
	void decide(const cocos2d::Point& target, float targetBodyWidth);
private:
	unsigned int m_nextDecisionTime;
	bool collisionDetection(const BoundingBox &hitBox, const BoundingBox &bodyBox)
	{
		Rect hitRect = hitBox.actual;
		Rect bodyRect = bodyBox.actual;
		if (hitRect.intersectsRect(bodyRect))
		{
			return true;
		}
		return false;
	}
private:
	Player* my_hero;
	Node* my_map;
	float dis;
	bool flag;
	float time;

};
#endif