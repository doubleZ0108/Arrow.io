#include "GamePlaying.h"
#include "SimpleAudioEngine.h"
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
#define GAP_GID 145
#define NOR_GID 138
#define HP_GID 137
#define EXP_GID 142

#define SM_MAP_SIZE 245
#define RETE (260.0/1605)  //smallplayer和player移动距离的比

USING_NS_CC;

std::vector<HP_MESS> GamePlaying::hp_auto_arise;   //用于储存随机安置的回血道具的相关信息
std::vector<EXP_MESS> GamePlaying::exp_auto_arise;   //用于储存随机安置的经验道具的相关信息

extern bool language_flag;  //true->English   false->Chinese
extern int is_paused;       //关于is_paused的具体解释请见 "HelloWorldScene.h"
extern char *FontToUTF8(const char* font);

bool smallmap_switch = true;       //小地图控制开关,true->打开小地图,false->关上小地图
								   //每次打开小地图的时候小人物的位置要随m_player做相应的调整
bool music_switch = true;
bool mode_switch = true;

int which_map = 1;
int which_player = 1;

bool magent = false;
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
	
	MusicPrinter();
	MapPrinter();
	ScenePrinter();
	SmallmapPrinter();
	ModePrinter();

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
}
void GamePlaying::ScenePrinter()
{
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

	///////////////////////////////////////////////
	PlayerPrinter();
	SettingPrinter();
	//////////////////////////////////////////////

	//磁铁石技能开关
	auto magnetMenuItem = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1( GamePlaying::Magent_change, this),
		MenuItemFont::create("Magnet"),
		MenuItemFont::create("Origin"),
		NULL);

	Menu* magnetmn = Menu::create(magnetMenuItem, NULL);
	x = rect.origin.x + rect.size.width*(33.7f / 40.0f);
	y = rect.origin.y + rect.size.height*(10.0f / 20.0f);
	magnetmn->setPosition(Vec2(x, y));
	this->addChild(magnetmn, 1);

	/*
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

void GamePlaying::PlayerPrinter()
{
	m_player->sprite = Sprite::create("player1.png");
	m_player->bindSprite(m_player->sprite);
	m_player->setScale(2.0f, 2.0f);
	m_player->sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
	m_player->setPosition(Point(m_player->x_coord, m_player->y_coord));
	tiledmap->addChild(m_player, 10);

	n_player->sprite = Sprite::create("player2.png");
	n_player->bindSprite(n_player->sprite);
	n_player->setScale(1.8, 1.8);
	n_player->setPosition(Point(n_player->x_coord, n_player->y_coord));
	tiledmap->addChild(n_player, 10);

	plsum.push_back(m_player);
	plsum.push_back(n_player);


	///////////////////////////////////////////////
	//血条初始化
	m_pProgressView = new ProgressView;
	m_pProgressView->setPosition(ccp(m_player->x_coord, m_player->y_coord + 50));
	m_pProgressView->setScale(2);
	m_pProgressView->setBackgroundTexture("background.png");
	m_pProgressView->setForegroundTexture("foreground.png");
	m_pProgressView->setTotalProgress(50);
	m_pProgressView->setCurrentProgress(50);
	tiledmap->addChild(m_pProgressView, 2);

	n_pProgressView = new ProgressView;
	n_pProgressView->setPosition(ccp(n_player->x_coord, n_player->y_coord + 50));
	n_pProgressView->setScale(2);
	n_pProgressView->setBackgroundTexture("background.png");
	n_pProgressView->setForegroundTexture("foreground.png");
	n_pProgressView->setTotalProgress(50);
	n_pProgressView->setCurrentProgress(50);
	tiledmap->addChild(n_pProgressView, 2);

	///////////////////////////////////////////////
	this->scheduleUpdate();
}
void GamePlaying::SettingPrinter()
{
	float x, y;
	////////////////////////////////////////////////
	//add small map
	Label *smallmapword;
	if (language_flag)
	{
		smallmapword = Label::createWithTTF("Global Map",
			"fonts/Marker Felt.ttf", 40);
	}
	else
	{
		smallmapword = Label::create(FontToUTF8("全局地图"),
			"Arial", 40);
	}
	x = rect.origin.x + rect.size.width*(33.7f / 40.0f);
	y = rect.origin.y + rect.size.height*(18.0f / 20.0f);
	smallmapword->setPosition(Vec2(x, y));
	this->addChild(smallmapword, 1);

	auto smallMenuItem = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(GamePlaying::Smallmap_Switch, this),
		MenuItemImage::create(
			"checkbox_selected.png",
			"checkbox_selected.png"),
		MenuItemImage::create(
			"checkbox_normal.png",
			"checkbox_normal.png"),
		NULL);
	Menu* smallmn = Menu::create(smallMenuItem, NULL);
	x = rect.origin.x + rect.size.width*(37.4f / 40.0f);
	smallmn->setPosition(Vec2(x, y));
	this->addChild(smallmn, 1);

	/////////////////////////////////////////////////
	//播放和暂停游戏音乐
	Label *musicword;
	if (language_flag)
	{
		musicword = Label::createWithTTF("Game Music",
			"fonts/Marker Felt.ttf", 40);
	}
	else
	{
		musicword = Label::create(FontToUTF8("游戏音乐"),
			"Arial", 40);
	}
	x = rect.origin.x + rect.size.width*(33.9f / 40.0f);
	y = rect.origin.y + rect.size.height*(17.0f / 20.0f);
	musicword->setPosition(Vec2(x, y));
	this->addChild(musicword, 1);

	auto musicMenuItem = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(GamePlaying::Music_Switch, this),
		MenuItemImage::create(
			"checkbox_selected.png",
			"checkbox_selected.png"),
		MenuItemImage::create(
			"checkbox_normal.png",
			"checkbox_normal.png"),
		NULL);
	Menu* musicmn = Menu::create(musicMenuItem, NULL);
	x = rect.origin.x + rect.size.width*(37.4f / 40.0f);
	musicmn->setPosition(Vec2(x, y));
	this->addChild(musicmn, 1);

	////////////////////////////////////////////////
	//切换不同的游戏模式
	Label *modeword;
	if (language_flag)
	{
		modeword = Label::createWithTTF("Game Mode",
			"fonts/Marker Felt.ttf", 40);
	}
	else
	{
		modeword = Label::create(FontToUTF8("游戏模式"),
			"Arial", 40);
	}
	x = rect.origin.x + rect.size.width*(33.9f / 40.0f);
	y = rect.origin.y + rect.size.height*(16.0f / 20.0f);
	modeword->setPosition(Vec2(x, y));
	this->addChild(modeword, 1);

	auto modeMenuItem = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(GamePlaying::Mode_Switch, this),
		MenuItemImage::create(
			"checkbox_selected.png",
			"checkbox_selected.png"),
		MenuItemImage::create(
			"checkbox_normal.png",
			"checkbox_normal.png"),
		NULL);
	Menu* modemn = Menu::create(modeMenuItem, NULL);
	x = rect.origin.x + rect.size.width*(37.4f / 40.0f);
	modemn->setPosition(Vec2(x, y));
	this->addChild(modemn, 1);
}

void GamePlaying::MusicPrinter()
{
	Label *OnorOff;
	float x, y;
	if (music_switch)
	{
		if (this->getChildByName("MUSIC_OFF"))
		{//逻辑链是这样的,如果有一个名字叫做MUSIC_OFF的精灵了
			//就证明已经不是init调的这个函数了
			//那就继续播放刚刚暂停的音乐
			this->getChildByName("MUSIC_OFF")->removeFromParentAndCleanup(true);
			CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
		}
		else
		{
			//init才会调到这,从头播放音乐
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Escape.mp3");
		}
		
		if (language_flag)
		{
			OnorOff = Label::createWithTTF("ON",
				"fonts/Marker Felt.ttf", 30);
		}
		else
		{
			OnorOff = Label::create(FontToUTF8("播放"),
				"Arial", 30);
		}
		OnorOff->setName("MUSIC_ON");
	}
	else
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		
		if (this->getChildByName("MUSIC_ON"))
		{
			this->getChildByName("MUSIC_ON")->removeFromParentAndCleanup(true);
		}
		if (language_flag)
		{
			OnorOff = Label::createWithTTF("OFF",
				"fonts/Marker Felt.ttf", 30);
		}
		else
		{
			OnorOff = Label::create(FontToUTF8("暂停"),
				"Arial", 30);
		}
		OnorOff->setName("MUSIC_OFF");
	}
	x = rect.origin.x + rect.size.width*(39.0f / 40.0f);
	y = rect.origin.y + rect.size.height*(17.0f / 20.0f);
	OnorOff->setPosition(Vec2(x, y));
	this->addChild(OnorOff, 1);
}
void GamePlaying::Music_Switch(Ref * pSender)
{
	music_switch = (music_switch ? false : true);
	MusicPrinter();
}

void GamePlaying::SmallmapPrinter()
{
	Label *OnorOff;
	float x, y;
	if (smallmap_switch)        //如果要打开小地图，则重新构建，因为每次的小人物位置不同
	{
		////////////////////////////////////////
		//add a smallmap to draw something and besides the smallmap is also a cover
		//cccv的第四个参数取值0~225，越大越不透明
		//m_smallmap = LayerColor::create(ccc4(0, 0, 0, 100), 250, 250);
		m_smallmap = Sprite::create("smallmap.png");
		m_smallmap->setOpacity(220);     //设置小地图的透明度
										 //m_smallmap->setColor(Color3B(0, 0, 205));
		m_smallmap->setAnchorPoint(Vec2(0.0f, 0.0f));
		x = rect.origin.x + rect.size.width*0.0f;
		y = rect.origin.y + rect.size.height*(2.0f / 3.0f - 0.01f);  //减0.01是为了消去一个极其小的位置偏差
		m_smallmap->setPosition(Vec2(x, y));
		this->addChild(m_smallmap, 1);

		m_smallplayer = Player::create();
		m_smallplayer->bindSprite(Sprite::create("smallplayer.png"));
		m_smallplayer->ignoreAnchorPointForPosition(false);
		//smallplayer->setAnchorPoint(Vec2(0.0f, 0.0f));
		m_smallplayer->setPosition(
			Vec2(m_player->getPositionX()*RETE, m_player->getPositionY()*RETE));
		m_smallmap->addChild(m_smallplayer);

		if (this->getChildByName("SMALLMAP_OFF"))
		{
			this->getChildByName("SMALLMAP_OFF")->removeFromParentAndCleanup(true);
		}
		if (language_flag)
		{
			OnorOff = Label::createWithTTF("ON",
				"fonts/Marker Felt.ttf", 30);
		}
		else
		{
			OnorOff = Label::create(FontToUTF8("开启"),
				"Arial", 30);
		}
		OnorOff->setName("SMALLMAP_ON");
	}
	else      //如果要关上就直接销毁精灵
	{
		if (m_smallmap)
		{
			m_smallmap->removeFromParentAndCleanup(true);
		}
		
		if (this->getChildByName("SMALLMAP_ON"))
		{
			this->getChildByName("SMALLMAP_ON")->removeFromParentAndCleanup(true);
		}
		if (language_flag)
		{
			OnorOff = Label::createWithTTF("OFF",
				"fonts/Marker Felt.ttf", 30);
		}
		else
		{
			OnorOff = Label::create(FontToUTF8("关闭"),
				"Arial", 30);
		}
		OnorOff->setName("SMALLMAP_OFF");
	}
	x = rect.origin.x + rect.size.width*(39.0f / 40.0f);
	y = rect.origin.y + rect.size.height*(18.0f / 20.0f);
	OnorOff->setPosition(Vec2(x, y));
	this->addChild(OnorOff, 1);
}
void GamePlaying::Smallmap_Switch(Ref* pSender)
{
	smallmap_switch = (smallmap_switch ? false : true);
	//将开着的小地图关上，将关着的小地图打开
	SmallmapPrinter();
}

void GamePlaying::ModePrinter()
{
	Label *OnorOff;
	float x, y;
	if (mode_switch)
	{
		if (this->getChildByName("MODE2"))
		{
			this->getChildByName("MODE2")->removeFromParentAndCleanup(true);
		}

		OnorOff = Label::createWithTTF("1",
			"fonts/Marker Felt.ttf", 30);
		OnorOff->setName("MODE1");
	}
	else
	{
		if (this->getChildByName("MODE1"))
		{
			this->getChildByName("MODE1")->removeFromParentAndCleanup(true);
		}

		OnorOff = Label::createWithTTF("2",
			"fonts/Marker Felt.ttf", 30);
		OnorOff->setName("MODE2");
	}
	x = rect.origin.x + rect.size.width*(39.0f / 40.0f);
	y = rect.origin.y + rect.size.height*(16.0f / 20.0f);
	OnorOff->setPosition(Vec2(x, y));
	this->addChild(OnorOff, 1);
}
void GamePlaying::Mode_Switch(Ref * pSender)
{
	waytorun = (waytorun ? false : true);
	mode_switch = (mode_switch ? false : true);
	ModePrinter();
	onEnter();
}

void GamePlaying::Magent_change(Ref * pSender)
{
	magent = (magent ? false : true);
}

bool GamePlaying::up(bool flag)
{
	float x = m_player->getPositionX(), y = m_player->getPositionY();
	if (y + tileSize.height < MAP_SIZE && isCanReach(x, y + DIFF - 8))   //往上的判断多+1消除卡墙bug
	{	//如果精灵上面那格不是地图的上边界
		//之所以是一格大小的一半,是因为精灵的锚点在中心,上面一个的下边界只需要再加16
		//sprite->setPositionY(y + 32);  //把精灵置于上面一格的位置
			if (flag)
			{
				runEvent();
				tofindEat(x, y);
			}
			if ((y + tiledmap->getPositionY() > size.height / 2) && ((MAP_SIZE - y) > size.height / 2))
			{   //调整地图,使人物尽量居中
				//地图移动速度与人物移动速度保持一直，获得最佳游戏体验，尽享丝滑
				tiledmap->setPositionY(tiledmap->getPositionY() - m_player->speed);
				y_move += m_player->speed;
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
			tofindEat(x, y);
		}
		if ((x + tiledmap->getPositionX() > size.width / 2) && ((MAP_SIZE - x) > size.width / 2))
		{
			tiledmap->setPositionX(tiledmap->getPositionX() - m_player->speed);
			x_move += m_player->speed;
		}
		return true;
	}
	return false;
}
bool GamePlaying::left(bool flag)
{
	float x = m_player->getPositionX(), y = m_player->getPositionY();
	if (x>tileSize.width && isCanReach(x - DIFF - 1, y))  //往左的判断多-1消除卡墙bug
	{
		if (flag)
		{
			runEvent();
			tofindEat(x, y);
		}
		if ((x + tiledmap->getPositionX() < size.width / 2) && tiledmap->getPositionX() != 0)
		{
			tiledmap->setPositionX(tiledmap->getPositionX() + m_player->speed);
			x_move -= m_player->speed;
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
			tofindEat(x, y);
		}
		if ((y + tiledmap->getPositionY() < size.height / 2) && tiledmap->getPositionY() != 0)
		{
			tiledmap->setPositionY(tiledmap->getPositionY() + m_player->speed);
			y_move -= m_player->speed;
		}
		return true;
	}
	return false;
}
bool GamePlaying::isCanReach(float x, float y)
{
	bool result;
	//log("ancherpoint %f %f", m_player->getAnchorPoint().x, m_player->getAnchorPoint().y);
	double mapX = ((x - DIFF) / 32);        //减去16是由于人物的锚点在中心
	double mapY = (MAP1_HEIGHT - (y - DIFF) / 32);   //49为Tiled里地图的坐标最大值
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
void GamePlaying::tofindEat(const float x, const float y)
{
	if (magent)
	{
		for (int i = -32; i <= 32; i += 32)
		{
			for (int j = -32; j <= 32; j += 32)
			{
				HPjudge(Vec2((x + i) / tileSize.width,
					(mapSize.height*tileSize.height - y + j) / tileSize.height));
				EXPjudge(Vec2((x + i) / tileSize.width,
					(mapSize.height*tileSize.height - y + j) / tileSize.height));
			}
		}
	}
	else
	{
		HPjudge(Vec2(x / tileSize.width,
			(mapSize.height*tileSize.height - y) / tileSize.height));
		EXPjudge(Vec2(x / tileSize.width,
			(mapSize.height*tileSize.height - y) / tileSize.height));
	}
}



void GamePlaying::onEnter()
{
	Scene::onEnter();
	//////////////////////////////////////
	auto keylistener = EventListenerKeyboard::create();
	auto touchlistener = EventListenerTouchOneByOne::create();
	auto mouselistener = EventListenerMouse::create();
	if (waytorun)
	{
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
	else
	{
		//键盘监听器，用于人物移动
		keylistener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event *event)
		{
			if (keyCode == (EventKeyboard::KeyCode)59)
				touchon = true;
		};

		EventDispatcher *eventDispatcher1 = Director::getInstance()->getEventDispatcher();
		eventDispatcher1->addEventListenerWithSceneGraphPriority(keylistener, this);

		mouselistener->onMouseMove = [&](Event *event)
		{
			EventMouse* e = (EventMouse*)event;
			pos.x = e->getCursorX();
			pos.y = e->getCursorY();
			pos.x += this->x_move;
			pos.y += this->y_move;
		};

		EventDispatcher *eventDispatcher2 = Director::getInstance()->getEventDispatcher();
		eventDispatcher2->addEventListenerWithSceneGraphPriority(mouselistener, this);

	}
	////////////////////////////////////////
}

void GamePlaying::update(float delta)
{
	float x = m_player->getPositionX(), y = m_player->getPositionY();
	m_player->x_coord = x; m_player->y_coord = y;
	//CCLOG("x=%f , y=%f", x, y);
	/////////////////////////////////////////
	//血条位置&长度设定
	m_pProgressView->setCurrentProgress(m_player->p_hp);
	m_pProgressView->setPosition(ccp(m_player->x_coord, m_player->y_coord + 50));
	n_pProgressView->setCurrentProgress(n_player->p_hp);
	n_pProgressView->setPosition(ccp(n_player->x_coord, n_player->y_coord + 50));
	//////////////////////////////////////////
	if (!waytorun)
	{
		keys[k_w] = keys[k_a] = keys[k_s] = keys[k_d] = false;
		if (pos.x - x > 0) { keys[k_d] = true; }
		else { keys[k_a] = true; }

		if (pos.y - y > 0) { keys[k_w] = true; }
		else { keys[k_s] = true; }
	}

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
	for (auto &bub : bubsum)
	{
		/*if (!isCanReach(bub->getPositionX(), bub->getPositionY()))
		{
			bub->hide();
		}*/
		for (auto pl : plsum)
		{
			if (bub->collidePlayer(pl))
			{
				Sprite* star = Sprite::create("attacked.png");
				star->setScale(1.5f, 1.5f);
				srand(time(NULL));
				auto randx = rand() % 20 - 10, randy = rand() % 20 - 10;
				star->setPosition(pl->x_coord + randx,
					pl->y_coord + randy);
				this->addChild(star);
				MoveBy* moveBy = MoveBy::create(0.2f, Point(0, 0));
				star->runAction(Sequence::create(moveBy, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, star)), NULL));
			}
		}
	}

}

void GamePlaying::runEvent()
{
	if (waytorun)
		m_player->runway1(keys, m_smallplayer);
	else m_player->runway2(pos, m_smallplayer);
}

void GamePlaying::attack()
{
	//   log("attack in x = %f  y = %f", pos.x, pos.y);
	auto Abullet = BulletBase::create();
	Abullet->bindSprite(Sprite::create("arrow.png"));
	Abullet->setPosition(Point(m_player->x_coord - x_move, m_player->y_coord - y_move));
	this->addChild(Abullet);

	float angle;
	float dx, dy;
	dx = pos.x - m_player->x_coord;
	dy = pos.y - m_player->y_coord;
	log("player %f  %f", m_player->x_coord, m_player->y_coord);
	log("pos  %f  %f", pos.x, pos.y);
	if (dy == 0)
	{
		if (dx >= 0)
			angle = 90;
		else angle = 270;
	}
	else
	{
		angle = atan(dx / dy) / 3.1416 * 180;
		if (dy < 0)
			angle += 180;
	}

	auto * rotateto = RotateTo::create(0, 45 + angle);
	Abullet->runAction(rotateto);

	Abullet->exist = true;
	bubsum.push_back(Abullet);

	Abullet->attacking(m_player, Abullet, pos);
}
void GamePlaying::menuStartScene(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	
	//返回按钮之前把一切回到最初状态
	smallmap_switch = true; 
	music_switch = true;
	mode_switch = true;

	auto sc = StartScene::createScene();        //缩放交替的切换动画
	auto reScene = TransitionShrinkGrow::create(1.0f, sc);
	Director::getInstance()->replaceScene(reScene);
}
