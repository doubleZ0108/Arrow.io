#include "Bulletbase.h"

BulletBase::BulletBase()
{
	attpower = 5;
	flyspeed = 5;

}

BulletBase::~BulletBase()
{
}

bool BulletBase::init()
{
	return true;
}

BulletBase* BulletBase::create(Sprite *sprite)
{
	BulletBase* abullet = new BulletBase();
	return abullet;
}

void BulletBase::attacking(Player *player, BulletBase *Abullet, Point pos)
{
	float time = (Abullet->range) / (Abullet->flyspeed)*0.01;
	float length = sqrt((player->x_coord - pos.x)*(player->x_coord - pos.x) + (player->y_coord - pos.y)*(player->y_coord - pos.y));
	float xchange = (pos.x - player->x_coord) / length * Abullet->range;
	float ychange = (pos.y - player->y_coord) / length * Abullet->range;
	//在cocos2dx中你甚至能学几何

	auto moveBy = MoveBy::create(time, Point(xchange, ychange));



	Abullet->runAction(Sequence::create(moveBy, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, Abullet)), NULL));
	//将子弹位移动作和子弹消失动作合并，后期可以考虑子弹停留效果（现在不写是因为碰撞判定问题……
	//话说这个函数边执行边把自己这个对象删除，结果居然不出bug，看来360强力卸载是能卸掉360的
}

bool BulletBase::collidePlayer(Player *player)
{
	return true;
}