#ifndef __Player_H__
#define __Player_H__

#include "cocos2d.h"
#include "Entity.h"

using namespace cocos2d;

class Player : public Entity {
public:
	CREATE_FUNC(Player);
	virtual bool init();
	//void run1(Player *player, std::map<EventKeyboard::KeyCode, bool>keys, Player *smallplayer);
	Size visibleSize = CCDirector::getInstance()->getVisibleSize();

	int number;				 //人物ID
	Sprite *sprite = 0;		 //绑定的精灵
	bool life = true;		 //死亡判定
	int lives = 3;			 //剩余复活次数
	int level = 1;			 //等级
	int exp = 0;			 //当前经验值
	bool unbeat = false;	 //无敌装填
	bool attackcd = false;   //是否在攻击CD中
	bool animating = false;	 //是否在播放动画
	float x_coord = 80.0f;
	float y_coord = 80.0f;

	void runway1(std::map<EventKeyboard::KeyCode, bool>keys, Player *smallplayer);
	void hpraise(int num);		//踩到道具时血量的增加
	bool expraise(int num);		//经验值的增加
	int explimit();				//当前等级最大经验

	bool hurt(float atk);
	void animationcreate(int direct);	//生成动画的动作
	void runanimate(std::map<EventKeyboard::KeyCode, bool>keys); //动画的播放
	void attackCD();					//攻击的冷却时间
	void die(int rex, int rey);			//人物死亡
	void restart(int rex, int rey);		//复活
	

	//技能区
	float speed = 5;      //速度
	float radius = 30;		//判定大小
	float p_hp = 50.0;//当前血量
	int hpLimit = 50;//血量上限
	float atkpower = 1.0;//攻击力
	float atkrange = 1.0;//攻击距离
	float defpower = 1.0;//防御力
	bool magnet = false;  //磁铁技能

	//子弹数量
	int front = 1;		//向前发射子弹数量
	int leftside = 0;	//向左发射子弹数量
	int rightside = 0;	//向右发射子弹数量
	int back = 0;		//向右发射子弹数量

	//武器种类1箭2地刺3刀4飞镖
	int weapon = 1;
	//回血、经验增加
	float hpincrease = 1;
	float expincrease = 10;
	//CD时间
	float atkCD = 0.8;
	//穿墙技能
	bool ifcan_breakwall = false;	//人物能否穿墙
	bool ifbreakwall = false;		//子弹能否穿墙

	
	
private:
	Point restartp;
};

#endif