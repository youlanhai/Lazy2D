//GameButton.cpp
#include "Commen.h"
#include "GameButton.h"
#include "BitmapManager.h"

CBitmapManager CGameButton::bmpCommenButton=CBitmapManager();

bool CGameButton::InitializeCommen(HINSTANCE hInstance,std::string strBmpListName)
{
	if(!bmpCommenButton.CreateBitmapFromFileList(hInstance,strBmpListName))
		return false;
	return true;
}

CGameButton::CGameButton()
{
	bsState=BUTTON_NORMAL;
	bsStyle=BUTTON_STATIC;
	pButtonCall=NULL;
	crBackColor=crForeColor=0;
}
CGameButton::~CGameButton()
{

}
CGameButton::CGameButton(std::string caption,ButtonStyle style,GameButtonCall pFuction)
{
	SetCaption(caption);
	SetStyle(style);
	SetButtonCall(pFuction);
}
void CGameButton::InitButton
(
	std::string caption,ButtonStyle style,
	COLORREF foreColor,COLORREF backColor,
	GameButtonCall pFuction
)
{
	SetCaption(caption);
	SetStyle(style);
	SetColor(foreColor,backColor);
	SetButtonCall(pFuction);
}
ButtonState CGameButton::ProcessMouseMessage(POINT* ptMouse,bool bDown,void* pParam)
{
	if(rect.IsIn(ptMouse))
	{
		if(bDown)
		{
			bsState=BUTTON_DOWN;
			if(pButtonCall)
				pButtonCall(pParam);
		}
		else bsState=BUTTON_ACTIVE;
	}
	else bsState=BUTTON_NORMAL;
	return bsState;
}
void CGameButton::SetRect(int x,int y,int w,int h)
{
	SetRect(&CGameRect(x,y,w,h));	
}
void CGameButton::SetRect(CGameRect* rc)
{
	if(NULL==rc)return;
	rect=*rc;
	if(0==rect.w || 0==rect.h)
	{
		if(bmpButton[0].hBitmap!=NULL)//先考虑手绘位图
		{
			rect.w=bmpButton[0].nWidth;
			rect.h=bmpButton[0].nHeight;
		}
		else
		{
			LBitmapInfo bmi=bmpCommenButton.GetAt(0);
			if(bmi)//再考虑公共位图
			{
				rect.w=bmi->nWidth;
				rect.h=bmi->nHeight;
			}
			else//最后是默认大小
			{
				rect.w=100;
				rect.h=25;
			}
		}
	}
}
void CGameButton::SetBitmap(CBitmapManager* bmpManager,int nStartPosition)
{
	LBitmapInfo bmi=bmpManager->GetAt(nStartPosition);
	if(bmi)
		bmpButton[0]=*bmi;
	bmi=bmpManager->GetAt(nStartPosition+1);
	if(bmi)
	{
		bmpButton[1]=*bmi;
	}
	bmi=bmpManager->GetAt(nStartPosition+2);
	if(bmi)
	{
		bmpButton[2]=*bmi;
	}
}

void CGameButton::Show(HDC hDC)
{
	HDC hMemDC=CreateCompatibleDC(0);
	if(bsStyle==BUTTON_DRAW)//自定义位图绘制
	{
		if(bmpButton[bsState].hBitmap)
		{
			SelectObject(hMemDC,bmpButton[bsState].hBitmap);
			TransparentBlt(
				hDC,rect.x,rect.y,
				rect.w,rect.h,
				hMemDC,0,0,
				bmpButton[bsState].nWidth,bmpButton[bsState].nHeight,
				GetPixel(hMemDC,0,0));
		}
	}
	else if(bsStyle==BUTTON_PUSH)//公共位图绘制
	{
		LBitmapInfo bmi=bmpCommenButton.GetAt(bsState);
		if(bmi)
		{
			SelectObject(hMemDC,bmi->hBitmap);
			TransparentBlt(
				hDC,rect.x,rect.y,
				rect.w,rect.h,
				hMemDC,0,0,
				bmi->nWidth,bmi->nHeight,
				GetPixel(hMemDC,0,0));
		}
	}
	else if(bsStyle==BUTTON_STATIC)//单位图或无位图绘制
	{
		if(bmpButton[0].hBitmap)
		{
			SelectObject(hMemDC,bmpButton[0].hBitmap);
			TransparentBlt(
				hDC,rect.x,rect.y,
				rect.w,rect.h,
				hMemDC,0,0,
				bmpButton[0].nWidth,bmpButton[0].nHeight,
				GetPixel(hMemDC,0,0));
		}
		else//绘制矩形
		{
			HBRUSH hBrush=CreateSolidBrush(crBackColor);
			HBRUSH hOldBrush=(HBRUSH)SelectObject(hDC,hBrush);
			Rectangle(hDC,rect.x,rect.y,rect.x+rect.w,rect.y+rect.h);
			SelectObject(hDC,hOldBrush);
			DeleteObject(hBrush);
		}
	}
	if(!strCaption.empty())//绘制文本
	{
		COLORREF crOldColor=SetTextColor(hDC,crForeColor);
		DrawText(hDC,strCaption.c_str(),strCaption.size(),
			rect.ToRECT(),DT_VCENTER|DT_CENTER|DT_SINGLELINE);
		SetTextColor(hDC,crOldColor);
	}
	DeleteDC(hMemDC);
}