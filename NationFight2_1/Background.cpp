#include "../G2DEngine/Commen.h"
#include "Background.h"
#include "Game.h"
extern CGame g_game;

CBackground::CBackground(void)
{
	bShake=false;			//窗口震颤
	nBackBmpIndex=0;		//背景图片索引
	nX=0;					//背景往客户区上绘制的坐标
	nY=0;
	nWindowWidth=0;
	nWindowHeight=0;
	nShakeCounter=0;
}

CBackground::~CBackground(void)
{
}
void CBackground::init(int winWidth,int winHeight)
{
	nWindowWidth=winWidth;
	nWindowHeight=winHeight;
}
void CBackground::update()
{
	LBitmapInfo lbmi=g_game.m_backBmp.GetAt(nBackBmpIndex);
	//震颤处理
	if(bShake)
	{
		if(++nShakeCounter>=8)
		{
			bShake=false;
			nX=0;
			nShakeCounter=0;
		}
		if(nX == 0)
		{
			nX=-10;
		}
		else
		{
			if (nX > 0)
			{
				nX=-(nX-1);
			}
			else
			{
				nX=(-nX)-1;
			}
		}
	}
	//滚屏处理
	nY += 1;
	if(nY >= lbmi->nHeight)
	{
		nY=0;
	}
}
void CBackground::render(HDC hDC)
{
	LBitmapInfo lbmi=g_game.m_backBmp.GetAt(nBackBmpIndex);
	HDC memDC=CreateCompatibleDC(hDC);
	SelectObject(memDC,lbmi->hBitmap);
	if (bFlow)
	{
		BitBlt(hDC,nX,0,nWindowWidth,nY,memDC,0,lbmi->nHeight-nY,SRCCOPY);
		BitBlt(hDC,nX,nY,nWindowWidth,nWindowHeight-nY,memDC,0,0,SRCCOPY);
	}
	else
	{
		BitBlt(hDC,0,0,nWindowWidth,nWindowHeight,memDC,0,0,SRCCOPY);
	}
	DeleteObject(memDC);
}

	