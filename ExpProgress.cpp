#include "ExpProgress.h"

#define choicesnum 19

//#define charactersize 20
//#pragma execution_character_set("utf-8");

using namespace cocos2d::ui;

ExpProgress::ExpProgress()
{
}

ExpProgress::~ExpProgress()
{
}

bool ExpProgress::init()
{
	expProgressView = new ProgressView;
	expProgressView->setPosition(ccp(640, 690));
	expProgressView->setScale(2, 1.2);
	expProgressView->setBackgroundTexture("energyback.png");
	expProgressView->setForegroundTexture("energyfore.png");
	expProgressView->setTotalProgress(1);
	expProgressView->setCurrentProgress(0);
	addChild(expProgressView);

	lv = Label::create("LV 1", "Arial", 40);
	lv->setPosition(ccp(640, 690));
	addChild(lv);

	for (int i = 0; i < choicesnum + 1; i++)
		unchoose.push_back(false);

	return true;
}

void ExpProgress::ExpChange(int exp, int limit)
{
	expProgressView->setCurrentProgress((float)exp / limit);
}

void ExpProgress::LvChange(int level)
{
	removeChild(lv);
	if (level == 11)
	{
		lv = Label::create("LV MAX", "Arial", 40);
	}
	else
	{
		lv = Label::create(StringUtils::format("LV %d", level), "Arial", 40);
	}
	lv->setPosition(ccp(640, 690));
	addChild(lv);

	skillsave++;
	if (skillsave <= 1)
		ButtonAppear();
}

void ExpProgress::ButtonAppear()
{
	srand(time(NULL));

	c1 = rand() % choicesnum + 1;
	while (unchoose[c1] == true)
		c1 = rand() % choicesnum + 1;
	choice1 = Button::create(StringUtils::format("Player/skill/choice%d.png", c1));
	choice1->setPosition(ccp(420, 120));
	choice1->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		Choices(c1);
		ButtonRemove();
	});
	addChild(choice1);

	c2 = rand() % choicesnum + 1;
	while (c2 == c1 || unchoose[c2] == true)
		c2 = rand() % choicesnum + 1;
	choice2 = Button::create(StringUtils::format("Player/skill/choice%d.png", c2));
	choice2->setPosition(ccp(640, 120));
	choice2->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		Choices(c2);
		ButtonRemove();
	});
	addChild(choice2);

	c3 = rand() % choicesnum + 1;
	while (c3 == c1 || c3 == c2 || unchoose[c3] == true)
		c3 = rand() % choicesnum + 1;
	choice3 = Button::create(StringUtils::format("Player/skill/choice%d.png", c3));
	choice3->setPosition(ccp(860, 120));
	choice3->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		Choices(c3);
		ButtonRemove();
	});
	addChild(choice3);
}

void ExpProgress::ButtonRemove()
{
	removeChild(choice1);
	removeChild(choice2);
	removeChild(choice3);
	skillsave--;

	if (skillsave > 0)
		ButtonAppear();
}

void ExpProgress::Choices(int i)
{
	//Dictionary* strings = Dictionary::createWithContentsOfFile("string.xml");
	//const char* str;
	//str = ((String*)strings->objectForKey(StringUtils::format("choice%d",i)))->getCString();
	//Label *label = Label::createWithSystemFont(str, "", charactersize);

	switch (i)
	{
	case 1://地刺
		player->weapon = 2;
		break;
	case 2://武士刀
		player->weapon = 3;
		break;
	case 3://飞镖
		player->weapon = 4;
		break;
	case 4://加攻击力
		player->atkpower += 0.3;
		break;
	case 5://加射程
		player->atkrange += 0.2;
		break;
	case 6://加防御
		player->defpower -= 0.2;
		break;
	case 7://加前方
		player->front++;
		break;
	case 8://加后方
		player->back++;
		break;
	case 9://加左右
		player->leftside++;
		player->rightside++;
		break;
	case 10://红心增加更多hp
		player->hpincrease += 0.5;
		break;
	case 11://增加更多经验
		player->expincrease += 0.5;
		break;
	case 12://加速度
		player->speed += 1;
		break;
	case 13://血上限
		player->hpLimit += 10;
		break;
	case 14://减血上限加攻击力
		player->hpLimit -= 10;
		player->atkpower += 0.7;
		break;
	case 15://减攻击CD
		player->atkCD -= 0.15;
		break;
	case 16://狙击武器
		player->atkrange += 0.5;
		player->speed -= 1;
		break;
	case 17://全属性提升
		player->atkpower += 0.1;
		player->atkrange += 0.1;
		player->defpower -= 0.1;
		break;
	case 18://子弹穿墙
		player->ifcan_breakwall = true;
		break;
	case 19:
		player->magnet = true;
		break;
	}

	for (int i = 1; i <= 3; i++)
		unchoose[i] = false;
	unchoose[player->weapon - 1] = true;

	if (player->speed <= 2)
		unchoose[16] = true;
	else unchoose[16] = false;

	if (player->atkCD <= 0.4)
		unchoose[15] = true;

	if (player->weapon == 2 || player->ifcan_breakwall == true)
		unchoose[18] = true;
	else unchoose[18] = false;

	if (player->magnet)
		unchoose[19] = true;
	else unchoose[19] = false;

	//label->setPosition(ccp(640, 640));
	//addChild(label);
}