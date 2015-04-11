//FightSystem.h
#pragma once
#include "Effect.h"
#include "MagicSystem.h"
#include "GameMenu.h"
#include "GameObject.h"
#include "GameButton.h"
#include "GameForm.h"
/*
class CGameObject;
class CEffect;
class CMagicSystem;
class CGameMenu;
*/
typedef enum EnumFightMode
{
	FM_ATTACK,	//攻击
	FM_DEFENCE,	//防御
	FM_MAGIC,	//法术
	FM_GOODS,	//物品
	FM_ESCAPE,	//逃跑
	FM_NONE		//没有选择
}FightMode;//战斗模式

/***************************
战斗类
***************************/
class CFightSystem
{
public:
	CFightSystem();
	~CFightSystem();
	bool Initialize//初始化
	(
		HWND hWnd,
		HINSTANCE hInstance
	);
	void SetFighter(CGameObject* player,CGameObject* enemy);	//设置战士
	bool SetBackGroundBitmap(std::string strFileName);			//设置背景图片
	bool SetFighterHeadBitmap(std::string strFileName);			//设置头像图片组
	bool SetFightFormBitmap(std::string strFileName);			//设置战斗窗体图片组
	bool SetGoodsFormBitmap(std::string strFileName);			//设置物品窗体图片组
	bool SetUIBitmap(std::string strFileName);
	bool SetMagicList(std::string strFileName);					//设置魔法列表
	void SetFightMode(FightMode fmMode){fmFightMode=fmMode;}	//设置战斗模式
	void SetBackGroundRect(int x,int y,int width,int height);
	int FightLoop(HDC hDC);			//战斗循环
	void ChoiceFightMode(HDC hDC);	//选择战斗模式
	void ChoiceMagic();				//选择魔法
	int Attack();					//攻击
	int Deffence();					//防御
	int Magic();					//魔法
	int UseGoods();					//使用物品
	int Escape();					//逃跑
	void DrawBackGround(HDC hDC);	//绘制背景
	void DrawFighterState(HDC hDC);	//绘制精灵状态
	void DrawFighterState2(HDC hDC,int x,int y,CGameObject* pSprit);

    bool IsFighting() const { return bFighting; }
    int GetResult() const { return fightResult; }
private:
	HWND		hWindowHandle;		//窗口句柄
	HINSTANCE	hInstance;			//实例句柄
	int			nRound;				//回合计数
	bool		bPlayerDecision;	//玩家决策
	bool		bFighting;			//战斗进行中
	CGameRect	rcBackGround;		//背景区域
	CMagicSystem magicSystem;		//魔法系统
	CBitmapManager bmpBackground;	//背景图片组
	CBitmapManager bmpFighterHead;	//精灵战斗头像
	CGameObject* pPlayer;			//玩家
	CGameObject* pEnemy;			//敌人
	CGameRect	rcPlayer;
	CGameRect	rcEnemy;
	char szShowPlayerInformation[256];
	char szShowEnemyInformation[256];
	CEffect		playerEffect;		//玩家动作特效
	CEffect		enemyEffect;		//敌人动作特效
	CEffect		magicEffect;		//魔法特效
	bool		bSpritShowEnd;		//精灵动作特效结束
	bool		bMagicShowEnd;		//魔法特效结束
	FightMode	fmFightMode;		//作战方式
	int			nUseMagicID;		//魔法方式
	CGameForm	fightForm;			//选择战斗模式窗体
	CGameForm   goodsForm;			//选择魔法和物体窗体
	CBitmapManager	fightBitmap;	//选择战斗模式窗体图片组
	CBitmapManager	goodsBitmap;	//选择魔法和物体窗体图片组
	CBitmapManager	uiBitmap;		//UI图片
    int             fightResult;
};
