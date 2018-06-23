#include "Player.h"
#include "GamePlaying.h"
#define k_w (EventKeyboard::KeyCode)146
#define k_a (EventKeyboard::KeyCode)124
#define k_s (EventKeyboard::KeyCode)142
#define k_d (EventKeyboard::KeyCode)127
#define XIEBIAN 0.707

#define RETE (260.0/1605)  //smallplayer移动的距离和m_player移动距离的比值

extern bool smallmap_switch;       //小地图控制开关,true->打开小地图,false->关上小地图
bool Player::init()
{
	return true;
}

void Player::runway1(std::map<EventKeyboard::KeyCode, bool>keys, Player *smallplayer)
{
	float xchange = 0, ychange = 0;

	if (keys[k_w] && !keys[k_a] && !keys[k_s] && !keys[k_d])
	{
		ychange = speed;
	}
	else if (!keys[k_w] && keys[k_a] && !keys[k_s] && !keys[k_d])
	{
		xchange = -speed;
	}
	else if (!keys[k_w] && !keys[k_a] && keys[k_s] && !keys[k_d])
	{
		ychange = -speed;
	}
	else if (!keys[k_w] && !keys[k_a] && !keys[k_s] && keys[k_d])
	{
		xchange = speed;
	}
	else if (keys[k_w] && keys[k_a] && !keys[k_s] && !keys[k_d])
	{
		xchange = -XIEBIAN * speed; ychange = XIEBIAN * speed;
	}
	else if (keys[k_w] && !keys[k_a] && !keys[k_s] && keys[k_d])
	{
		xchange = XIEBIAN * speed; ychange = XIEBIAN * speed;
	}
	else if (!keys[k_w] && keys[k_a] && keys[k_s] && !keys[k_d])
	{
		xchange = -XIEBIAN * speed; ychange = -XIEBIAN * speed;
	}
	else if (!keys[k_w] && !keys[k_a] && keys[k_s] && keys[k_d])
	{
		xchange = XIEBIAN * speed; ychange = -XIEBIAN * speed;
	}

	x_coord += xchange;
	y_coord += ychange;

	//	log("x = %f  y = %f", x_coord, y_coord);
	this->setPosition(ccp(x_coord, y_coord));

	if (smallmap_switch)
	{
		auto moveBy_smallplayer = MoveBy::create(0.1f, Point(xchange*RETE, ychange*(RETE)));
		smallplayer->setPosition(Vec2(
			smallplayer->getPositionX() + xchange * RETE,
			smallplayer->getPositionY() + ychange * (RETE)));
	}

}

bool Player::hurt(float atk)
{
	p_hp -= atk * defpower;
	if (p_hp <= 0)
	{
		die();
		return true;
	}
	return false;
}

void Player::die()
{
	lives--;

	this->sprite->setAnchorPoint(Point(0.5, 0));
	this->setPosition(x_coord, y_coord - 35);

	auto * rotateto = RotateTo::create(0.3, 90);
	CCActionInterval * delaytime = CCDelayTime::create(1);

	auto callbackFunc = [&]()
	{
		restart();
	};
	CallFunc* callFunc = CallFunc::create(callbackFunc);

	Action* actions = Sequence::create(rotateto, delaytime, callFunc, NULL);

	this->runAction(actions);
}

void Player::restart()
{
	if (lives == 0)
	{
		life = 0;
		return;
	}

	p_hp = 50;
	unbeat = 1;

	this->sprite->setAnchorPoint(Point(0.5, 0.5));
	x_coord = rand() % 1400 + 100;
	y_coord = rand() % 1400 + 100;

	this->setPosition(x_coord, y_coord);

	auto * rotateto = RotateTo::create(0, 0);
	Blink *blink = Blink::create(3, 6);

	auto callbackFunc = [&]()
	{
		unbeat = 0;
	};
	CallFunc* callFunc = CallFunc::create(callbackFunc);

	Action* actions = Sequence::create(rotateto, blink, callFunc, NULL);

	this->runAction(actions);
}

void Player::animationcreate(int direct)
{
	if (animating)
		return;
	animating = 1;
	/* 加载图片帧到缓存池 */

	/* 用一个列表保存所有SpriteFrame对象 */
	Animation* animation = Animation::create();

	for (int i = 1 + direct * 5; i <= 5 + direct * 5; i++)
	{
		/* 从SpriteFrame缓存池中获取SpriteFrame对象 */
		char nameSize[100] = { 0 };
		sprintf(nameSize, "Player/Figure/player%d_%d.png", number, i);
		animation->addSpriteFrameWithFile(nameSize);
	}

	animation->setDelayPerUnit(0.15f);

	/* 将动画包装成一个动作 */
	Animate* action = Animate::create(animation);

	auto callbackFunc = [&]()
	{
		animating = 0;
	};
	CallFunc* callFunc = CallFunc::create(callbackFunc);

	Action* actions = Sequence::create(action, callFunc, NULL);

	sprite->runAction(actions);
}

void Player::hpraise(int num)
{
	p_hp += hpincrease * num * 10;

	if (p_hp >= hpLimit)
		p_hp = hpLimit;
}

bool Player::expraise(int num)
{
	exp += expincrease * num * 2;
	bool whether = 0;
	while (exp >= explimit() && level <= 10)
	{
		exp -= explimit();
		level++;
		whether = 1;
	}
	return whether;
}

int Player::explimit()
{
	return (5 * level*level + 15 * level);
}

void Player::attackCD()
{
	attackcd = 1;

	MoveBy* moveBy = MoveBy::create(atkCD, ccp(0, 0));
	auto callbackFunc = [&]()
	{
		attackcd = 0;
	};
	CallFunc* callFunc = CallFunc::create(callbackFunc);

	Action* actions = Sequence::create(moveBy, callFunc, NULL);
	this->runAction(actions);
}

void Player::runanimate(std::map<EventKeyboard::KeyCode, bool>keys1)
{
	if (keys1[k_w] && !keys1[k_a] && !keys1[k_s] && !keys1[k_d])
	{
		animationcreate(3);
	}
	else if (!keys1[k_w] && keys1[k_a] && !keys1[k_s] && !keys1[k_d])
	{
		animationcreate(1);
	}
	else if (!keys1[k_w] && !keys1[k_a] && keys1[k_s] && !keys1[k_d])
	{
		animationcreate(0);
	}
	else if (!keys1[k_w] && !keys1[k_a] && !keys1[k_s] && keys1[k_d])
	{
		animationcreate(2);
	}
	else if (keys1[k_w] && keys1[k_a] && !keys1[k_s] && !keys1[k_d])
	{
		animationcreate(3);
	}
	else if (keys1[k_w] && !keys1[k_a] && !keys1[k_s] && keys1[k_d])
	{
		animationcreate(3);
	}
	else if (!keys1[k_w] && keys1[k_a] && keys1[k_s] && !keys1[k_d])
	{
		animationcreate(0);
	}
	else if (!keys1[k_w] && !keys1[k_a] && keys1[k_s] && keys1[k_d])
	{
		animationcreate(0);
	}
}