//Game.h
#pragma once
#include "../G2DEngine/App.h"
#include "../G2DEngine/GDI.h"

enum GAME_UI_ID
{
    GUID_STATIC = 100,
    GUID_TALK ,
    GUID_MP3,
    GUID_FORCE_WORD = 0xffffffff
};

enum GAME_TIMER_ID
{
    GTID_GAME_LOOP = 100,
    GTID_FORCE_DWORD = 0xffffffff
};


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

class CSnow;
class CExplode;

class CBrushPanel;
class CLRCLoader;
class CMp3Player;
class CBitmapViewer;
class CTimer;
class CHero;


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
    /*注册窗口类*/
    void onRegisterClass(WNDCLASSEX *pwc);	

    /*初始化游戏*/
    bool onInit() ; 

    /*更新*/
    void onTick() ;	

    /*渲染*/
    void onDraw() ;	

    /*销毁资源*/	
    void onClose() ;	

	void renderText();
	void renderUI();

    bool initUI();

public:
    GameState	gameState;			//游戏状态
    CGameRect	m_clientRect;
    CFont		m_backFont;

    RefPtr<CTimer>          m_pTimer;
    RefPtr<CKeyboard>	    m_pKeybord;

    CBitmapManager m_bmpManager;
    CBitmapManager m_bmpSprit;

    CUIPane     m_pane;
    CUIDialog   m_dlgTalk;
    CMp3Player  *m_pMP3;

    RefPtr<CBitmapViewer>   m_pBmpViewer;
    RefPtr<CExplode>        m_pExplode;
    RefPtr<CSnow>           m_pSnow;
    RefPtr<CBrushPanel>     m_pBrush;
    RefPtr<CFire>           m_pFire;
    RefPtr<CHero>           m_pJJ;
    
};