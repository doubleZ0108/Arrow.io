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

	int atkpower = 5;
	int flyspeed = 500;
	float range = 200;
	Player *comefrom;//子弹来源

	void attacking(Player *player, BulletBase *Abullet, Point pos);
	bool collidePlayer(Player *player);//判断碰撞

	void pointChange(float dt);

	bool exist = false;//子弹存在

	void hide();
private:
	Point point;//子弹当前坐标
	Point target;//预定攻击坐标
	float xcs, ycs, rcs;//x y r单位时间改变量
	float flyrange;//已飞行距离
};

#endif