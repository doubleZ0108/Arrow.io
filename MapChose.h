#ifndef _MAPCHOSE_H_
#define _MAPCHOSE_H_

#include "cocos2d.h"

class MapChose : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	// a selector callback
	void menuStartScene(cocos2d::Ref* pSender);

	void ScenePrinter();


	// implement the "static create()" method manually
	CREATE_FUNC(MapChose);
};

#endif //_MAPCHOSE_H_