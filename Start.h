#ifndef _START_H_
#define _START_H_

#include "cocos2d.h"

USING_NS_CC;

class StartScene : public Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuHellowWorldScene(Ref* pSender);
	void menuStartButton(Ref* pSender);

	void ScenePrinter();

	// implement the "static create()" method manually
	CREATE_FUNC(StartScene);

	
};

#endif //_START_H_