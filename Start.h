#ifndef _START_H_
#define _START_H_

#include "cocos2d.h"
USING_NS_CC;
class StartScene : public Scene
{
public:
	Size size;
	Sprite* sprite;


	TMXTiledMap* tiledmap;
	TMXLayer* meta;
	TMXObjectGroup* HP_objects;

	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuHellowWorldScene(Ref* pSender);

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