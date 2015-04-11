//Game.h
#pragma once
#include "../G2DEngine/Commen.h"
#include "../G2DEngine/App.h"
#include "../G2DEngine/GameGUI.h"
#include "../G2DEngine/BitmapManager.h"
#include "../SoundEngine/DxMusic.h"
#include "../SoundEngine/DxSound.h"
#include "SpritManager.h"

/**********************
游戏状态
**********************/
enum GameState
{
	GAME_INIT,		//初始化
	GAME_HEADER,	//开头动画
	GAME_MENU,		//开始菜单
	GAME_RUN,		//游戏运行
	GAME_END,		//游戏结束
	GAME_SPEAKER,	//对话
	GAME_FIGHT,		//战斗
	GAME_OVER,		//战斗失败
	GAME_WIN		//战斗胜利
};

class CFPS;
class CKeyboard;
class CDxSound;
class CDxMusic;
class CBackground;
class Player;
class Enemy;
class CNFScript;

/****************************************************
游戏类
游戏初始化，游戏主进程等。
******************************************************/
class CGame : public CApp
{
public:
	CGame();						//构造函数
	~CGame();						//析构函数

protected:

    void onRegisterClass(WNDCLASSEX *pwc) override;	//注册窗口类
    bool onInit() override;		//初始化游戏
    void onDraw() override;			//渲染
    void onTick() override;			//更新
    void onClose() override;			//销毁资源
	//窗口过程
	bool onWindowMessage(
		HWND hWnd,UINT msg,
		WPARAM wParam,LPARAM lParam) override;

	void renderText();

	void setGameState(GameState gs);

public:
	void AddHero();//添加英雄
	void AddBullet(int x,int y,int index);//添加子弹
	void CheckHit();
	bool CheckHit(Object *obj1,Object *obj2);
public:
	HFONT		m_backFont;
	int			m_updateTimeInterval;	//更新时间间隔
	int			m_updateCounter;		//更新的帧数
	GameState	gameState;				//游戏状态
	CGameRect	m_clientRect;
    int         m_nMusicID;//背景音乐。-1无音乐。

	CDxSound	*m_pSound;
    CDxMusic    *m_pMusic;
	CBackground	*m_pBackground;
	CNFScript	*m_pScript;

	Player		*m_pPlayer;
    Enemy       *m_pBoss;

	CUIPane		m_pane;
	CUIDialog	m_Menu;
	CUIDialog	m_Help;

	CSpritManager	m_plyerManager;
	CSpritManager	m_enemyManager;
	CSpritManager	m_bulletManager;
	CSpritManager	m_enBulletManager;//敌人子弹
	CSpritManager	m_explodeManager;
	CSpritManager	m_prizeManager;

	CBitmapManager	m_heroBmp;
	CBitmapManager	m_enemyBmp;
	CBitmapManager  m_bulletBmp;
	CBitmapManager	m_backBmp;
	CBitmapManager	m_explodeBmp;
	CBitmapManager	m_prizeBmp;

public:
	void onMenu();
	void onRun();
	void onEnd();
	void onOver();
	void onWin();
	void onMenuStart();
	void onMenuHelp();
};