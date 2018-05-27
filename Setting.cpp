#include "Setting.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
USING_NS_CC;

Scene* SettingScene::createScene()
{
	return SettingScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool SettingScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}
	
	ScenePrinter();
	MusicPrinter();
	LanguagePrinter();
	SizePrinter();
	return true;
}

void SettingScene::ScenePrinter()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
	float x = rect.origin.x + rect.size.width / 2;
	float y = rect.origin.y + rect.size.height / 2;

	///////////////////////////////////////////
	//add Setting_Background
	
	x = rect.origin.x + rect.size.width / 2;
	y = rect.origin.y + rect.size.height / 2;
	auto *background = Sprite::create("Setting_Background.png");
	background->setPosition(Vec2(x, y));
	this->addChild(background);

	///////////////////////////////////
	//a return button which click to back to HelloWorldScene
	auto *return_button = MenuItemImage::create(
		"backtoupper.png",
		"backtoupper_select.png",
		CC_CALLBACK_1(SettingScene::menuHellowWorldScene, this));

	auto *preturn = Menu::create(return_button, NULL);
	x = rect.origin.x + rect.size.width*(10.0f / 11.0f);
	y = rect.origin.y + rect.size.height*(1.0f / 10.0f);
	preturn->setPosition(Vec2(x, y));
	
	preturn->setScale(1.0f);
	this->addChild(preturn);

	/////////////////////////////////////////
	//add  
	auto *musicbutton = Sprite::create("button.png");
	x = rect.origin.x + rect.size.width*(1.0f / 3.0f);
	y = rect.origin.y + rect.size.height*(2.0f / 3.0f);
	musicbutton->setPosition(Vec2(x, y));
	this->addChild(musicbutton);
	auto *musicword = Label::createWithTTF("Music",
		"fonts/Marker Felt.ttf", 40);
	musicword->setPosition(Vec2(x, y));
	this->addChild(musicword);

	auto *languagebutton = Sprite::create("button.png");
	y = rect.origin.y + rect.size.height*(1.0f / 2.0f);
	languagebutton->setPosition(Vec2(x, y));
	this->addChild(languagebutton);
	auto *languageword = Label::createWithTTF("Language",
		"fonts/Marker Felt.ttf", 40);
	languageword->setPosition(Vec2(x, y));
	this->addChild(languageword);

	auto *sizebutton = Sprite::create("button.png");
	y = rect.origin.y + rect.size.height*(1.0f / 3.0f);
	sizebutton->setPosition(Vec2(x, y));
	this->addChild(sizebutton);
	auto *sizeword = Label::createWithTTF("Size",
		"fonts/Marker Felt.ttf", 40);
	sizeword->setPosition(Vec2(x, y));
	this->addChild(sizeword);
}
void SettingScene::MusicPrinter()
{
	auto rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
	float x, y;
	is_paused = false;
	//play button
	auto* label_play = Label::create("play", "Arial", 40);
	auto* pLabel_play = MenuItemLabel::create(label_play, this, menu_selector(SettingScene::play));
	auto* button_play = Menu::create(pLabel_play, NULL);
	x = rect.origin.x + rect.size.width*(1.0f / 2.0f);
	y = rect.origin.y + rect.size.height*(2.0f / 3.0f);
	button_play->setPosition(Vec2(x, y));
	button_play->setColor(Color3B::BLACK);
	this->addChild(button_play);
	//pause button
	auto* label_pause = Label::create("pause", "Arial", 40);
	auto* pLabel_pause = MenuItemLabel::create(label_pause, this, menu_selector(SettingScene::pause));
	auto* button_pause = Menu::create(pLabel_pause, NULL);
	x = rect.origin.x + rect.size.width*(7.0f / 12.0f);
	button_pause->setPosition(Vec2(x, y));
	button_pause->setColor(Color3B::BLACK);
	this->addChild(button_pause);
	//stop button
	auto* label_stop = Label::create("stop", "Arial", 40);
	auto* pLabel_stop = MenuItemLabel::create(label_stop, this, menu_selector(SettingScene::stop));
	auto* button_stop = Menu::create(pLabel_stop, NULL);
	x = rect.origin.x + rect.size.width*(2.0f / 3.0f);
	button_stop->setPosition(Vec2(x, y));
	button_stop->setColor(Color3B::BLACK);
	this->addChild(button_stop);
}
void SettingScene::LanguagePrinter()
{
	auto rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
	float x, y;
	//中文按钮
	auto* chinese_word = Label::create("Chinses", "Arial", 40);
	auto* pchinese_word = MenuItemLabel::create(chinese_word, this, menu_selector(SettingScene::language_change));
	auto* chinese_button = Menu::create(pchinese_word, NULL);
	x = rect.origin.x + rect.size.width*(21.0f / 40.0f);
	y = rect.origin.y + rect.size.height*(1.0f / 2.0f);
	chinese_button->setPosition(Vec2(x, y));
	chinese_button->setColor(Color3B::BLACK);
	this->addChild(chinese_button);
	//英文按钮
	auto* english_word = Label::create("English", "Arial", 40);
	auto* penglish_word = MenuItemLabel::create(english_word, this, menu_selector(SettingScene::language_change));
	auto* english_button = Menu::create(penglish_word, NULL);
	x = rect.origin.x + rect.size.width*(77.0f / 120.0f);
	english_button->setPosition(Vec2(x, y));
	english_button->setColor(Color3B::BLACK);
	this->addChild(english_button);

}
void SettingScene::SizePrinter()
{

}

void SettingScene::menuHellowWorldScene(Ref* pSender)
{
	Director::getInstance()->popScene();
}
void SettingScene::play(cocos2d::Object* pSender)
{
	if (is_paused)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	}
	else
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Escape.mp3");
	}
	is_paused = false;
}
void SettingScene::stop(cocos2d::Object* pSender)
{
	is_paused = false;
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}
void SettingScene::pause(cocos2d::Object* pSender)
{
	is_paused = true;
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void SettingScene::language_change(cocos2d::Object * pSender)
{
	
}
void SettingScene::size_change(cocos2d::Object * pSender)
{

}
