#ifndef __Player_H__
#define __Player_H__

#include "cocos2d.h"
#include "Entity.h"
using namespace cocos2d;

class Player : public Entity {
public:
	Player();
	~Player();
	CREATE_FUNC(Player);
	virtual bool init();
	///////////////////////////////////////
	void runway1(std::map<EventKeyboard::KeyCode, bool>keys);
	void runway2(Point point);

	Size visibleSize = CCDirector::getInstance()->getVisibleSize();
	float x_coord = visibleSize.width / 2;
	float y_coord = visibleSize.height / 2;//xy×ø±ê
	int speed = 3;
	float radius = 30;
	int p_hp = 50;

	void hurt(int atk);

	Sprite *sprite;
	void animationcreate(int direct);

	bool unbeat = 0;
private:
	bool animating = 0;
	void die();
	void restart();//¸´»î
};

#endif