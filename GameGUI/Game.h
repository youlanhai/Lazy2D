//Game.h
#pragma once
#include "../G2DEngine/App.h"
#include "../G2DEngine/GameGUI.h"
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

/****************************************************
游戏类
游戏初始化，游戏主进程等。
******************************************************/
class CGame : public CApp
{
public:
	CGame();						//构造函数
	~CGame();						//析构函数
	//窗口过程
	static LRESULT CALLBACK windowProc(
		HWND hWnd,UINT msg,
		WPARAM wParam,LPARAM lParam);
	void registerClass();	//注册窗口类
	bool initGame();		//初始化游戏
	bool initUI();
	void render();			//渲染
	void renderText();
	void renderUI();
	void update();			//更新
	void destroy();			//销毁资源

public:
	GameState	gameState;			//游戏状态
	CGameRect	m_clientRect;

	HFONT		m_backFont;
	CFPS		*m_pFPS;
	CKeyboard	*m_pKeybord;

	CBitmapManager m_bmpManager;
	CUIDialog	m_dialog;
	CUIPane		m_pane;
};