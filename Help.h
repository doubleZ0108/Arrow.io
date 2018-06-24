#ifndef _HELP_H_
#define _HELP_H_
#include "cocos2d.h"
#include "GUI/CCControlExtension/CCControlSwitch.h" 
#include "ui/CocosGUI.h"  
USING_NS_CC;
using namespace ui;

#define pWidth  240  
#define pHeight 160  
class HelpScene : public cocos2d::Scene
{
private:
	float x, y;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Rect rect = Director::getInstance()->getOpenGLView()->getVisibleRect();

public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuHellowWorldScene(cocos2d::Ref* pSender);

	void ScenePrinter();
	void PagePrinter();

	void Page0(Layout *v_layout);
	void Page1(Layout *v_layout);
	void Page2(Layout *v_layout);
	void Page3(Layout *v_layout);
	void Page4(Layout *v_layout);
	void Page5(Layout *v_layout);
	void Page6(Layout *v_layout);
	void Page7(Layout *v_layout);
	void Page8(Layout *v_layout);
	void Page9(Layout *v_layout);
	void Page10(Layout *v_layout);
	void Page11(Layout *v_layout);

	void Func(cocos2d::Ref* pSender);
	// implement the "static create()" method manually
	CREATE_FUNC(HelpScene);


	/////////////////////////////////////////
	/*void initRole();
	static Scene* createS();
	void initS();*/
private:
	PageView * pageView;
	Vec2                    ClickBeganPos;  //µã»÷µÄ×ø±ê  
};

#endif //HELP