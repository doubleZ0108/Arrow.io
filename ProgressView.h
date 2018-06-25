#ifndef __PROGRESSVIEW_H__  
#define __PROGRESSVIEW_H__  

#include "cocos2d.h"  
using namespace cocos2d;

class ProgressView : public CCNode
{
public:
	ProgressView();

public:
	void setBackgroundTexture(const char *pName);//ÉèÖÃÑªÌõ
	void setForegroundTexture(const char *pName);//ÉèÖÃÑªÁ¿
	void setTotalProgress(float total);//×ÜÑªÁ¿
	void setCurrentProgress(float progress);//µ±Ç°ÑªÁ¿
	void setpoint(Point point);//ÉèÖÃ×ø±ê

private:
	void setForegroundTextureRect(const CCRect &rect);

private:
	CCSprite * m_progressBackground;
	CCSprite *m_progressForeground;
	float m_totalProgress;
	float m_currentProgress;
	float m_scale;
};

#endif