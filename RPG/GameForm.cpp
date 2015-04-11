//GameForm
#include "Commen.h"
#include "BitmapManager.h"
#include "GameForm.h"
#include "GameButton.h"

CGameForm::CGameForm()
{
	crBackColor=0;
	crForeColor=0;
	fsState=FORM_HIDE;
}
CGameForm::~CGameForm()
{
	ClearButton();
}
void CGameForm::InitForm
(
	std::string caption,FormStyle style,
	int x,int y,
	int width,int height,
	COLORREF foreColor,COLORREF backColor
)
{
	SetCaption(caption);
	SetStyle(style);
	SetRect(x,y,width,height);
	SetColor(foreColor,backColor);
}
void CGameForm::SetCaption(std::string caption)
{
	strCaption=caption;
}
void CGameForm::SetColor(COLORREF foreColor,COLORREF backColor)
{
	crForeColor=foreColor;
	crBackColor=backColor;
	if(foreColor==backColor)
	{
		foreColor=RGB(0,0,0);
		backColor=RGB(255,255,255);
	}
}
void CGameForm::SetRect(int x,int y,int w,int h)
{
	SetRect(&CGameRect(x,y,w,h));
}
void CGameForm::SetRect(CGameRect* rc)
{
	if(rc!=NULL)
	{
		rect=*rc;
		if(0==rect.w || 0==rect.h)
		{
			if(bmpBackGround.hBitmap!=NULL)
			{
				rect.w=bmpBackGround.nWidth;
				rect.h=bmpBackGround.nHeight;
			}
			else
			{
				rect.w=300;
				rect.h=200;
			}
		}
	}
	rcClose.w=30;
	rcClose.h=30;
	rcClose.x=rect.w-rcClose.w;
	rcClose.y=0;
}
void CGameForm::SetBitmap(CBitmapManager* bmpManager,int nStartPosition)
{
	LBitmapInfo bmi=bmpManager->GetAt(nStartPosition);
	if(bmi!=NULL)
	{
		bmpBackGround=*bmi;
	}
}
void CGameForm::Show(HDC hDC)
{
	if(fsState!=FORM_SHOW)
		return ;
	HDC hBackDC=CreateCompatibleDC(0);
	HBITMAP hBackBmp=CreateCompatibleBitmap(hDC,rect.w,rect.h);
	SelectObject(hBackDC,hBackBmp);

	if(FORM_NONE!=fsStyle)
	{
		if(bmpBackGround.hBitmap)
		{//用位图绘制背景
			HDC hMemDC=CreateCompatibleDC(0);
			SelectObject(hMemDC,bmpBackGround.hBitmap);
			TransparentBlt(hBackDC,0,0,rect.w,rect.h,
				hMemDC,0,0,bmpBackGround.nWidth,bmpBackGround.nHeight,
				GetPixel(hMemDC,0,0));
			DeleteDC(hMemDC);
		}
		else
		{//用画刷绘制背景
			HBRUSH hBrush=CreateSolidBrush(crBackColor);
			HBRUSH hOldBrush=(HBRUSH)SelectObject(hBackDC,hBrush);
			HPEN hPen=CreatePen(PS_SOLID,1,crForeColor);
			HPEN hOldPen=(HPEN)SelectObject(hBackDC,hPen);
			Rectangle(hBackDC,0,0,rect.w,rect.h);
			if(FORM_CAPTION==fsStyle)
			{
				//绘制关闭按钮
				HBRUSH hRedBrush=CreateSolidBrush(RGB(255,0,0));
				SelectObject(hBackDC,hRedBrush);
				Rectangle(hBackDC,rcClose.x,rcClose.y,rcClose.x+rcClose.w,rcClose.y+rcClose.h);
				//绘制标题、窗体分割线
				MoveToEx(hBackDC,0,rcClose.h,NULL);
				LineTo(hBackDC,rect.w,rcClose.h);
				DeleteObject(hRedBrush);
			}
			SelectObject(hBackDC,hOldPen);
			SelectObject(hBackDC,hOldBrush);
			DeleteObject(hBrush);
			DeleteObject(hPen);
		}
		if(FORM_CAPTION==fsStyle)	
		{//绘制标题
			if(!strCaption.empty())
			{
				SetTextColor(hBackDC,crForeColor);
				SetBkMode(hBackDC,TRANSPARENT);
				CGameRect rc=CGameRect(0,0,rect.w,rcClose.h);
				DrawText(hBackDC,strCaption.c_str(),strCaption.size(),
					rc.ToRECT(),DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
		}
	}
	//绘制按钮
	{
		CGameButton* pButton=NULL;
		int nButtonCount=vecButtons.size();
		for(int i=0;i<nButtonCount;++i)
		{
			pButton=vecButtons.at(i);
			if (pButton)
			{
				pButton->Show(hBackDC);
			}
		}
	}
	TransparentBlt(hDC,rect.x,rect.y,rect.w,rect.h,
		hBackDC,0,0,rect.w,rect.h,GetPixel(hBackDC,0,0));
	DeleteObject(hBackBmp);
	DeleteDC(hBackDC);
}
void CGameForm::ProcessMessage(HWND hWnd,void* pParam)
{
	if(fsState!=FORM_SHOW)
		return;
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd,&ptMouse);
	ClientToForm(&ptMouse);
	bool bLButtonDown=false;
	if(KEYDOWN(VK_LBUTTON))
		bLButtonDown=true;//鼠标左键按下
	if(FORM_CAPTION==fsStyle)
	{//处理关闭按钮消息
		if(bLButtonDown)
		{
			if(rcClose.IsIn(&ptMouse))
			{
				fsState=FORM_HIDE;
			}
		}
	}
	//处理按钮组消息
	{
		CGameButton* pButton=NULL;
		int nButtonCount=vecButtons.size();
		for(int i=0;i<nButtonCount;++i)
		{
			pButton=vecButtons.at(i);
			if(pButton)
			{
				pButton->ProcessMouseMessage(&ptMouse,bLButtonDown,pParam);
			}
		}
	}
}
/*添加按钮
遵循外部分配资源，内部释放的原则
*/
void CGameForm::AddButton(CGameButton* button)
{
	vecButtons.push_back(button);
}
//释放按钮组资源
void CGameForm::ClearButton()
{
	CGameButton* pButton=NULL;
	int nButtonCount=vecButtons.size();
	for(int i=0;i<nButtonCount;++i)
	{
		pButton=vecButtons.at(i);
		if(pButton)
		{
			delete pButton;
			pButton=NULL;
		}
	}
	vecButtons.clear();
}
void CGameForm::ClientToForm(POINT* pt)
{
	if(pt!=NULL)
	{
		pt->x-=rect.x;
		pt->y-=rect.y;
	}
}
void CGameForm::ClientToForm(RECT* rc)
{
	if(rc!=NULL)
	{
		rc->left-=rect.x;
		rc->top-=rect.y;
		rc->right-=rect.x;
		rc->bottom-=rect.y;
	}
}
void CGameForm::ClientToForm(CGameRect* rc)
{
	if(rc!=NULL)
	{
		rc->x-=rect.x;
		rc->y-=rect.y;
	}
}