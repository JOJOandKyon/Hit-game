#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "cocos2d.h"
#include "cocostudio\CocoStudio.h"
#include "Hero\Hero.h"
#include "Player\Player.h"
#include "enemy\enemy.h"
#include "Manager\EnemyManager.h"
using namespace cocos2d;

class GameScene : public cocos2d::Layer
{
public:
	static Scene* createScene();
private:
	virtual bool init();
	CREATE_FUNC(GameScene);
private:
	void keyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void keyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void move_d(float dalta);
	void move_s(float dalta);
	void move_a(float dalta);
	void move_w(float dalta);
	void jump();
	void updateEnemy(float dt);
	void addEnemy();
	void onEnemyAttack(BaseSprite* pSprite);
	void ZOrderUpdate(float dt);
	bool collisionDetection(const BoundingBox &hitBox, const BoundingBox &bodyBox);

private:
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Hero* hero;
	Vector<Enemy*> enemyList;
	EventListenerKeyboard* listener;
	TMXTiledMap* map;
	EnemyManager* manager;
	float m_tileWidth;
	float m_tileHeight;
	CC_SYNTHESIZE_READONLY(Player*, m_pHero, Player);
	SpriteBatchNode* m_pSpriteNodes;
	bool hero_judgement;
	bool attack_flag;

};
#endif