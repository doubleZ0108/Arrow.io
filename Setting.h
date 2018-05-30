#ifndef _SETTING_H__
#define _SETTING_H__
#include "cocos2d.h"

class SettingScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	// a selector callback
	void menuHellowWorldScene(cocos2d::Ref* pSender);

	void ScenePrinter();
	void MusicPrinter();
	void LanguagePrinter();
	void SizePrinter();
	//music setting
	bool is_paused;
	void play(cocos2d::Object* pSender);
	void stop(cocos2d::Object* pSender);

	void pause(cocos2d::Object* pSender);
	//language setting
	void language_change_tochinese(cocos2d::Object* pSender);
	void language_change_toenglish(cocos2d::Object* pSender);

	void size_change(cocos2d::Object* pSender);

	char *FontToUTF8(const char* font);

	void ScenePrinter_chinese();
	void MusicPrinter_chinese();
	void LanguagePrinter_chinese();
	// implement the "static create()" method manually
	CREATE_FUNC(SettingScene);
};

#endif // _SETTINGSCENE_H_
