#ifndef _START_H_
#define _START_H_

#include "cocos2d.h"
#include "Player.h"
#include "BulletBase.h"
#include <windows.h>  
#include <vector>
#define KEY_DOWN(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 1 : 0)  
#define KEY_UP(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 0 : 1)  
USING_NS_CC;
class StartScene : public Scene
{
public:
	Size size;
	Sprite* sprite;

	Size mapSize;
	Size tileSize;
	float x_move = 0.0;   //用于地图调整之后鼠标点击的位置做相应的数值转换
	float y_move = 0.0;
	TMXTiledMap* tiledmap;
	TMXLayer* meta;
	TMXObjectGroup* HP_objects;

	static std::vector<Sprite*> hp_potion;
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuHellowWorldScene(Ref* pSender);

	void MapPrinter();
	void ScenePrinter();

	bool up(bool flag); //true代表我需要调用runEvent函数实实在在的移动
	bool right(bool flag);//false代表我只是想判断这个方向能不能走，其实不想移动
	bool left(bool flag);
	bool down(bool flag);
	bool isCanReach(float x, float y);
	void HPjudge(const Vec2 &pos);

	// implement the "static create()" method manually
	CREATE_FUNC(StartScene);

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
	std::vector<Player*>plsum;
	std::vector<BulletBase*>bubsum;
};

#endif //_START_H_