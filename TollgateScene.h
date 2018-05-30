#ifndef __TollgateScene_H__
#define __TollgateScene_H__

#include "cocos2d.h"
#include <windows.h>  
#define KEY_DOWN(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 1 : 0)  
#define KEY_UP(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 0 : 1)  

using namespace cocos2d;

class Player;
class TollgateScene : public Layer {
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(TollgateScene);
	virtual void update(float delta);
	void onEnter();
	void runEvent();
	void attack();
private:
	Player * m_player;		//主角1
	Player* n_player;       //主角2，作为不动靶，闲的话也可以加一套操作系统当多人线下对战
	std::map<EventKeyboard::KeyCode, bool>keys;//记录按键状态
	bool touchon = false;//是否单击
	Point pos;//单击坐标，用于攻击
	bool once = false;//难以说明……
};

#endif