#include "Player.h"
#include "GamePlaying.h"
#define k_w (EventKeyboard::KeyCode)146
#define k_a (EventKeyboard::KeyCode)124
#define k_s (EventKeyboard::KeyCode)142
#define k_d (EventKeyboard::KeyCode)127
#define XIEBIAN 0.707

#define RETE (260.0/1600)  //smallplayer和player移动距离的比
bool Player::init()
{

	return true;
}

void Player::run(Player *player, std::map<EventKeyboard::KeyCode, bool>keys, Player *smallplayer)
{
	float xchange = 0, ychange = 0;

	if (keys[k_w] && !keys[k_a] && !keys[k_s] && !keys[k_d])
		ychange = player->speed;
	else if (!keys[k_w] && keys[k_a] && !keys[k_s] && !keys[k_d])
		xchange = -player->speed;
	else if (!keys[k_w] && !keys[k_a] && keys[k_s] && !keys[k_d])
		ychange = -player->speed;
	else if (!keys[k_w] && !keys[k_a] && !keys[k_s] && keys[k_d])
		xchange = player->speed;
	else if (keys[k_w] && keys[k_a] && !keys[k_s] && !keys[k_d]) 
	{
		xchange = -XIEBIAN *player->speed; ychange = XIEBIAN *player->speed;
	}
	else if (keys[k_w] && !keys[k_a] && !keys[k_s] && keys[k_d]) 
	{
		xchange = XIEBIAN *player->speed; ychange = XIEBIAN *player->speed;
	}
	else if (!keys[k_w] && keys[k_a] && keys[k_s] && !keys[k_d]) 
	{
		xchange = -XIEBIAN *player->speed; ychange = -XIEBIAN *player->speed;
	}
	else if (!keys[k_w] && !keys[k_a] && keys[k_s] && keys[k_d]) 
	{
		xchange = XIEBIAN *player->speed; ychange = -XIEBIAN *player->speed;
	}
	player->x_coord += xchange;
	player->y_coord += ychange;

	auto moveBy = MoveBy::create(0.1f, Point(xchange, ychange));
	//	log("x = %f  y = %f", player->x_coord, player->y_coord);
	player->runAction(moveBy);

	auto moveBy_smallplayer = MoveBy::create(0.1f, Point(xchange*RETE, ychange*(RETE)));
	smallplayer->runAction(moveBy_smallplayer);
	
}

void Player::hurt(int atk)
{
	p_hp -= atk;
	log("eeeeeeeeeeeee");

	if (p_hp <= 0)
		die();
}

void Player::die()
{

}