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
	CGameRect	fightM;//ս���˵�����
	CGameRect	fightMJ;//ս���˵����ܰ�ť����
	CGameRect	fightMS;//������ť����
	CGameRect	fightMG;//����
	CGameRect	fightMF;//��
	CGameRect	fightMW;//��
	CGameRect	fightMT;//��
	int		index;
	bool	down;
	static HWND hWnd;
	static void InitMenu(HWND hWnd,HINSTANCE hInst,std::string fileName);
	//Ϊÿ���˵���ӵ�����ͼƬ������
	static CBitmapManager menuBmp;
};