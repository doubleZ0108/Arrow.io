#ifndef __Player_H__
#define __Player_H__

#include "cocos2d.h"
#include "Entity.h"
using namespace cocos2d;

#define JUMP_ACTION_TAG 1

class Player : public Entity {
public:
	Player();
	~Player();
	CREATE_FUNC(Player);
	virtual bool init();

	void run(Player *player, std::map<EventKeyboard::KeyCode, bool>keys);
public:
	Size visibleSize = Director::getInstance()->getVisibleSize();
	float x_coord = visibleSize.width / 2;
	float y_coord = visibleSize.height / 2;//xy×ø±ê
	int speed = 3;
private:
	int p_hp;
};

#endif