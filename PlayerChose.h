#ifndef _PLAYERCHOSE_H_
#define _PLAYERCHOSE_H_

#include "cocos2d.h"

class PlayerChose: public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuStartScene(cocos2d::Ref* pSender);

	void ScenePrinter();

	// implement the "static create()" method manually
	CREATE_FUNC(PlayerChose);
};

#endif //_PLAYERCHOSE_H_