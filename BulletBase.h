#ifndef _BulletBase_H_
#define _BulletBase_H_

#include "Entity.h"
#include "Player.h"

class BulletBase :public Entity
{
public:
	BulletBase();
	~BulletBase();

	CREATE_FUNC(BulletBase);
	virtual bool init();

	BulletBase *create(Sprite *sprite);
	bool init(Sprite *sprite);

	int attpower;
	int flyspeed = 1000;
	float range = 200;

	void attacking(Player *player, BulletBase *Abullet, Point pos);
	bool collidePlayer(Player *player);

	Point point;
private:

};

#endif