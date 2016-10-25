#include "GameScene.h"
#include "Hero\Hero.h"
#include "enemy\enemy.h"
#include "Manager\EnemyManager.h"
#include "JoyStick\JoyStick.h"

USING_NS_CC;

#define MOVE_SPEED  2.5
Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto rootNode = CSLoader::createNode("MainScene.csb");
	addChild(rootNode);
	SpriteFrameCache* pFrameCache = SpriteFrameCache::sharedSpriteFrameCache();
	pFrameCache->addSpriteFramesWithFile("pd_sprites.plist");

	listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(GameScene::keyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(GameScene::keyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	hero_judgement = true;

	map = TMXTiledMap::create("pd_tilemap.tmx");
	addChild(map, 0);


	auto winSize = Director::getInstance()->getWinSize();
	JoyStick* joy = JoyStick::create();
	joy->setAnchorPoint(ccp(0,0));
	joy->setPosition(Point(0,0));
	joy->ignoreAnchorPointForPosition(false);
	addChild(joy);
	
	m_pHero = joy->getHero();
	m_pHero->setPosition(Point(50,50));
	addChild(m_pHero);

	manager = EnemyManager::getInstace(m_pHero, map);
	addChild(manager);
	enemyList = manager->getEnemyList();

	schedule(schedule_selector(GameScene::ZOrderUpdate), 0.1f);

	joy->onHeroAttack = [this]()
	{
		if (m_pHero->getCurrActionState() != ACTION_STATE_WALK)
		{
			m_pHero->runAttackAction();
			if (m_pHero->getCurrActionState() == ACTION_STATE_ATTACK)
			{
				Object* enemyObj = NULL;
				{
					Enemy* enemyL;
					for (size_t i = 0; i < enemyList.size(); i++)
					{
						enemyL = enemyList.at(i);
						Enemy *pEnemy = (Enemy*)enemyObj;
						if (fabsf(m_pHero->getPosition().y - enemyL->getPosition().y) < 10)
						{
							BoundingBox heroHitBox = m_pHero->getHitBox();
							BoundingBox enemyBodyBox = enemyL->getBodyBox();
							if (collisionDetection(heroHitBox, enemyBodyBox))
							{
								enemyL->setHP(enemyL->getHP() - m_pHero->getAttack());
								enemyL->runHurtAction();
								auto heroHP = m_pHero->getHP();
								auto heroATK = m_pHero->getAttack();
								auto enemyHP = enemyL->getHP();
								auto enemyATK = enemyL->getAttack();
								if (enemyL->getHP() <= 0)
								{
									enemyL->runDeadAction();
								}
							}
						}
					}
				}
			}
		}

	};
	return true;
}

void GameScene::keyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (m_pHero->getCurrActionState() != ACTION_STATE_ATTACK)
	{
		switch (keyCode)
		{
		case cocos2d::EventKeyboard::KeyCode::KEY_J:
		{
			if (m_pHero->getCurrActionState() != ACTION_STATE_WALK)
			{
				m_pHero->runAttackAction();
				if (m_pHero->getCurrActionState() == ACTION_STATE_ATTACK)
				{
					Object* enemyObj = NULL;
					{
						Enemy* enemyL;
						for (size_t i = 0; i < enemyList.size(); i++)
						{
							enemyL = enemyList.at(i);
							Enemy *pEnemy = (Enemy*)enemyObj;
							if (fabsf(m_pHero->getPosition().y - enemyL->getPosition().y) < 10)
							{
								BoundingBox heroHitBox = m_pHero->getHitBox();
								BoundingBox enemyBodyBox = enemyL->getBodyBox();
								if (collisionDetection(heroHitBox, enemyBodyBox))
								{
									enemyL->setHP(enemyL->getHP() - m_pHero->getAttack());
									enemyL->runHurtAction();
									auto heroHP = m_pHero->getHP();
									auto heroATK = m_pHero->getAttack();
									auto enemyHP = enemyL->getHP();
									auto enemyATK = enemyL->getAttack();
									if (enemyL->getHP() <= 0)
									{
										enemyL->runDeadAction();
									}
								}
							}
						}
					}
				}
			}
			
		}
		break;
		case cocos2d::EventKeyboard::KeyCode::KEY_D:
		{
			m_pHero->setFlippedX(false);
			m_pHero->runWalkAction();
			schedule(schedule_selector(GameScene::move_d), 0.01);
		}
		break;
		case cocos2d::EventKeyboard::KeyCode::KEY_S:
		{
			m_pHero->runWalkAction();
			schedule(schedule_selector(GameScene::move_s), 0.01);
		}
		break;
		case cocos2d::EventKeyboard::KeyCode::KEY_A:
		{
			m_pHero->setFlippedX(true);
			m_pHero->runWalkAction();
			schedule(schedule_selector(GameScene::move_a), 0.01);
			
		}
		break;
		case cocos2d::EventKeyboard::KeyCode::KEY_W:
		{
			m_pHero->runWalkAction();
			schedule(schedule_selector(GameScene::move_w), 0.01);
		}
		break;
		case cocos2d::EventKeyboard::KeyCode::KEY_K:
		{
			
		}
		break;
		}
	}
}

void GameScene::keyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_J:
	{
	}
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
	{
		attack_flag = false;
		this->unschedule(schedule_selector(GameScene::move_d));
		m_pHero->runIdleAction();
		
	}
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
	{
		this->unschedule(schedule_selector(GameScene::move_s));
		m_pHero->runIdleAction();
	}
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
	{
		attack_flag = false;
		this->unschedule(schedule_selector(GameScene::move_a));
		m_pHero->runIdleAction();
	}
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
	{
		this->unschedule(schedule_selector(GameScene::move_w));
		m_pHero->runIdleAction();
	}
		break;
	}
}

void GameScene::move_d(float dalta)
{
	if (m_pHero->getPositionX() > visibleSize.width - 30)
	{
		m_pHero->setPosition(Point(m_pHero->getPositionX(), m_pHero->getPositionY()));
	}
	else
	{
		m_pHero->setPosition(Point(m_pHero->getPositionX() + MOVE_SPEED, m_pHero->getPositionY()));
	}
}

void GameScene::move_s(float dalta)
{
	if (m_pHero->getPositionY() < 45)
	{
		m_pHero->setPosition(Point(m_pHero->getPositionX(), m_pHero->getPositionY()));
	}
	else
	{
		m_pHero->setPosition(Point(m_pHero->getPositionX(), m_pHero->getPositionY() - MOVE_SPEED));
	}
}
void GameScene::move_a(float dalta)
{
	if (m_pHero->getPositionX() < 30)
	{
		m_pHero->setPosition(Point(m_pHero->getPositionX(), m_pHero->getPositionY()));
	}
	else
	{
		m_pHero->setPosition(Point(m_pHero->getPositionX() - MOVE_SPEED, m_pHero->getPositionY()));
	}

}
void GameScene::move_w(float dalta)
{
	if (m_pHero->getPositionY() > 125)
	{
		//m_pHero->runAction(MoveBy::create((0.1), Point(0, 0)));
		m_pHero->setPosition(Point(m_pHero->getPositionX(), m_pHero->getPositionY()));
	}
	else
	{
		//m_pHero->runAction(MoveBy::create((0.1), Point(0, 10)));
		m_pHero->setPosition(Point(m_pHero->getPositionX(), m_pHero->getPositionY() + MOVE_SPEED));
	}
}
void GameScene::jump()
{
	hero->runAction(JumpBy::create(0.75, Point(0,0), 95, 1));
}


void GameScene::updateEnemy(float dt)
{

}

void GameScene::ZOrderUpdate(float dt)
{
	m_pHero->setGlobalZOrder(visibleSize.height - m_pHero->getPositionY());
	for (int i = 0; i < enemyList.size(); i++)
	{
		Enemy* enemy = enemyList.at(i);
		enemy->setGlobalZOrder(visibleSize.height - enemy->getPositionY());
	}	
}

bool GameScene::collisionDetection(const BoundingBox &hitBox, const BoundingBox &bodyBox)
{
	Rect hitRect = hitBox.actual;
	Rect bodyRect = bodyBox.actual;
	if (hitRect.intersectsRect(bodyRect))
	{
		return true;
	}
	return false;
}

void GameScene::addEnemy()
{

}

void GameScene::onEnemyAttack(BaseSprite* pSprite)
{

}

