#include "Bulletbase.h"
#include "GamePlaying.h"
BulletBase::BulletBase()
{
}

BulletBase::~BulletBase()
{
}

bool BulletBase::init()
{
	schedule(schedule_selector(BulletBase::pointChange), 0.02);
	return true;
}

BulletBase* BulletBase::create(Sprite *sprite)
{
	BulletBase* abullet = new BulletBase();
	return abullet;
}

void BulletBase::attacking(Player *player, BulletBase *Abullet, Point pos)
{
	comefrom = player;
	float time = (Abullet->range) / (Abullet->flyspeed);//单位为秒
	float length = sqrt((player->x_coord - pos.x)*(player->x_coord - pos.x) + (player->y_coord - pos.y)*(player->y_coord - pos.y));
	float xchange = (pos.x - player->x_coord) / length * Abullet->range;
	float ychange = (pos.y - player->y_coord) / length * Abullet->range;
	//在cocos2dx中你甚至能学几何

	auto moveBy = MoveBy::create(time, Point(xchange, ychange));
	point.x = player->x_coord;
	point.y = player->y_coord;
	target.x = point.x + xchange;
	target.y = point.y + ychange;

	//	log("start x = %f  y = %f", point.x, point.y);
	//	log("target x = %f  y = %f", target.x, target.y);

	xcs = xchange / time * 0.02;
	ycs = ychange / time * 0.02;
	rcs = sqrt(xcs*xcs + ycs * ycs);
	flyrange = 0;

	Abullet->runAction(moveBy);
	//将子弹位移动作和子弹消失动作合并，后期可以考虑子弹停留效果（现在不写是因为碰撞判定问题……
	//话说这个函数边执行边把自己这个对象删除，结果居然不出bug，看来360强力卸载是能卸掉360的
}

void BulletBase::hide()
{
	auto del = CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, this));
	this->runAction(del);
	exist = false;
}

void BulletBase::pointChange(float dt)
{
	if (getSprite() != NULL)
	{
		if (flyrange + rcs <= range)
		{
			point.x += xcs;
			point.y += ycs;
			flyrange += rcs;
			//			log("x = %f  y = %f", point.x, point.y);
		}
		else
		{
			point = target;
			//			log("x = %f  y = %f", point.x, point.y);
			this->hide();
		}
	}
}

bool BulletBase::collidePlayer(Player *player)
{
	if (this->comefrom != player && player->p_hp > 0 && !player->unbeat)
	{
		float dx, dy;
		dx = point.x - player->x_coord;
		dy = point.y - player->y_coord;
		if (dx < 0) dx = -dx;
		if (dy < 0) dy = -dy;
		if (dx < player->radius && dy < player->radius)
		{
			this->hide();
			player->hurt(atkpower);
			return true;
		}
	}
	return false;
}
