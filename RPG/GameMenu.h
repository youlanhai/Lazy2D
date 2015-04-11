//CGameMenu.h
#pragma once
#include "BitmapManager.h"

class CGameMenu
{
public:
	CGameMenu();
	~CGameMenu();
public:
	int CheckCursor();
	void SetFightMPos(int x,int y);
	void DrawFightM(HDC hDC);
	void TransBlt(HDC hDCDest,HDC hDCSrc,const CGameRect &rc);
	CGameRect	fightM;//战斗菜单区域
	CGameRect	fightMJ;//战斗菜单技能按钮区域
	CGameRect	fightMS;//法术按钮区域
	CGameRect	fightMG;//攻击
	CGameRect	fightMF;//防
	CGameRect	fightMW;//物
	CGameRect	fightMT;//逃
	int		index;
	bool	down;
	static HWND hWnd;
	static void InitMenu(HWND hWnd,HINSTANCE hInst,std::string fileName);
	//为每个菜单添加单独的图片管理器
	static CBitmapManager menuBmp;
};