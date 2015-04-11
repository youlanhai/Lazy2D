//Game.h
#pragma once
#include "BitmapManager.h"
#include "Animation.h"
#include "Scene.h"
#include "Map.h"
#include "AStarSerach.h"
#include "Sprit.h"
#include "SpritManager.h"
#include "FightSystem.h"
#include "Speaker.h"

#include "App.h"

/**********************
游戏状态
**********************/
typedef enum
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
}GameState;

/****************************************************
游戏类
游戏初始化，游戏主进程等。
******************************************************/
class CGame : public CApp
{
public:
	CGame();						//构造函数
	~CGame();						//析构函数
	
	void Start();
	void Continue();
	void Exit();

protected:

    virtual bool onInit();//初始化游戏
    virtual void onTick();		//更新
    virtual void onDraw();		//渲染
    virtual void onClose();		//销毁资源
    virtual bool onWindowMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    void onRun();
	void onRender();				//游戏循环

	void SetGameState(GameState state){gameState=state;}

    void LoopBGMusic(int index);
private:
	CScene			scene;
	CAStarSearch	pathSystem;
    CGameMap		mapSytem;
	CFightSystem	fightSystem;
	CSpritManager	spritManager;
	CAnimation		animation;
	CSpeaker		speaker;	//对话系统
	CSprit			player;
	HFONT			hFont;//字体
	CGameForm		menuForm;
	CBitmapManager	menuButtonBitmap;
	GameState	    gameState;			//游戏状态
};