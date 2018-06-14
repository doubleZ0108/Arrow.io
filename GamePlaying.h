#ifndef _GAMEPLAYING_H_
#define _GAMEPLAYING_H_

#include "cocos2d.h"
#include "Player.h"
#include "BulletBase.h"
#include "ProgressView.h"
#include <windows.h>  
#include <vector>
#define KEY_DOWN(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 1 : 0)  
#define KEY_UP(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 0 : 1)  
USING_NS_CC;
struct HP_MESS
{
	Sprite* hp_potion;   //道具药水图片的精灵
	int savex, savey;  //储存下道具的位置用于检索
	HP_MESS(Sprite* buf, int randx, int randy)
		:hp_potion(buf), savex(randx), savey(randy) {}
	//重载==运算符，否则的话调用std::find()函数时会出bug
	//std::find()的内部实现应该是借用了==运算符
	bool operator==(const HP_MESS &thv)
	{
		if(this->savex==thv.savex && this->savey==thv.savey)
		{
			return true;
		}
		else { return false; }
	}
};
struct EXP_MESS
{
	Sprite* exp_potion;   //道具药水图片的精灵
	int savex, savey;  //储存下道具的位置用于检索
	EXP_MESS(Sprite* buf, int randx, int randy)
		:exp_potion(buf), savex(randx), savey(randy) {}
	bool operator==(const EXP_MESS &thv)
	{
		if (this->savex == thv.savex && this->savey == thv.savey)
		{
			return true;
		}
		else { return false; }
	}
};
class GamePlaying : public Scene
{
private:
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Rect rect = Director::getInstance()->getOpenGLView()->getVisibleRect();

private:
	Size size;
	Sprite* sprite;

	Size mapSize;
	Size tileSize;
	float x_move = 0.0;   //用于地图调整之后鼠标点击的位置做相应的数值转换
	float y_move = 0.0;
	TMXTiledMap* tiledmap;
	TMXLayer* meta;
	TMXLayer* HP_objects;

	static std::vector<HP_MESS> hp_auto_arise;
	static std::vector<EXP_MESS> exp_auto_arise;
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuStartScene(Ref* pSender);

	void MapPrinter();
	void ScenePrinter();
	void MusicPrinter();
	void SmallmapPrinter();
	void ModePrinter();

	void Smallmap_Switch(Ref* pSender);
	void Mode_Switch(Ref* pSender);
	void Music_Switch(Ref* pSender);

	bool up(bool flag); //true代表我需要调用runEvent函数实实在在的移动
	bool right(bool flag);//false代表我只是想判断这个方向能不能走，其实不想移动
	bool left(bool flag);
	bool down(bool flag);
	bool isCanReach(float x, float y);

	void HPjudge(const Vec2 &pos);
	void HP_grow(float dt);
	void EXPjudge(const Vec2 &pos);
	void EXP_grow(float dt);
	// implement the "static create()" method manually
	CREATE_FUNC(GamePlaying);

	virtual void update(float delta);
	void runEvent();
	void onEnter();
	void attack();

private:
	Player * m_player = Player::create();		//主角1
	Player* n_player = Player::create();       //主角2，作为不动靶，闲的话也可
	std::map<EventKeyboard::KeyCode, bool>keys;//记录按键状态
	bool touchon = false;//是否单击
	Point pos;//单击坐标，用于攻击
	std::vector<Player*>plsum;
	std::vector<BulletBase*>bubsum;

	bool waytorun = false;  //true->键盘移动鼠标攻击

	Sprite *m_smallmap;
	//LayerColor *m_smallmap;
	Player *m_smallplayer;

	///////////////////////////////
	//血条
	ProgressView *m_pProgressView;
	ProgressView *n_pProgressView;
};

#endif //_GAMEPLAYING_H_