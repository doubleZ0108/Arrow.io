#include "MapChose.h"
#include "Start.h"

USING_NS_CC;

extern bool language_flag;   //true->English   false->Chinese
extern char *FontToUTF8(const char* font);
//it is define in another .cpp file 
//and it is used to change character
int which_map = 1;

Scene* MapChose::createScene()
{
	return MapChose::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in MapChose.cpp\n");
}

// on "init" you need to initialize your instance
bool MapChose::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	ScenePrinter();

	return true;
}

void MapChose::ScenePrinter()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
	float x = rect.origin.x + rect.size.width / 2;
	float y = rect.origin.y + rect.size.height / 2;

	///////////////////////////////////////////
	//add Setting_Background

	x = rect.origin.x + rect.size.width / 2;
	y = rect.origin.y + rect.size.height / 2;
	auto *background = Sprite::create("Help_Background.png");
	background->setPosition(Vec2(x, y));
	this->addChild(background);

	///////////////////////////////////
	//a return button which click to back to HelloWorldScene
	auto *return_button = MenuItemImage::create(
		"backtoupper.png",
		"backtoupper_select.png",
		CC_CALLBACK_1(MapChose::menuStartScene, this));

	auto *preturn = Menu::create(return_button, NULL);
	x = rect.origin.x + rect.size.width*(10.0f / 11.0f);
	y = rect.origin.y + rect.size.height*(1.0f / 10.0f);
	preturn->setPosition(Vec2(x, y));
	preturn->setScale(1.0f);
	this->addChild(preturn);
	//////////////////////////////////////
	//add two tiledmap sceneshot
	auto pre_map1 = Sprite::create("pre_map.png");
	x = rect.origin.x + rect.size.width*(1.0f / 4.0f);
	y = rect.origin.y + rect.size.height*(1.0f / 2.0f);
	pre_map1->setPosition(Vec2(x, y));

	auto gridNodeTarget_1 = NodeGrid::create();
	this->addChild(gridNodeTarget_1);
	gridNodeTarget_1->addChild(pre_map1);

	auto pre_map2 = Sprite::create("pre_map.png");
	x = rect.origin.x + rect.size.width*(3.0f / 4.0f);
	pre_map2->setPosition(Vec2(x, y));

	auto gridNodeTarget_2 = NodeGrid::create();
	this->addChild(gridNodeTarget_2);
	gridNodeTarget_2->addChild(pre_map2);


	/////////////////////////////////////////////
	// Make pre_map1 touchable  
	auto listener1 = EventListenerTouchOneByOne::create();//创建一个触摸监听  
	listener1->setSwallowTouches(true); //设置是否想下传递触摸  

	//通过 lambda 表达式 直接实现触摸事件的回掉方法  
	listener1->onTouchBegan = [=](Touch* touch, Event* event) 
	{
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
	
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);

		if (rect.containsPoint(locationInNode))
		{
			CCActionInterval* fadeOutTRTiles = CCFadeOutTRTiles::create(5, CCSize(50, 50));
			gridNodeTarget_2->runAction(fadeOutTRTiles); 

			log("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
			target->setOpacity(180);
			return true;
		}
		return false;
	};
	listener1->onTouchEnded = [=](Touch* touch, Event* event) 
	{
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		target->setOpacity(255);
		which_map = 1;
		log("which_map %d", which_map);
		_eventDispatcher->removeEventListener(listener1);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, pre_map1);

	/////////////////////////////////////////////
	// Make pre_map2 touchable  
	auto listener2 = EventListenerTouchOneByOne::create();//创建一个触摸监听  
	listener2->setSwallowTouches(true); //设置是否想下传递触摸  
	
	listener2->onTouchBegan = [=](Touch* touch, Event* event)
	{
		auto target = static_cast<Sprite*>(event->getCurrentTarget());

		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);

		if (rect.containsPoint(locationInNode))
		{
			CCActionInterval* fadeOutUpTiles = CCFadeOutUpTiles::create(5, CCSize(10, 10));
			gridNodeTarget_1->runAction(fadeOutUpTiles);
			log("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
			target->setOpacity(180);
			return true;
		}
		return false;
	};
	listener2->onTouchEnded = [=](Touch* touch, Event* event)
	{
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		target->setOpacity(255);
		which_map = 2;
		log("which_map %d", which_map);
		_eventDispatcher->removeEventListener(listener2);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, pre_map2);
	
}
void MapChose::menuStartScene(Ref* pSender)
{
	auto sc = StartScene::createScene();        //按列分割界面的切换动画
	auto reScene = TransitionSlideInR::create(1.0f, sc);  //从右边推入的动画
	Director::getInstance()->replaceScene(reScene);
}
