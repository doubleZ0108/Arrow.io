#ifndef _START_H_
#define _START_H_

#include "cocos2d.h"

class StartScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuHellowWorldScene(cocos2d::Ref* pSender);

	void MapPrinter();
	void ScenePrinter();

	// implement the "static create()" method manually
	CREATE_FUNC(StartScene);
};

#endif //_START_H_