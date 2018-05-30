#include "TollgateScene.h"
#include "Player.h"
#include "BulletBase.h"
#include <math.h>
#define k_w (EventKeyboard::KeyCode)119
#define k_a (EventKeyboard::KeyCode)97
#define k_s (EventKeyboard::KeyCode)115
#define k_d (EventKeyboard::KeyCode)100

Scene* TollgateScene::createScene()
{
	auto scene = Scene::create();
	auto layer = TollgateScene::create();

	scene->addChild(layer);

	return scene;
}

bool TollgateScene::init()
{
	if (!Layer::init())
		return false;

	Size visibleSize = CCDirector::getInstance()->getVisibleSize();

	m_player = Player::create();
	m_player->bindSprite(Sprite::create("player1.png"));
	m_player->setPosition(Point(m_player->x_coord, m_player->y_coord));
	this->addChild(m_player);

	n_player = Player::create();
	n_player->bindSprite(Sprite::create("player2.png"));
	n_player->setPosition(Point(n_player->x_coord, n_player->y_coord));
	this->addChild(n_player);

	this->scheduleUpdate();

	return true;
}
//我也不知道onEnter是什么意思只是照着抄的，只知道这里是监控室
void TollgateScene::onEnter()
{
	Layer::onEnter();

	auto keylistener = EventListenerKeyboard::create();
	//键盘监听器，用于人物移动
	keylistener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event *event)
	{
		keys[keyCode] = true;
	};

	keylistener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event *event)
	{
		keys[keyCode] = false;
	};

	EventDispatcher *eventDispatcher1 = Director::getInstance()->getEventDispatcher();
	eventDispatcher1->addEventListenerWithSceneGraphPriority(keylistener, this);


	auto touchlistener = EventListenerTouchOneByOne::create();
	//触摸监听器，用于人物攻击
	touchlistener->onTouchBegan = [&](Touch* touch, Event *event)
	{
		touchon = true;
		Point pos1 = touch->getLocationInView();
		Point pos2 = Director::getInstance()->convertToGL(pos1);
		pos = pos2;//得到单击坐标
		return true;
	};
	touchlistener->onTouchEnded = [&](Touch* toych, Event *event)
	{
		touchon = false;
	};

	EventDispatcher *eventDispatcher2 = Director::getInstance()->getEventDispatcher();
	eventDispatcher2->addEventListenerWithSceneGraphPriority(touchlistener, this);

}

void TollgateScene::update(float delta)
{
	if (keys[k_w] || keys[k_a] || keys[k_s] || keys[k_d])//分别是wasd，参见#define
	{
		runEvent();
	}

	if (!touchon)
	{
		once = true;
	}
	if (touchon&&once)
	{
		attack();
		once = false;
	}
	//鼠标点击事件，通过前面这几行操作可以使单击一次时仅攻击一次（原本想设计个攻击CD，因未知原因BUG，无果
}
//主角跑动的函数，不恒居中因为场景这一块不是我写的……到时候看着改吧
void TollgateScene::runEvent()
{
	m_player->run(m_player, keys);

}

void TollgateScene::attack()
{


	log("attack in x = %f  y = %f", pos.x, pos.y);
	auto Abullet = BulletBase::create();
	Abullet->bindSprite(Sprite::create("bullet.png"));
	Abullet->setPosition(Point(m_player->x_coord, m_player->y_coord));
	this->addChild(Abullet);

	Abullet->attacking(m_player, Abullet, pos);
}