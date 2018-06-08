#include "Start.h"
#include "HelloWorldScene.h"
//#include "TollgateScene.h"
#include "cstdlib"
#include <math.h>

#define k_w (EventKeyboard::KeyCode)146
#define k_a (EventKeyboard::KeyCode)124
#define k_s (EventKeyboard::KeyCode)142
#define k_d (EventKeyboard::KeyCode)127
#define ENNUMBER 20;
#define MAP_SIZE 1600
#define MAP1_WIDTH 49
#define MAP1_HEIGHT 49

#define GAP_GID 145
#define NOR_GID 138
#define HP_GID 137
USING_NS_CC;

std::vector<Sprite*> StartScene::hp_potion;


extern bool language_flag;  //true->English   false->Chinese
extern int is_paused;       //关于is_paused的具体解释请见 "HelloWorldScene.h"
extern char *FontToUTF8(const char* font);
//it is define in another .cpp file 
//and it is used to change character

Scene* StartScene::createScene()
{
	is_paused = reply_music;   //进入正式游戏后吧初始音乐设为reply_music
	//正式进入游戏后会切换到新的游戏音乐，并把欢迎界面的音乐设为stop
	//再次返回到欢迎界面的时候回从头播放音乐
	//////////////////////////////////
	auto scene = StartScene::create();
	
	return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool StartScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	MapPrinter();
	ScenePrinter();
	
	return true;
}

void StartScene::MapPrinter()
{
	size = Director::getInstance()->getVisibleSize();
	//打开第一张瓦片地图
	tiledmap = TMXTiledMap::create("ArcherBattle_TiledMap_1.tmx");
	this->addChild(tiledmap);
	//////////////////////////////////////////
	mapSize = tiledmap->getMapSize();      // 获取以tiles数量为单位的地图尺寸
	tileSize = tiledmap->getTileSize();    // 获取以像素点为单位的tile尺寸属性
	log("tileSize %f %f", tileSize.width, tileSize.height);
	/////////////////////////////////////////
	//将meta设置为属性层
	meta = tiledmap->layerNamed("meta");
	meta->setVisible(false);
	////////////////////////////////////////
	//获取HP和MP对象层
	HP_objects = tiledmap->getObjectGroup("HP");
	
	/*int metax, metay, spritex, spritey;
	for (int i = 0; i < 10;)
	{
		srand(time(NULL));
		metax = rand() % MAP1_WIDTH;
		metay = rand() % MAP1_HEIGHT;
		int gid = meta->getTileGIDAt(Vec2(metax, metay));
		if (GAP_GID != gid && HP_GID != gid)
		{
			++i;
			meta->setTileGID(HP_GID, Vec2(metax, metay));
			spritex = metax * tileSize.width;
			spritey = (mapSize.height - metay)*tileSize.height;
			hp_potion.push_back(Sprite::create("HP_tiledmap.png"));
			hp_potion[hp_potion.size() - 1]->ignoreAnchorPointForPosition(false);
			hp_potion[hp_potion.size() - 1]->setAnchorPoint(Vec2(0.0f, 1.0f));
			hp_potion[hp_potion.size() - 1]->setPosition(Vec2(spritex, spritey));
			hp_potion[hp_potion.size() - 1]->setTag(i);
			tiledmap->addChild(hp_potion[hp_potion.size() - 1]);
		}
	}*/

	//std::vector<Sprite*> sprite;
	//for (auto&enemy : HP_objects->getObjects()) {
	//	// 获取对象的属性  
	//	ValueMap& dict = enemy.asValueMap();
	//	if (dict["HP"].asString() == "HP") { // 自定义属性“Enemy”  
	//		float x = dict["x"].asFloat();     // x坐标  
	//		float y = dict["y"].asFloat();      // y坐标  
	//		sprite.push_back(Sprite::create("HP_tiledmap.png"));
	//		sprite[sprite.size()-1]->ignoreAnchorPointForPosition(false);
	//		sprite[sprite.size() - 1]->setAnchorPoint(Vec2(0.0f, 1.0f));
	//		sprite[sprite.size() - 1]->setPosition(Vec2(x, y));
	//		CCLOG("x = %f,y = %f", x, y);
	//		x = x / tileSize.width;
	//		y = (mapSize.height*tileSize.height - y) / tileSize.height;
	//		CCLOG("x = %f,y = %f", x, y);
	//		meta->setTileGID(GAP_GID, Vec2(x, y));
	//		
	//		tiledmap->addChild(sprite[sprite.size() - 1]);
	//	}
	//}
	
}

void StartScene::ScenePrinter()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
	float x = rect.origin.x + rect.size.width / 2;
	float y = rect.origin.y + rect.size.height / 2;
	
	///////////////////////////////////
	//a return button which click to back to HelloWorldScene
	auto *return_button = MenuItemImage::create(
		"backtoupper.png",
		"backtoupper_select.png",
		CC_CALLBACK_1(StartScene::menuHellowWorldScene, this));

	auto *preturn = Menu::create(return_button, NULL);
	x = rect.origin.x + rect.size.width*(10.0f / 11.0f);
	y = rect.origin.y + rect.size.height*(1.0f / 10.0f);
	preturn->setPosition(Vec2(x, y));

	preturn->setScale(1.0f);
	this->addChild(preturn,100);   //把返回按钮置于100层，防止遮挡

	m_player = Player::create();
	m_player->bindSprite(Sprite::create("player1.png"));
	m_player->setScale(0.5f);
	m_player->ignoreAnchorPointForPosition(false);
	m_player->setAnchorPoint(Vec2(0.5f, 0.5f));
	m_player->setPosition(Point(m_player->x_coord, m_player->y_coord));
	tiledmap->addChild(m_player,10);

	n_player = Player::create();
	n_player->bindSprite(Sprite::create("player2.png"));
	n_player->setScale(0.5f);
	n_player->x_coord += 1000;
	n_player->y_coord += 1000;
	n_player->setPosition(Point(n_player->x_coord, n_player->y_coord));
	tiledmap->addChild(n_player,10);

	plsum.push_back(m_player);
	plsum.push_back(n_player);

	this->scheduleUpdate();
	/*
	////////////////////////////////////////
	//add a cover to draw something
	auto *cover = CCLayerColor::create(ccc4(0, 0, 0, 150));  //cccv的第四个参数取值0~225，越大越不透明
	this->addChild(cover, 1);
	////////////////////////////////////////
	//starting cortoon淡入淡出
	auto *actionFade = CCFadeOut::create(3.0f);
	auto *actionFadeBack = actionFade->reverse();
	auto *sequence1 = CCSequence::create(actionFade, actionFadeBack, NULL);
	//starting cortoon 颜色变化
	auto *actionTint = CCTintBy::create(2, -255, -127, -50);
	auto *actionTinback = actionTint->reverse();
	auto *sequence2 = CCSequence::create(actionTint, actionTint, NULL);
	//chose the sequence that you prefer
	cover->runAction(sequence2);*/
	
	/*sprite = Sprite::create("sprite.png");
	tiledmap->addChild(sprite, 10); 
	sprite->setPosition(Vec2(80.0f, 80.0f));

	auto* pLeft = MenuItemImage::create("left.png", "left1.png", this, menu_selector(StartScene::left));
	auto* left = Menu::create(pLeft, NULL);
	x = rect.origin.x + rect.size.width*(2.0f / 32.0f);
	y = rect.origin.y + rect.size.height*(3.0f / 16.0f);
	left->setPosition(Vec2(x, y));
	this->addChild(left);

	auto* pUp = MenuItemImage::create("up.png", "up1.png", this, menu_selector(StartScene::up));
	auto* up = Menu::create(pUp, NULL);
	x = rect.origin.x + rect.size.width*(3.0f / 32.0f);
	y = rect.origin.y + rect.size.height*(1.0f / 4.0f);
	up->setPosition(Vec2(x, y));
	this->addChild(up);

	auto* pRight = MenuItemImage::create("right.png", "right1.png", this, menu_selector(StartScene::right));
	auto* right = Menu::create(pRight, NULL);
	x = rect.origin.x + rect.size.width*(4.0f / 32.0f);
	y = rect.origin.y + rect.size.height*(3.0f / 16.0f);
	right->setPosition(Vec2(x, y));
	this->addChild(right);

	auto* pDown = MenuItemImage::create("down.png", "down1.png", this, menu_selector(StartScene::down));
	auto* down = Menu::create(pDown, NULL);
	x = rect.origin.x + rect.size.width*(3.0f / 32.0f);
	y = rect.origin.y + rect.size.height*(1.0f / 8.0f);
	down->setPosition(Vec2(x, y));
	this->addChild(down);
	isCanReach(sprite->getPositionX(), sprite->getPositionY());*/

}
bool StartScene::up(bool flag)
{
	float x = m_player->getPositionX(), y = m_player->getPositionY();
	if (y+32<MAP_SIZE&&isCanReach(x, y+16))
	{	//如果精灵上面那格不是地图的上边界
		//之所以是一格大小的一半,是因为精灵的锚点在中心,上面一个的下边界只需要再加16
		//sprite->setPositionY(y + 32);  //把精灵置于上面一格的位置
		if (flag)
		{
			runEvent();
			HPjudge(Vec2(x / tileSize.width,
				(mapSize.height*tileSize.height - y) / tileSize.height));
			if ((y + tiledmap->getPositionY() > size.height / 2) && ((MAP_SIZE - y) > size.height / 2))
			{ //调整地图,使人物尽量居中
				tiledmap->setPositionY(tiledmap->getPositionY() - 5);
				y_move += 5;
			}
		}
		return true;
	}
	return false;
}
bool StartScene::right(bool flag)
{
	float x = m_player->getPositionX(), y = m_player->getPositionY();
	if (x + 32 < MAP_SIZE&&isCanReach(x + 32, y))
	{
		if (flag)
		{
			runEvent();
			HPjudge(Vec2(x / tileSize.width,
				(mapSize.height*tileSize.height - y) / tileSize.height));
			if ((x + tiledmap->getPositionX() > size.width / 2) && ((MAP_SIZE - x) > size.width / 2))
			{
				tiledmap->setPositionX(tiledmap->getPositionX() - 5);
				x_move += 5;
			}
		}
		return true;
	}
	return false;
}
bool StartScene::left(bool flag)
{
	float x = m_player->getPositionX(), y = m_player->getPositionY();
	if (x>32&&isCanReach(x-16, y))
	{
		if (flag)
		{
			runEvent();
			HPjudge(Vec2(x / tileSize.width,
				(mapSize.height*tileSize.height - y) / tileSize.height));
			if ((x + tiledmap->getPositionX() < size.width / 2) && tiledmap->getPositionX() != 0)
			{
				tiledmap->setPositionX(tiledmap->getPositionX() + 5);
				x_move -= 5;
			}
		}
		return true;
	}
	return false;
}
bool StartScene::down(bool flag)
{
	float x = m_player->getPositionX(), y = m_player->getPositionY();
	if (y > 32&&isCanReach(x, y-32))
	{
		if (flag)
		{
			runEvent();
			HPjudge(Vec2(x / tileSize.width,
				(mapSize.height*tileSize.height - y) / tileSize.height));
			if ((y + tiledmap->getPositionY() < size.height / 2) && tiledmap->getPositionY() != 0)
			{
				tiledmap->setPositionY(tiledmap->getPositionY() + 5);
				y_move -= 5;
			}
		}
		return true;
	}
	return false;
}
bool StartScene::isCanReach(float x, float y)
{
	bool result;
	int mapX = (int)((x - 16) / 32);        //减去16是由于人物的锚点在中心
	int mapY = (int)(49 - (y - 16) / 32);   //49为Tiled里地图的坐标最大值
	int tileGid = meta->tileGIDAt(Vec2(mapX, mapY)); //32是一格的大小
	if (tileGid != GAP_GID)
	{
		result = true;
	}
	else
	{
		result = false;
	}
	return result;
}
void StartScene::HPjudge(const Vec2 &pos)
{
	if (HP_GID == meta->getTileGIDAt(pos))
	{
		CCLOG("hero is in HP_tiledmap");
		meta->setTileGID(NOR_GID, Vec2(static_cast<int>(pos.x),static_cast<int>(pos.y)));

	}

}
void StartScene::menuHellowWorldScene(Ref* pSender)
{
	auto sc = HelloWorld::createScene();        //缩放交替的切换动画
	auto reScene = TransitionShrinkGrow::create(1.0f, sc);
	Director::getInstance()->replaceScene(reScene);
}

//我也不知道onEnter是什么意思只是照着抄的，只知道这里是监控室
void StartScene::onEnter()
{
    Scene::onEnter();

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

	EventDispatcher *eventDispatcher2 = Director::getInstance()->getEventDispatcher();
	eventDispatcher2->addEventListenerWithSceneGraphPriority(touchlistener, this);

}

void StartScene::update(float delta)
{
	float x = m_player->getPositionX(), y = m_player->getPositionY();
	m_player->x_coord = x; m_player->y_coord = y;
	//CCLOG("x=%f , y=%f", x, y);

	if (keys[k_w] || keys[k_a] || keys[k_s] || keys[k_d])//分别是wasd，参见#define
	{
		if (keys[k_w] && keys[k_d])
		{
			if(up(false) && right(false))
			{
				up(true);
			}
		}
		else if (keys[k_w] && keys[k_a])
		{
			if (up(false) && left(false))
			{
				up(true);
			}
		}
		else if (keys[k_a] && keys[k_s])
		{
			if (left(false) && down(false))
			{
				down(true);
			}
		}
		else if (keys[k_s] && keys[k_d])
		{
			if (down(false) && right(false))
			{
				down(true);
			}
		}
		else if (keys[k_w])
		{
			up(true);
		}
		else if (keys[k_a])
		{
			left(true);
		}
		else if (keys[k_s])
		{
			down(true);
		}
		else if (keys[k_d])
		{
			right(true);
		}
	}

	if (touchon)
	{
		attack();
		touchon = false;
	}
	//鼠标点击事件，通过前面这几行操作可以使单击一次时仅攻击一次，bug已修复

	std::vector<BulletBase*>::iterator it;
	for (it = bubsum.begin(); it != bubsum.end();)
	{
		if ((*it)->exist != true)
		{
			it = bubsum.erase(it);
						log("delete");
		}
		else it++;
	}
	for (auto bub : bubsum)
	{
		for (auto pl : plsum)
		{
			bub->collidePlayer(pl);
		}
	}

}
//主角跑动的函数，不恒居中因为场景这一块不是我写的……到时候看着改吧
void StartScene::runEvent()
{
		m_player->run(m_player, keys);
}

void StartScene::attack()
{
	log("attack in x = %f  y = %f", pos.x+x_move, pos.y+y_move);
	log("player in x = %f  y = %f", m_player->x_coord, m_player->y_coord);
	log("sourse in x = %f  y = %f", m_player->x_coord, m_player->y_coord);
	pos.x += x_move; pos.y += y_move;
	auto Abullet = BulletBase::create();
	Abullet->bindSprite(Sprite::create("bullet.png"));
	Abullet->setPosition(Point(m_player->x_coord-x_move, m_player->y_coord-y_move));
	this->addChild(Abullet);

	Abullet->exist = true;
	bubsum.push_back(Abullet);

	Abullet->attacking(m_player, Abullet, pos);
}