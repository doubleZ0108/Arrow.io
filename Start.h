#ifndef _START_H_
#define _START_H_

#include "cocos2d.h"

class StartScene : public cocos2d::Scene
{
public:
	cocos2d::Size size;
	cocos2d::Sprite* sprite;
	cocos2d::TMXTiledMap* map;
	cocos2d::TMXLayer* meta;

	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuHellowWorldScene(cocos2d::Ref* pSender);

	void MapPrinter();
	void ScenePrinter();

	void up(cocos2d::Object* pSender);
	void right(cocos2d::Object* pSender);
	void left(cocos2d::Object* pSender);
	void down(cocos2d::Object* pSender);
	bool isCanReach(float x, float y);

	// implement the "static create()" method manually
	CREATE_FUNC(StartScene);
};

#endif //_START_H_