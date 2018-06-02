#include "Start.h"
#include "HelloWorldScene.h"
#include "TollgateScene.h"
#define MAP_SIZE 1320
USING_NS_CC;

extern bool language_flag;  //true->English   false->Chinese
extern char *FontToUTF8(const char* font);
//it is define in another .cpp file 
//and it is used to change character

Scene* StartScene::createScene()
{
	auto scene = StartScene::create();
	//auto layer = TollgateScene::createScene();
	//scene->addChild(layer);
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
	auto *map = TMXTiledMap::create("ArcherBattle_map1.tmx");
	this->addChild(map);
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
	size = Director::getInstance()->getVisibleSize();
	map = TMXTiledMap::create("map.tmx");
	this->addChild(map);

	meta = map->layerNamed("meta");
	meta->setVisible(false);
	sprite = Sprite::create("sprite.png");
	map->addChild(sprite, 10);
	
	sprite->setPosition(Vec2(16.5f, 16.5f));

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
	isCanReach(sprite->getPositionX(), sprite->getPositionY());

}
void StartScene::up(cocos2d::Object * pSender)
{

	float y = sprite->getPositionY();   //因为是往上走，所以获取精灵相对于地图的y坐标
	if (y + 16.5<MAP_SIZE&&isCanReach(sprite->getPositionX(), y + 33))
	{	//如果精灵上面那格不是地图的上边界
		//之所以是一格大小的一半,是因为精灵的锚点在中心,上面一个的下边界只需要再加16.5
		sprite->setPositionY(y + 33);  //把精灵置于上面一格的位置
		if ((y + map->getPositionY() > size.height / 2) && ((MAP_SIZE - y)>size.height / 2))
		{ //调整地图,使人物尽量居中
			map->setPositionY(map->getPositionY() - 33);
		}
	}

}
void StartScene::right(cocos2d::Object * pSender)
{
	float x = sprite->getPositionX();
	if (x + 16.5<MAP_SIZE&&isCanReach(x + 33, sprite->getPositionY()))
	{
		sprite->setPositionX(x + 33);
		if ((x + map->getPositionX() > size.width / 2) && ((MAP_SIZE - x)>size.width / 2))
		{
			map->setPositionX(map->getPositionX() - 33);
		}
	}
}
void StartScene::left(cocos2d::Object * pSender)
{
	float x = sprite->getPositionX();
	if (x>16.5&&isCanReach(x - 33, sprite->getPositionY()))
	{
		sprite->setPositionX(x - 33);
		if ((x + map->getPositionX() < size.width / 2) && map->getPositionX() != 0)
		{
			map->setPositionX(map->getPositionX() + 33);
		}
	}
}
void StartScene::down(cocos2d::Object * pSender)
{
	float y = sprite->getPositionY();
	if (y > 16.5&&isCanReach(sprite->getPositionX(), y - 33))
	{
		sprite->setPositionY(y - 33);
		if ((y + map->getPositionY() < size.height / 2) && map->getPositionY() != 0)
		{
			map->setPositionY(map->getPositionY() + 33);
		}
	}
}
bool StartScene::isCanReach(float x, float y)
{
	bool result;
	int mapX = (int)((x - 16.5) / 33);        //减去16.5是由于人物的锚点在中心
	int mapY = (int)(39 - (y - 16.5) / 33);   //39为Tiled里地图的坐标最大值
	int tileGid = meta->tileGIDAt(Vec2(mapX, mapY)); //33是一格的大小
	if (tileGid == 49)
	{
		result = true;
	}
	else
	{
		result = false;
	}
	return result;
}
void StartScene::menuHellowWorldScene(Ref* pSender)
{
	Director::getInstance()->replaceScene(HelloWorld::create());
}

