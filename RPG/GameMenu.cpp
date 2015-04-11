//GameMenu.cpp
#include "Commen.h"
#include "GameMenu.h"

HWND CGameMenu::hWnd=NULL;
CBitmapManager CGameMenu::menuBmp=CBitmapManager();

CGameMenu::CGameMenu()
{
	index=0;
	down=false;
}
CGameMenu::~CGameMenu()
{

}
int CGameMenu::CheckCursor()
{
	static UINT nOldTime=0;
	UINT nNewTime=timeGetTime();
	if(nNewTime-nOldTime<50)
	{
		return 0;
	}
	nOldTime=nNewTime;
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(hWnd,&pt);
	if(KEYDOWN(VK_LBUTTON))//(key[VK_RBUTTON])
	{
		down=true;
	}
	else 
		down=false;
	if(fightM.IsIn(&pt))
	{
		if(fightMJ.IsIn(&pt))
		{
			if(!down)
				index=1;
			if(down)
				index=2;
		}
		else if(fightMS.IsIn(&pt))
		{
			if(!down)
				index=3;
			if(down)
				index=4;
		}
		else if(fightMG.IsIn(&pt))
		{
			if(!down)
				index=5;
			if(down)
				index=6;
		}	
		else if(fightMF.IsIn(&pt))
		{
			if(!down)
				index=7;
			if(down)
				index=8;
		}
		else if(fightMW.IsIn(&pt))
		{
			if(!down)
				index=9;
			if(down)
				index=10;
		}
		else if(fightMT.IsIn(&pt))
		{
			if(!down)
				index=11;
			if(down)
				index=12;
		}
		else
			index=0;
	}
	else
		index=0;
	return index;
}

void CGameMenu::TransBlt(HDC hDCDest,HDC hDCSrc,const CGameRect &rc)
{
	TransparentBlt(hDCDest,rc.x,rc.y,rc.w,rc.h,
		hDCSrc,0,0,rc.w,rc.h,GetPixel(hDCSrc,0,0));
}
void CGameMenu::InitMenu(HWND hWn,HINSTANCE hInst,std::string fileName)
{	
	hWnd=hWn;
	menuBmp.CreateBitmapFromFileList(hInst,fileName);
}
void CGameMenu::SetFightMPos(int x,int y)
{
	LBitmapInfo bmi=menuBmp.GetAt(18);
	fightM=CGameRect(x,y,bmi->nWidth,bmi->nHeight);
	bmi=menuBmp.GetAt(0);
	fightMJ=CGameRect(x+54,y,bmi->nWidth,bmi->nHeight);
	bmi=menuBmp.GetAt(3);
	fightMS=CGameRect(x,y+52,bmi->nWidth,bmi->nHeight);
	bmi=menuBmp.GetAt(6);
	fightMG=CGameRect(x+51,y+52,bmi->nWidth,bmi->nHeight);
	bmi=menuBmp.GetAt(9);
	fightMF=CGameRect(x+108,y+54,bmi->nWidth,bmi->nHeight);
	bmi=menuBmp.GetAt(12);
	fightMW=CGameRect(x+54,y+108,bmi->nWidth,bmi->nHeight);
	bmi=menuBmp.GetAt(15);
	fightMT=CGameRect(x+86,y+86,bmi->nWidth,bmi->nHeight);
}
void CGameMenu::DrawFightM(HDC hDC)
{
	//主菜单
	LBitmapInfo bmi=menuBmp.GetAt(18);
	HDC memDC=CreateCompatibleDC(hDC);
	SelectObject(memDC,bmi->hBitmap);
	TransBlt(hDC,memDC,fightM);
	
	int id;
	//技按钮
	if(index==1)
		id=1;
	else if(index==2)
		id=2;
	else id=0;
	bmi=menuBmp.GetAt(id);
	SelectObject(memDC,bmi->hBitmap);
	TransBlt(hDC,memDC,fightMJ);
	//术按钮
	if(index==3)
		id=4;
	else if(index==4)
		id=5;
	else id=3;
	bmi=menuBmp.GetAt(id);
	SelectObject(memDC,bmi->hBitmap);
	TransBlt(hDC,memDC,fightMS);
	//击按钮
	if(index==5)
		id=7;
	else if(index==6)
		id=8;
	else id=6;
	bmi=menuBmp.GetAt(id);
	SelectObject(memDC,bmi->hBitmap);
	TransBlt(hDC,memDC,fightMG);
	//防
	if(index==7)
		id=10;
	else if(index==8)
		id=11;
	else id=9;
	bmi=menuBmp.GetAt(id);
	SelectObject(memDC,bmi->hBitmap);
	TransBlt(hDC,memDC,fightMF);
	//物
	if(index==9)
		id=13;
	else if(index==10)
		id=14;
	else id=12;
	bmi=menuBmp.GetAt(id);
	SelectObject(memDC,bmi->hBitmap);
	TransBlt(hDC,memDC,fightMW);
	//逃
	if(index==11)
		id=16;
	else if(index==12)
		id=17;
	else id=15;
	bmi=menuBmp.GetAt(id);
	SelectObject(memDC,bmi->hBitmap);
	TransBlt(hDC,memDC,fightMT);

	DeleteDC(memDC);
}