#ifndef __Player_H__
#define __Player_H__

#include "cocos2d.h"
#include "Entity.h"
using namespace cocos2d;

class Player : public Entity {
public:
	CREATE_FUNC(Player);
	virtual bool init();

	void run(Player *player, std::map<EventKeyboard::KeyCode, bool>keys, Player *smallplayer);

	Size visibleSize = CCDirector::getInstance()->getVisibleSize();
	float x_coord = 80.0f;
	float y_coord = 80.0f;
	int speed = 4;
	float radius = 30;
	int p_hp = 50;

	void hurt(int atk);
	void die();
};

#endif