#ifndef _GAMEPLAYING_H_
#define _GAMEPLAYING_H_

#include "cocos2d.h"
#include "Player.h"
#include "BulletBase.h"
#include "ProgressView.h"
#include "ExpProgress.h"
#include <windows.h>  
#include <vector>
#include "network\SocketIO.h"

#pragma comment(lib,"libcurl_imp.lib")

#define KEY_DOWN(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 1 : 0)  
#define KEY_UP(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 0 : 1)  

#define k_w (EventKeyboard::KeyCode)146
#define k_a (EventKeyboard::KeyCode)124
#define k_s (EventKeyboard::KeyCode)142
#define k_d (EventKeyboard::KeyCode)127

#define MAP_SIZE 1598
#define MAP_WIDTH 49
#define MAP_HEIGHT 49

#define DIFF 16   //人物和墙壁间的距离差值（比较玄学的测试，主要用于检测某一个方向是否有不可以走的地方

#define GAP1_GID 145
#define NOR1_GID 138
#define HP1_GID 137
#define EXP1_GID 142

#define GAP2_GID 129
#define NOR2_GID 122
#define HP2_GID 123
#define EXP2_GID 128

#define GAP3_GID 405
#define NOR3_GID 398
#define HP3_GID 399
#define EXP3_GID 404

#define GAP_GID (1==which_map?GAP1_GID : (2==which_map? GAP2_GID : GAP3_GID))
#define NOR_GID (1==which_map?NOR1_GID : (2==which_map? NOR2_GID : NOR3_GID))
#define HP_GID (1==which_map?HP1_GID : (2==which_map? HP2_GID : HP3_GID))
#define EXP_GID (1==which_map?EXP1_GID : (2==which_map? EXP2_GID : EXP3_GID))

#define SM_MAP_SIZE 245
#define RETE (260.0/1600)  //smallplayer和player移动距离的比
#define XIE 0.707
#define TLMAP_SCALE (viewsize==1? 1.0 :(viewsize==1.75? 2.0: 0.8))//地图缩放时的比例调整
#define MOUCE_SCALE (viewsize==1? 1.0 :(viewsize==2? 1.35: 0.8))//地图缩放时鼠标的比例调整
USING_NS_CC;
using namespace cocos2d::network;
struct HP_MESS
{
	Sprite* hp_potion;		//道具药水图片的精灵
	int savex, savey;		//储存下道具的位置用于检索
	HP_MESS(Sprite* buf, int randx, int randy)
		:hp_potion(buf), savex(randx), savey(randy) {}
	//重载==运算符，否则的话调用std::find()函数时会出bug
	//std::find()的内部实现应该是借用了==运算符
	bool operator==(const HP_MESS &thv)
	{
		if (this->savex == thv.savex && this->savey == thv.savey)
		{
			return true;
		}
		else { return false; }
	}
};
struct EXP_MESS
{
	Sprite* exp_potion;		//道具药水图片的精灵
	int savex, savey;		//储存下道具的位置用于检索
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
class GamePlaying : public cocos2d::Layer, public cocos2d::network::SocketIO::SIODelegate
{
private:

	/////////////////////////////////////////////
	//各种坐标的计算
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Rect rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
	/////////////////////////////////////////////

private:
	Size size;
	Sprite* sprite;

	Size mapSize;			//地图的尺寸
	Size tileSize;			//瓦片的尺寸
	float x_move = 0.0;     //用于地图调整之后鼠标点击的位置做相应的数值转换
	float y_move = 0.0;
	TMXTiledMap* tiledmap;
	TMXLayer* meta;

public:

	static cocos2d::Scene* createScene();

	virtual bool init();

	//////////////////////////////////
	//各种界面的绘制
	void NetworkPrinter();
	void PlayerPrinter();
	void SettingPrinter();
	void MapPrinter();
	void ScenePrinter();
	void MusicPrinter();
	void SmallmapPrinter();
	void ModePrinter();
	////////////////////////////////////

	void Weapon_change(Ref* pSender);
	void TLmap_change(Ref* pSender);
	void Breakwall_change(Ref* pSender);

	///////////////////////////////////
	//各种开关的回调函数
	void menuHelloWorldScene(Ref* pSender);
	void Network_Switch(Ref* Spender);
	void Smallmap_Switch(Ref* pSender);
	void Mode_Switch(Ref* pSender);
	void Music_Switch(Ref* pSender);
	////////////////////////////////////

	////////////////////////////////////
	//各个方向的判断和墙壁检测
	bool up(bool flag, int ifxie = 0);			//true代表我需要调用runEvent函数实实在在的移动
	bool right(bool flag, int ifxie = 0);		//false代表我只是想判断这个方向能不能走，其实不想移动
	bool left(bool flag, int ifxie = 0);
	bool down(bool flag, int ifxie = 0);
	bool isCanReach(float x, float y);
	//////////////////////////////////

	/////////////////////////////////////////
	//道具的随机出现函数 和 判断并吃掉道具的函数
	static std::vector<HP_MESS> hp_auto_arise;
	static std::vector<EXP_MESS> exp_auto_arise;

	void HPjudge(const Vec2 &pos);
	void HP_grow(float dt);
	void EXPjudge(const Vec2 &pos);
	void EXP_grow(float dt);
	void tofindEat(const float x, const float y);
	void HPEXP_clear();
	///////////////////////////////////////////


	//////////////////////////////////////
	//网络  信息接收  信息解码
	SIOClient * _sioClient;
	//socket连接时调用
	void onConnect(SIOClient* client) {}
	//收到数据时调用
	void onMessage(SIOClient* client, const std::string& data) {}
	//连接错误或接收到错误信号时调用
	void onError(SIOClient* client, const std::string& data) {}
	//socket关闭时调用
	void onClose(SIOClient* client) {}

	void DeCode_for_Map(const std::string &buf, int &metax, int &metay);
	void DeCode_for_Run(const std::string &buf);
	void DeCode_for_Playrun(const std::string &buf);
	void DeCode_for_Enemy(const std::string &buf,
		int &v_player, float &v_posx, float &v_posy,
		float &v_hp, int &v_hplimit, int &v_weapon);
	void DeCode_for_attack(const std::string &buf,
		int &v_weapon, float &v_posx, float &v_posy);
	void DeCode_for_heronature(const std::string &buf,
		float &v_speed, float &v_p_hp, int &v_hpLimit,
		float &v_atkpower, float &v_atkrange, float &v_defpower,
		int &v_front, int &v_leftside, int &v_rightside,
		int &v_back, bool &v_ifcanbreakwall);
	void DeCode_for_hurt(const std::string &buf, int &v_bloodmin);
	void DeCode_for_restart(const std::string &buf, float &v_posx, float &v_posy);

	void ID_judge(SIOClient* client, const std::string& data);
	void HP_recieve(SIOClient* client, const std::string& data);
	void EXP_recieve(SIOClient* client, const std::string& data);
	void HP_remove(SIOClient* client, const std::string& data);
	void EXP_remove(SIOClient* client, const std::string& data);
	void addEnemy(SIOClient* client, const std::string& data);

	void runEvent_n(SIOClient* client, const std::string& data);
	void playRun_n(SIOClient* client, const std::string& data);
	void attack_n(SIOClient* client, const std::string& data);
	void heronature_n(SIOClient* client, const std::string& data);
	void hurt_n(SIOClient* client, const std::string& data);
	void restart_n(SIOClient* client, const std::string& data);
	void redhp_n(SIOClient* client, const std::string& data);
	void roomcrowded(SIOClient* client, const std::string& data);
	/////////////////////////////////////////

	// implement the "static create()" method manually
	CREATE_FUNC(GamePlaying);

	virtual void update(float delta);
	void runEvent();
	void onEnter();
	void attack(Player* player, Point point);//单击坐标
	void attackweapon(Player* player, float num, Point point);
	void attack_nn(Player* player, Point point);
	void attackweapon_nn(Player* player, float num, Point point);
	void levelup();
	/////////////////////////
private:
	Point pos;					//单击坐标，用于攻击
	Player * m_player = Player::create();	//主角1
	Player* n_player = Player::create();       //主角2
	std::vector<Player*>plsum;
	std::vector<BulletBase*>bubsum;
	bool waytorun = true;	   //true->键盘移动鼠标攻击
	///////////////////////////////
	//血条
	ProgressView *m_pProgressView;
	ProgressView *n_pProgressView;
	/////////////////////////
	//经验条
	ExpProgress *expPro;
	ExpProgress *expPro_n;
	Label *lv;
	int live = 0;

	Sprite *m_smallmap;
	Player *m_smallplayer;
	Player *n_smallplayer;

	///////////////////////////////
	EventListenerKeyboard *keylistener;
	EventListenerMouse *mouselistener;
	EventDispatcher *eventDispatcher1;
	EventDispatcher *eventDispatcher2;
	std::map<EventKeyboard::KeyCode, bool>keys;//记录按键状态
	std::map<EventKeyboard::KeyCode, bool>keys1;
	std::map<EventKeyboard::KeyCode, bool>keys_n;
	std::map<EventKeyboard::KeyCode, bool>keys1_n;
	bool touchon = false;//是否单击
};

#endif //_GAMEPLAYING_H_