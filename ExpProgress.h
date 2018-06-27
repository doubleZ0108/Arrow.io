#ifndef __EXPPROGRESS_H__
#define __EXPPROGRESS_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Player.h"
#include "ProgressView.h"
USING_NS_CC;

class ExpProgress :public cocos2d::CCLayer
{
private:
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Rect rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
public:
	ExpProgress();
	~ExpProgress();
	virtual bool init();
	CREATE_FUNC(ExpProgress);
	void ExpChange(int exp, int limit);
	void LvChange(int level);
	void ButtonAppear();
	void ButtonRemove();
	void Choices(int i);
	void youwin();
	Player *player;
	void youlose();
	bool ifchose = 0;
	void Breakwall_change(Player* player);
	void loselink();
private:
	ProgressView * expProgressView;
	Label *lv;
	ui::Button *choice1;
	ui::Button *choice2;
	ui::Button *choice3;
	Sprite *choiceback1;
	Sprite *choiceback2;
	Sprite *choiceback3;
	int c1, c2, c3;
	int skillsave = 0;
	std::vector<bool>unchoose;
};

#endif