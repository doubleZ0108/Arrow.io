#include "Player.h"
#define k_w (EventKeyboard::KeyCode)119
#define k_a (EventKeyboard::KeyCode)97
#define k_s (EventKeyboard::KeyCode)115
#define k_d (EventKeyboard::KeyCode)100

Player::Player()
{
}

Player::~Player()
{
}

bool Player::init()
{

	return true;
}

void Player::run(Player *player, std::map<EventKeyboard::KeyCode, bool>keys)
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
	else if (keys[k_w] && keys[k_a] && !keys[k_s] && !keys[k_d]) {
		xchange = -0.7*player->speed; ychange = 0.7*player->speed;
	}
	else if (keys[k_w] && !keys[k_a] && !keys[k_s] && keys[k_d]) {
		xchange = 0.7*player->speed; ychange = 0.7*player->speed;
	}
	else if (!keys[k_w] && keys[k_a] && keys[k_s] && !keys[k_d]) {
		xchange = -0.7*player->speed; ychange = -0.7*player->speed;
	}
	else if (!keys[k_w] && !keys[k_a] && keys[k_s] && keys[k_d]) {
		xchange = 0.7*player->speed; ychange = -0.7*player->speed;
	}
	player->x_coord += xchange;
	player->y_coord += ychange;

	auto moveBy = MoveBy::create(0.1f, Point(xchange, ychange));
	log("x = %f  y = %f", player->x_coord, player->y_coord);
	player->runAction(moveBy);

}