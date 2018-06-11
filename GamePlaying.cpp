#include "GamePlaying.h"
#include "Start.h"
#include <cstdlib>
#include <math.h>
#include <vector>
#include <algorithm>

#define k_w (EventKeyboard::KeyCode)146
#define k_a (EventKeyboard::KeyCode)124
#define k_s (EventKeyboard::KeyCode)142
#define k_d (EventKeyboard::KeyCode)127

#define MAP_SIZE 1600
#define MAP1_WIDTH 49
#define MAP1_HEIGHT 49

#define DIFF 16   //人物和墙壁间的距离差值（比较玄学的测试，主要用于检测某一个方向是否有不可以走的地方
#define MAP_MOVE 5  //用于人物不在中央时，地图的位置移动和鼠标点击坐标的移动
#define GAP_GID 145
#define NOR_GID 138
#define HP_GID 137
#define EXP_GID 142
USING_NS_CC;

std::vector<HP_MESS> GamePlaying::hp_auto_arise;   //用于储存随机安置的回血道具的相关信息
std::vector<EXP_MESS> GamePlaying::exp_auto_arise;   //用于储存随机安置的经验道具的相关信息


extern bool language_flag;  //true->English   false->Chinese
extern int is_paused;       //关于is_paused的具体解释请见 "HelloWorldScene.h"
extern char *FontToUTF8(const char* font);
//it is define in another .cpp file 
//and it is used to change character

Scene* GamePlaying::createScene()
{
	auto scene = GamePlaying::create();

	return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GamePlaying::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	MapPrinter();
	ScenePrinter();

	schedule(schedule_selector(GamePlaying::EXP_grow), 0.1f);
	schedule(schedule_selector(GamePlaying::HP_grow), 1.5f);

	return true;
}

void GamePlaying::MapPrinter()
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


}

void GamePlaying::ScenePrinter()
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
		CC_CALLBACK_1(GamePlaying::menuStartScene, this));

	auto *preturn = Menu::create(return_button, NULL);
	x = rect.origin.x + rect.size.width*(10.0f / 11.0f);
	y = rect.origin.y + rect.size.height*(1.0f / 10.0f);
	preturn->setPosition(Vec2(x, y));

	preturn->setScale(1.0f);
	this->addChild(preturn, 100);   //把返回按钮置于100层，防止遮挡

	m_player = Player::create();
	m_player->bindSprite(Sprite::create("player1.png"));
	m_player->setScale(0.5f);
	//m_player->ignoreAnchorPointForPosition(true);
	m_player->setAnchorPoint(Vec2(0.0f, 0.0f));
	m_player->setPosition(Point(m_player->x_coord, m_player->y_coord));
	tiledmap->addChild(m_player, 10);

	n_player = Player::create();
	n_player->bindSprite(Sprite::create("player2.png"));
	n_player->setScale(0.5f);
	n_player->x_coord += 900;
	n_player->y_coord += 900;
	n_player->setPosition(Point(n_player->x_coord, n_player->y_coord));
	tiledmap->addChild(n_player, 10);

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


}
bool GamePlaying::up(bool flag)
{
	float x = m_player->getPositionX(), y = m_player->getPositionY();
	if (y + tileSize.height < MAP_SIZE && isCanReach(x, y + DIFF))
	{	//如果精灵上面那格不是地图的上边界
		//之所以是一格大小的一半,是因为精灵的锚点在中心,上面一个的下边界只需要再加16
		//sprite->setPositionY(y + 32);  //把精灵置于上面一格的位置
		if (flag)
		{
			runEvent();
			HPjudge(Vec2(x / tileSize.width,
				(mapSize.height*tileSize.height - y) / tileSize.height));
			EXPjudge(Vec2(x / tileSize.width,
				(mapSize.height*tileSize.height - y) / tileSize.height));
		}
		if ((y + tiledmap->getPositionY() > size.height / 2) && ((MAP_SIZE - y) > size.height / 2))
		{ //调整地图,使人物尽量居中
			tiledmap->setPositionY(tiledmap->getPositionY() - MAP_MOVE);
			y_move += MAP_MOVE;
		}
		return true;
	}
	return false;
}
bool GamePlaying::right(bool flag)
{
	float x = m_player->getPositionX(), y = m_player->getPositionY();
	if (x + tileSize.width < MAP_SIZE && isCanReach(x + 3 * DIFF, y))
	{
		if (flag)
		{
			runEvent();
			HPjudge(Vec2(x / tileSize.width,
				(mapSize.height*tileSize.height - y) / tileSize.height));
			EXPjudge(Vec2(x / tileSize.width,
				(mapSize.height*tileSize.height - y) / tileSize.height));
		}
		if ((x + tiledmap->getPositionX() > size.width / 2) && ((MAP_SIZE - x) > size.width / 2))
		{
			tiledmap->setPositionX(tiledmap->getPositionX() - MAP_MOVE);
			x_move += MAP_MOVE;
		}
		return true;
	}
	return false;
}
bool GamePlaying::left(bool flag)
{
	float x = m_player->getPositionX(), y = m_player->getPositionY();
	if (x>tileSize.width && isCanReach(x - DIFF, y))
	{
		if (flag)
		{
			runEvent();
			HPjudge(Vec2(x / tileSize.width,
				(mapSize.height*tileSize.height - y) / tileSize.height));
			EXPjudge(Vec2(x / tileSize.width,
				(mapSize.height*tileSize.height - y) / tileSize.height));
		}
		if ((x + tiledmap->getPositionX() < size.width / 2) && tiledmap->getPositionX() != 0)
		{
			tiledmap->setPositionX(tiledmap->getPositionX() + MAP_MOVE);
			x_move -= MAP_MOVE;
		}
		return true;
	}
	return false;
}
bool GamePlaying::down(bool flag)
{
	float x = m_player->getPositionX(), y = m_player->getPositionY();
	if (y>tileSize.height && isCanReach(x, y - 3 * DIFF))
	{
		if (flag)
		{
			runEvent();
			HPjudge(Vec2(x / tileSize.width,
				(mapSize.height*tileSize.height - y) / tileSize.height));
			EXPjudge(Vec2(x / tileSize.width,
				(mapSize.height*tileSize.height - y) / tileSize.height));

		}
		if ((y + tiledmap->getPositionY() < size.height / 2) && tiledmap->getPositionY() != 0)
		{
			tiledmap->setPositionY(tiledmap->getPositionY() + MAP_MOVE);
			y_move -= MAP_MOVE;
		}
		return true;
	}
	return false;
}
bool GamePlaying::isCanReach(float x, float y)
{
	bool result;
	int mapX = (int)((x - DIFF) / 32);        //减去16是由于人物的锚点在中心
	int mapY = (int)(MAP1_HEIGHT - (y - DIFF) / 32);   //49为Tiled里地图的坐标最大值
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

void GamePlaying::menuStartScene(Ref* pSender)
{
	auto sc = StartScene::createScene();        //缩放交替的切换动画
	auto reScene = TransitionShrinkGrow::create(1.0f, sc);
	Director::getInstance()->replaceScene(reScene);
}

void GamePlaying::HPjudge(const Vec2 &pos)
{
	if (HP_GID == meta->getTileGIDAt(pos))
	{
		CCLOG("hero is in HP_tiledmap");
		meta->setTileGID(NOR_GID, Vec2(static_cast<int>(pos.x), static_cast<int>(pos.y)));
		int judgex = static_cast<int>(pos.x), judgey = static_cast<int>(pos.y);
		//范围for的使用，用于找到当前接触的回血道具
		//将对应的精灵删除，并删除vector里相应的元素
		for (auto &hp_now : hp_auto_arise)
		{
			if (judgex == hp_now.savex && judgey == hp_now.savey)
			{
				hp_now.hp_potion->removeFromParentAndCleanup(true);
				auto hp_iter = std::find(hp_auto_arise.begin(), hp_auto_arise.end(), hp_now);
				hp_auto_arise.erase(hp_iter, hp_iter+1);
				
				break;
			}
		}

	}

}
void GamePlaying::HP_grow(float dt)
{
	/*static int time_locker = 0;
	++time_locker;
	if (time_locker > 20)
	{
	this->unschedule(schedule_selector(StartScene::HP_grow));
	}*/
	//限定场上回血道具的数量，超过30个的时候就先停止产生
	if (hp_auto_arise.size() > 30) { return; }
	int metax, metay;
	srand(time(NULL));
	//为了让回血道具产生的更稀疏（其实并没有什么差2333333                 
	metax = ((rand()%MAP1_WIDTH)*(rand()%MAP1_WIDTH)) % MAP1_WIDTH;
	metay = ((rand() % MAP1_HEIGHT)*(rand() % MAP1_HEIGHT)) % MAP1_HEIGHT;

	int gid = meta->getTileGIDAt(Vec2(1.0*metax, 1.0*metay));
	if (GAP_GID != gid && HP_GID != gid && EXP_GID != gid)
	{
		meta->setTileGID(HP_GID, Vec2(1.0*metax, 1.0*metay));

		//类的构造函数，添加一个回血道具
		hp_auto_arise.push_back(HP_MESS(Sprite::create("HP_tiledmap.png"), metax, metay));

		int now_vec_maxindex = hp_auto_arise.size() - 1;
		float spritex = metax * tileSize.width, spritey = (mapSize.height - metay)*tileSize.height;
		hp_auto_arise[now_vec_maxindex].hp_potion->ignoreAnchorPointForPosition(false);
		hp_auto_arise[now_vec_maxindex].hp_potion->setAnchorPoint(Vec2(0.0f, 1.0f));

		hp_auto_arise[now_vec_maxindex].hp_potion->setPosition(
			Vec2(spritex, spritey));
		tiledmap->addChild(hp_auto_arise[now_vec_maxindex].hp_potion);
	}
}
void GamePlaying::EXPjudge(const Vec2 & pos)
{
	if (EXP_GID == meta->getTileGIDAt(pos))
	{
		CCLOG("hero is in EXP_tiledmap");
		meta->setTileGID(NOR_GID, Vec2(static_cast<int>(pos.x), static_cast<int>(pos.y)));
		int judgex = static_cast<int>(pos.x), judgey = static_cast<int>(pos.y);

		for (auto &exp_now : exp_auto_arise)
		{
			if (judgex == exp_now.savex && judgey == exp_now.savey)
			{
				exp_now.exp_potion->removeFromParentAndCleanup(true);
				auto exp_iter = std::find(exp_auto_arise.begin(), exp_auto_arise.end(), exp_now);
				exp_auto_arise.erase(exp_iter, exp_iter + 1);

				break;
			}
		}

	}
}
void GamePlaying::EXP_grow(float dt)
{
	/*static int time_locker = 0;
	++time_locker;
	if (time_locker > 20)
	{
	this->unschedule(schedule_selector(StartScene::HP_grow));
	}*/
	//限定场上回血道具的数量，超过100个的时候就先停止产生
	if (exp_auto_arise.size() > 100) { return; }
	int metax, metay;
	srand(time(NULL));               
	metax = rand() % MAP1_WIDTH;
	metay = rand() % MAP1_HEIGHT;

	int gid = meta->getTileGIDAt(Vec2(1.0*metax, 1.0*metay));
	if (GAP_GID != gid && HP_GID != gid && EXP_GID != gid)
	{
		meta->setTileGID(EXP_GID, Vec2(1.0*metax, 1.0*metay));

		//类的构造函数，添加一个回血道具
		exp_auto_arise.push_back(EXP_MESS(Sprite::create("EXP_tiledmap.png"), metax, metay));

		int now_vec_maxindex = exp_auto_arise.size() - 1;
		float spritex = metax * tileSize.width, spritey = (mapSize.height - metay)*tileSize.height;
		exp_auto_arise[now_vec_maxindex].exp_potion->ignoreAnchorPointForPosition(false);
		exp_auto_arise[now_vec_maxindex].exp_potion->setAnchorPoint(Vec2(0.0f, 1.0f));

		exp_auto_arise[now_vec_maxindex].exp_potion->setPosition(
			Vec2(spritex, spritey));
		tiledmap->addChild(exp_auto_arise[now_vec_maxindex].exp_potion);
	}
}
//我也不知道onEnter是什么意思只是照着抄的，只知道这里是监控室
void GamePlaying::onEnter()
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

void GamePlaying::update(float delta)
{
	float x = m_player->getPositionX(), y = m_player->getPositionY();
	m_player->x_coord = x; m_player->y_coord = y;
	//CCLOG("x=%f , y=%f", x, y);

	if (keys[k_w] || keys[k_a] || keys[k_s] || keys[k_d])//分别是wasd，参见#define
	{
		//如果同时按了w和d，首先检测是否可以往上并且可以往右
		//方法是先将分方向判断函数的参数传递为false
		//这样只会检测是否可以走,不会实际调runEvent函数
		if (keys[k_w] && keys[k_d])  //如果同时按了w和d
		{
			bool flagup = up(false), flagright = right(false);
			if (flagup && flagright)			//如果往上和往右都可以走
			{
				up(true);              //虽然调用的是up(true)但是实际runEventl()里走的方向还是右上的
				right(false);		  //仅仅调up方向的移动函数还不够用,还需要通过调用right(false)来让地图能同步的移动
			}
			else if (flagup && !flagright)  //如果只是往上可以走，那表现的效果就是沿着墙往上跑
			{
				keys[k_d] = false;         //把右方向的键盘监听关掉，表现为相当于只按了w
				up(true);

			}
			else if (!flagup && flagright)
			{
				keys[k_w] = false;
				right(true);
			}

		}
		else if (keys[k_w] && keys[k_a])
		{
			bool flagup = up(false), flagleft = left(false);
			if (flagup && flagleft)
			{
				up(true);
				left(false);

			}
			else if (flagup && !flagleft)
			{
				keys[k_a] = false;
				up(true);
			}
			else if (!flagup && flagleft)
			{
				keys[k_w] = false;
				left(true);
			}
		}
		else if (keys[k_a] && keys[k_s])
		{
			bool flagleft = left(false), flagdown = down(false);
			if (flagleft && flagdown)
			{
				down(true);
				left(false);
			}
			else if (flagleft && !flagdown)
			{
				keys[k_s] = false;
				left(true);
			}
			else if (!flagleft && flagdown)
			{
				keys[k_a] = false;
				down(true);
			}
		}
		else if (keys[k_s] && keys[k_d])
		{
			bool flagdown = down(false), flagright = right(false);
			if (flagdown && flagright)
			{
				down(true);
				right(false);
			}
			else if (flagdown && !flagright)
			{
				keys[k_d] = false;
				down(true);
			}
			else if (!flagdown && flagright)
			{
				keys[k_s] = false;
				right(true);
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
void GamePlaying::runEvent()
{
	m_player->run(m_player, keys);
}

void GamePlaying::attack()
{
	log("attack in x = %f  y = %f", pos.x + x_move, pos.y + y_move);
	log("player in x = %f  y = %f", m_player->x_coord, m_player->y_coord);
	log("sourse in x = %f  y = %f", m_player->x_coord, m_player->y_coord);
	pos.x += x_move; pos.y += y_move;
	auto Abullet = BulletBase::create();
	Abullet->bindSprite(Sprite::create("bullet.png"));
	Abullet->setPosition(Point(m_player->x_coord - x_move, m_player->y_coord - y_move));
	this->addChild(Abullet);

	Abullet->exist = true;
	bubsum.push_back(Abullet);

	Abullet->attacking(m_player, Abullet, pos);
}

