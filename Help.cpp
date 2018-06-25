#include "Help.h"
#include "HelloWorldScene.h"

USING_NS_CC;

extern bool language_flag;   //true->English   false->Chinese
extern char *FontToUTF8(const char* font);
//it is define in another .cpp file 
//and it is used to change character

Scene* HelpScene::createScene()
{
	return HelpScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelpScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelpScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	ScenePrinter();
	PagePrinter();

	return true;
}

void HelpScene::ScenePrinter()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
	float x = rect.origin.x + rect.size.width / 2;
	float y = rect.origin.y + rect.size.height / 2;

	//a return button which click to back to HelloWorldScene
	auto *return_button = MenuItemImage::create(
		"Scene/Buttons/backtoupper.png",
		"Scene/Buttons/backtoupper_select.png",
		CC_CALLBACK_1(HelpScene::menuHellowWorldScene, this));

	auto *preturn = Menu::create(return_button, NULL);
	x = rect.origin.x + rect.size.width*(10.0f / 11.0f);
	y = rect.origin.y + rect.size.height*(1.0f / 10.0f);
	preturn->setPosition(Vec2(x, y));
	preturn->setScale(1.0f);
	this->addChild(preturn, 10);


}

void HelpScene::PagePrinter()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true); //设置是否想下传递触摸  
	listener->onTouchBegan = [&](Touch * touch, Event * unused_event)
	{
		return true;
	};
	listener->onTouchEnded = [&](Touch * touch, Event * unused_event)
	{

	};
	Director::getInstance()->getEventDispatcher()->
		addEventListenerWithSceneGraphPriority(listener, this);

	//滑动关卡  
	Size WinSize = Director::getInstance()->getWinSize();

	pageView = PageView::create();
	pageView->setSize(Size(WinSize.width, WinSize.height));
	pageView->setPosition(Vec2::ZERO);
	//8个滑动选项卡  
	auto rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
	float x = rect.origin.x + rect.size.width / 2;
	float y = rect.origin.y + rect.size.height / 2;

	for (int i = 0; i < 13; i++)
	{
		auto layout = Layout::create();
		layout->setSize(Size(WinSize.width, WinSize.height));

		switch (i)
		{
		case 0:Page0(layout); break;
		case 1:Page1(layout); break;
		case 2:Page2(layout); break;
		case 3:Page3(layout); break;
		case 4:Page4(layout); break;
		case 5:Page5(layout); break;
		case 6:Page6(layout); break;
		case 7:Page7(layout); break;
		case 8:Page8(layout); break;
		case 9:Page9(layout); break;
		case 10:Page10(layout); break;
		case 11:Page11(layout); break;

		default:break;
		}

		pageView->addPage(layout);

	}

	auto JumpSceneCallback_lambda = [&](Ref * pSender, PageView::EventType type)
	{};
	pageView->addEventListener(JumpSceneCallback_lambda);
	pageView->setName("pageView");
	this->addChild(pageView);
}

void HelpScene::Page0(Layout * v_layout)
{
	//爆炸效果*/
	CCParticleSystem* particleSystem = CCParticleExplosion::create();
	particleSystem->setTexture(CCTextureCache::sharedTextureCache()->addImage("Scene/Background/fire.jpg"));
	addChild(particleSystem);

	Label *title;
	if (language_flag)
	{
		title = Label::create("Basic Introduction", "Arial", 40);
	}
	else
	{
		title = Label::create(FontToUTF8("游戏简介"), "Arial", 40);
	}
	x = rect.origin.x + rect.size.width / 2;
	y = rect.origin.y + rect.size.height *(18.5 / 20.0f);
	title->setPosition(Vec2(x, y));
	v_layout->addChild(title);

	auto *crown = Sprite::create("Scene/Buttons/crown.png");
	y = rect.origin.y + rect.size.height*(14.0f / 20.0f);
	crown->setPosition(Vec2(x, y));
	v_layout->addChild(crown);

	Label *word1, *word2;
	if (language_flag)
	{
		word1 = Label::create("         As the level increases\n       you will gain more skills\n\
			       Make good use of them\nand give your opponent a knock\n Enjoy yourself in Archer-Battle!", "Arial", 40);
		word2 = Label::create("<- Slide left to see more", "Arial", 40);
	}
	else
	{
		word1 = Label::create(FontToUTF8("         随着等级的升高\n     你会获得更多的技能\n\
			           好好利用它们\n   去给你的对手一击痛击\n欢迎来到——弓箭手大作战!"), "Arial", 40);
		word2 = Label::create(FontToUTF8("<- 左滑查看更多"), "Arial", 40);
	}
	y = rect.origin.y + rect.size.height / 2.0f;
	//word1->setDimensions(500.0f, 281.25f);
	word1->setPosition(Vec2(x, y));
	v_layout->addChild(word1);

	x = rect.origin.x + rect.size.width*(5.0f / 6.0f);
	y = rect.origin.y + rect.size.height*(1.0f / 5.0f);
	word2->setPosition(Vec2(x, y));
	v_layout->addChild(word2);
}

void HelpScene::Page1(Layout *v_layout)
{
	auto *page1 = Sprite::create("Scene/Background/page1.png");
	x = rect.origin.x + rect.size.width * 0;
	y = rect.origin.y + rect.size.height;
	page1->setAnchorPoint(Vec2(0.0f, 1.0f));
	page1->setPosition(Vec2(x, y));
	v_layout->addChild(page1);

	Label *wifi, *setting, *lv, *skill, *potion, *word2;
	if (language_flag)
	{
		wifi = Label::create("Click the WIFI button to connect the network. Enjoy the game with\nyour friends. Please continue reading for attentions", "Arial", 20);
		setting = Label::create("Global map switch\nBackground music switch\nGame mode switch. Please continue reading for details", "Arial", 20);
		lv = Label::create("Current rank & experience level", "Arial", 20);
		skill = Label::create("Choose a prop upgraded\nPlease continue reading", "Arial", 20);
		potion = Label::create("EXP props & HP props", "Arial", 20);

		word2 = Label::create("<- Slide left to see more", "Arial", 40);
	}
	else
	{
		wifi = Label::create(FontToUTF8("点击联网，与你的朋友尽享欢乐时光\n注意事项请继续阅读"), "Arial", 20);
		setting = Label::create(FontToUTF8("全局地图开关\n背景音乐开关\n游戏模式切换开关,详细信息请继续阅读"), "Arial", 20);
		lv = Label::create(FontToUTF8("当前等级和经验条"), "Arial", 20);
		skill = Label::create(FontToUTF8("每一次升级可以选择一个道具\n详细功能请继续阅读"), "Arial", 20);
		potion = Label::create(FontToUTF8("能量道具和回血道具"), "Arial", 20);

		word2 = Label::create(FontToUTF8("<- 左滑查看更多"), "Arial", 40);
	}


	x = rect.origin.x + rect.size.width*(1.9f / 3.0f);
	y = rect.origin.y + rect.size.height*(19.0f / 20.0f);
	wifi->setAnchorPoint(Vec2(0.0f, 0.5f));
	wifi->setPosition(Vec2(x, y));
	v_layout->addChild(wifi);

	y = rect.origin.y + rect.size.height*(17.0f / 20.0f);
	setting->setAnchorPoint(Vec2(0.0f, 0.5f));
	setting->setPosition(Vec2(x, y));
	v_layout->addChild(setting);

	y = rect.origin.y + rect.size.height*(15.0f / 20.0f);
	lv->setAnchorPoint(Vec2(0.0f, 0.5f));
	lv->setPosition(Vec2(x, y));
	v_layout->addChild(lv);

	y = rect.origin.y + rect.size.height*(9.5f / 20.0f);
	skill->setAnchorPoint(Vec2(0.0f, 0.5f));
	skill->setPosition(Vec2(x, y));
	v_layout->addChild(skill);

	x = rect.origin.x + rect.size.height*(1.0f / 10.0f);
	y = rect.origin.y + rect.size.height*(6.5f / 20.0f);
	potion->setAnchorPoint(Vec2(0.0f, 0.5f));
	potion->setPosition(Vec2(x, y));
	v_layout->addChild(potion);

	x = rect.origin.x + rect.size.width*(5.0f / 6.0f);
	y = rect.origin.y + rect.size.height*(1.0f / 5.0f);
	word2->setPosition(Vec2(x, y));
	v_layout->addChild(word2);

}

void HelpScene::Page2(Layout * v_layout)
{
	Label *title;
	if (language_flag)
	{
		title = Label::create("Skill List", "Arial", 40);
	}
	else
	{
		title = Label::create(FontToUTF8("技能一览表"), "Arial", 40);
	}
	x = rect.origin.x + rect.size.width / 2;
	y = rect.origin.y + rect.size.height *(18.5 / 20.0f);
	title->setPosition(Vec2(x, y));
	v_layout->addChild(title);

	int i;
	Sprite *skill[6];
	for (i = 0; i<6; ++i)
	{
		skill[i] = Sprite::create(StringUtils::format("Player/skill/choice%d.png", i + 4));
	}
	float xx[6], yy[6];
	xx[0] = xx[1] = xx[2] = rect.origin.x + rect.size.width*(1.0f / 8.0f);
	xx[3] = xx[4] = xx[5] = rect.origin.x + rect.size.width*(5.0f / 8.0f);
	yy[0] = yy[3] = rect.origin.y + rect.size.height*(15.0f / 20.0f);
	yy[1] = yy[4] = rect.origin.y + rect.size.height*(9.0f / 20.0f);
	yy[2] = yy[5] = rect.origin.y + rect.size.height*(3.0f / 20.0f);
	for (i = 0; i < 6; ++i)
	{
		skill[i]->setScale(0.9f);
		skill[i]->setPosition(Vec2(xx[i], yy[i]));
		v_layout->addChild(skill[i]);
	}

	Label *label[6];
	if (language_flag)
	{
		label[0] = Label::create("DMG(Damage) Up", "Arial", 40);
		label[1] = Label::create("Range Up", "Arial", 40);
		label[2] = Label::create("DEF(Defense) Up", "Arial", 40);
		label[3] = Label::create("Launch an arrow forward more", "Arial", 40);
		label[4] = Label::create("Launch an arrow backward more", "Arial", 40);
		label[5] = Label::create("Launch two arrows toward left and right more", "Arial", 40);
	}
	else
	{
		label[0] = Label::create(FontToUTF8("增加攻击力"), "Arial", 40);
		label[1] = Label::create(FontToUTF8("增加射程"), "Arial", 40);
		label[2] = Label::create(FontToUTF8("增加防御力"), "Arial", 40);
		label[3] = Label::create(FontToUTF8("向前多发射一支箭"), "Arial", 40);
		label[4] = Label::create(FontToUTF8("向后多发射一支箭"), "Arial", 40);
		label[5] = Label::create(FontToUTF8("向左右各多发射一支箭"), "Arial", 40);
	}
	xx[0] = xx[1] = xx[2] = rect.origin.x + rect.size.width*(5.0f / 16.0f);
	xx[3] = xx[4] = xx[5] = rect.origin.x + rect.size.width*(13.0f / 16.0f);
	for (i = 0; i < 6; ++i)
	{
		label[i]->setDimensions(250.0f, 150.0f);
		label[i]->setPosition(Vec2(xx[i], yy[i]));
		v_layout->addChild(label[i]);
	}
}

void HelpScene::Page3(Layout * v_layout)
{
	int i;
	Sprite *skill[6];
	for (i = 0; i<6; ++i)
	{
		skill[i] = Sprite::create(StringUtils::format("Player/skill/choice%d.png", i + 10));
	}
	float xx[6], yy[6];
	xx[0] = xx[1] = xx[2] = rect.origin.x + rect.size.width*(1.0f / 8.0f);
	xx[3] = xx[4] = xx[5] = rect.origin.x + rect.size.width*(5.0f / 8.0f);
	yy[0] = yy[3] = rect.origin.y + rect.size.height*(15.0f / 20.0f);
	yy[1] = yy[4] = rect.origin.y + rect.size.height*(9.0f / 20.0f);
	yy[2] = yy[5] = rect.origin.y + rect.size.height*(3.0f / 20.0f);
	for (i = 0; i < 6; ++i)
	{
		skill[i]->setScale(0.9f);
		skill[i]->setPosition(Vec2(xx[i], yy[i]));
		v_layout->addChild(skill[i]);
	}

	Label *label[6];
	if (language_flag)
	{
		label[0] = Label::create("Recover More Blood Each Time", "Arial", 40);
		label[1] = Label::create("1.5 Times of Experience", "Arial", 40);
		label[2] = Label::create("Speed Up", "Arial", 40);
		label[3] = Label::create("HP Ceiling Up", "Arial", 40);
		label[4] = Label::create("Attack Means Defense", "Arial", 40);
		label[5] = Label::create("Reduce Attack CD", "Arial", 40);
	}
	else
	{
		label[0] = Label::create(FontToUTF8("每次回复更多的血量"), "Arial", 40);
		label[1] = Label::create(FontToUTF8("1.5倍经验"), "Arial", 40);
		label[2] = Label::create(FontToUTF8("增加移动速度"), "Arial", 40);
		label[3] = Label::create(FontToUTF8("增加血量上限"), "Arial", 40);
		label[4] = Label::create(FontToUTF8("以攻为守"), "Arial", 40);
		label[5] = Label::create(FontToUTF8("减少攻击CD"), "Arial", 40);
	}
	xx[0] = xx[1] = xx[2] = rect.origin.x + rect.size.width*(5.0f / 16.0f);
	xx[3] = xx[4] = xx[5] = rect.origin.x + rect.size.width*(13.0f / 16.0f);
	for (i = 0; i < 6; ++i)
	{
		label[i]->setDimensions(250.0f, 150.0f);
		label[i]->setPosition(Vec2(xx[i], yy[i]));
		v_layout->addChild(label[i]);
	}
}

void HelpScene::Page4(Layout * v_layout)
{
	int i;
	Sprite *skill[7];
	for (i = 0; i<6; ++i)
	{
		skill[i] = Sprite::create(StringUtils::format("Player/skill/choice%d.png", i + 16));
	}
	float xx[6], yy[6];
	xx[0] = xx[1] = xx[2] = rect.origin.x + rect.size.width*(1.0f / 8.0f);
	xx[3] = xx[4] = xx[5] = rect.origin.x + rect.size.width*(5.0f / 8.0f);
	yy[0] = yy[3] = rect.origin.y + rect.size.height*(15.0f / 20.0f);
	yy[1] = yy[4] = rect.origin.y + rect.size.height*(9.0f / 20.0f);
	yy[2] = yy[5] = rect.origin.y + rect.size.height*(3.0f / 20.0f);
	for (i = 0; i < 6; ++i)
	{
		skill[i]->setScale(0.9f);
		skill[i]->setPosition(Vec2(xx[i], yy[i]));
		v_layout->addChild(skill[i]);
	}

	Label *label[6];
	if (language_flag)
	{
		label[0] = Label::create("Defense Means Attack", "Arial", 40);
		label[1] = Label::create("All Stats Up (a little)", "Arial", 40);
		label[2] = Label::create("Bullets Passing Through Wall", "Arial", 40);
		label[3] = Label::create("Magnet", "Arial", 40);
		label[4] = Label::create("Hero Walking Through Wall", "Arial", 40);
		label[5] = Label::create("Stand Higher To Look Farther", "Arial", 40);
	}
	else
	{
		label[0] = Label::create(FontToUTF8("以守为攻"), "Arial", 40);
		label[1] = Label::create(FontToUTF8("全属性少量提升"), "Arial", 40);
		label[2] = Label::create(FontToUTF8("子弹可以穿墙"), "Arial", 40);
		label[3] = Label::create(FontToUTF8("磁铁"), "Arial", 40);
		label[4] = Label::create(FontToUTF8("人物可以穿墙"), "Arial", 40);
		label[5] = Label::create(FontToUTF8("站得更高看的更远"), "Arial", 40);
	}
	xx[0] = xx[1] = xx[2] = rect.origin.x + rect.size.width*(5.0f / 16.0f);
	xx[3] = xx[4] = xx[5] = rect.origin.x + rect.size.width*(13.0f / 16.0f);
	for (i = 0; i < 6; ++i)
	{
		label[i]->setDimensions(250.0f, 150.0f);
		label[i]->setPosition(Vec2(xx[i], yy[i]));
		v_layout->addChild(label[i]);
	}
}

void HelpScene::Page5(Layout *v_layout)
{
	Label *title;
	if (language_flag)
	{
		title = Label::create("Attention", "Arial", 40);
	}
	else
	{
		title = Label::create(FontToUTF8("注意事项"), "Arial", 40);
	}
	x = rect.origin.x + rect.size.width / 2;
	y = rect.origin.y + rect.size.height *(18.5 / 20.0f);
	title->setPosition(Vec2(x, y));
	v_layout->addChild(title);

	Label *word;
	if (language_flag)
	{
		word = Label::create("1.Please select your favorite map and character before entering the game. If you want to play with your friends, please make sure you are in the same map(Do you know the distance between Mars and the moon?\n\
\n2.You can play alone after entering the map, but if you want to play with friends, please read Tip 3 carefully\n\
\n3.How to play online: Do not rush to move after entering the map, make sure that you and your friend click the WIFI button in the upper right corner. You will see\n\
your friend on the map in the upper left corner, after this, you can enjoy playing online. You must follow the rules of the game, and bugbug will never occur\n\
\n4.Each person has 3 opportunities to come back to life. Cherish life and stay away from bugs！\n\
\n5.When you open a buff(After you get Hero Walking Through Wall, please do not go outside the map. The reason is the same as 1 (Do you know the distance between Mars and the moon?\n\
6.To ensure that you and your friend are in the same room and that others will not be affected when they want to create a room, please exit the game interface after game over.", "Arial", 30);
	}
	else
	{
		word = Label::create(FontToUTF8("1.进入游戏之前请选择你喜欢的地图和人物，如果你想和你的朋友一起玩耍，请确保你们在同一张地图中(你知道火星到月球之间的距离么?\n\
\n2.进到房间之后你可以进行单机玩耍，但是如果想和朋友一起玩耍请认真阅读提示3\n\
\n3.联机游戏设置：进入地图后不要着急移动，确保自己和朋友都点击了右上角的WIFI按钮后，你会在\n\
左上角的地图上看到你的朋友，这之后你们可以尽情的联机玩耍哦，请千万按照规则游戏，bugbug永不出现\n\
\n4.每人有3次复活机会，珍惜生命，远离bug\n\
\n5.如果你开了buff(启动了人物穿墙技能，请不要往地图外面走，原因同1(你知道火星到月球的距离么?\n\
\n6.为保证你和你的朋友在同一房间，并且不影响他人创建房间，请于游戏结束后退出游戏界面"), "Arial", 30);
	}
	x = rect.origin.x + rect.size.width / 2;
	y = rect.origin.y + rect.size.height / 3;
	word->setPosition(Vec2(x, y));
	word->setDimensions(1250.0f, 700.0f);
	v_layout->addChild(word);

}

void HelpScene::Page6(Layout * v_layout)
{
	Label *word;
	if (language_flag)
	{
		word = Label::create("Don't turn over, I have a bottom line", "Arial", 40);
	}
	else
	{
		word = Label::create(FontToUTF8("别翻了，我是有底线的"), "Arial", 40);
	}
	x = rect.origin.x + rect.size.width / 2;
	y = rect.origin.y + rect.size.height / 2;
	word->setPosition(Vec2(x, y));
	v_layout->addChild(word);

}

void HelpScene::Page7(Layout * v_layout)
{
	Label *word;
	if (language_flag)
	{
		word = Label::create("Why are you disobedient?", "Arial", 40);
	}
	else
	{
		word = Label::create(FontToUTF8("你这孩子怎么不听话呢？？？"), "Arial", 40);
	}
	x = rect.origin.x + rect.size.width / 2;
	y = rect.origin.y + rect.size.height / 2;
	word->setPosition(Vec2(x, y));
	v_layout->addChild(word);
}

void HelpScene::Page8(Layout * v_layout)
{
	Label *word;
	if (language_flag)
	{
		word = Label::create("Turn over again? Again?", "Arial", 40);
	}
	else
	{
		word = Label::create(FontToUTF8("还翻，还翻"), "Arial", 40);
	}
	x = rect.origin.x + rect.size.width / 2;
	y = rect.origin.y + rect.size.height / 2;
	word->setPosition(Vec2(x, y));
	v_layout->addChild(word, 1);

	auto miqi = Sprite::create("Scene/Background/miqi.png");
	miqi->setPosition(Vec2(x, y));
	v_layout->addChild(miqi, 0);
}

void HelpScene::Page9(Layout * v_layout)
{
	Label *word;
	if (language_flag)
	{
		word = Label::create("I guess you love me?!", "Arial", 40);
	}
	else
	{
		word = Label::create(FontToUTF8("你是不是喜欢我啊?!"), "Arial", 40);
	}
	x = rect.origin.x + rect.size.width / 2;
	y = rect.origin.y + rect.size.height / 2;
	word->setPosition(Vec2(x, y));
	v_layout->addChild(word);
}

void HelpScene::Page10(Layout * v_layout)
{
	Label *word;
	if (language_flag)
	{
		word = Label::create("Oh! You have been here, you must want to give me an A", "Arial", 40);
	}
	else
	{
		word = Label::create(FontToUTF8("都看到这了，不给优怎么说的过去嘞2333"), "Arial", 40);
	}
	x = rect.origin.x + rect.size.width / 2;
	y = rect.origin.y + rect.size.height / 2;
	word->setPosition(Vec2(x, y));
	v_layout->addChild(word);
}

void HelpScene::Page11(Layout * v_layout)
{
	Label *word;
	if (language_flag)
	{
		word = Label::create("Then let me tell you a story", "Arial", 40);
	}
	else
	{
		word = Label::create(FontToUTF8("那我给你讲个故事吧"), "Arial", 40);
	}
	x = rect.origin.x + rect.size.width / 2;
	y = rect.origin.y + rect.size.height / 2;
	word->setPosition(Vec2(x, y));
	v_layout->addChild(word);
}

void HelpScene::menuHellowWorldScene(Ref* pSender)
{

	auto sc = HelloWorld::createScene();        //按列分割界面的切换动画
	auto reScene = TransitionFadeBL::create(1.0f, sc);
	Director::getInstance()->replaceScene(reScene);
	this->getChildByName("pageView")->removeFromParentAndCleanup(true);
}