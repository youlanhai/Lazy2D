#include "../G2DEngine/Commen.h"
#include "GameGUI.h"
#include <Windowsx.h>

/////////////////////struct IUIControl start////////////
IUIControl::IUIControl(void)
{
	setCaption("UICotrol");
	setFontName("����");
	m_nID=0;
	m_crBackColor=RGB(255,255,255);		//����ɫ
	m_crForeColor=0;			//ǰ��ɫ
	m_bVisible=true;			//�����Ƿ�ɼ�
	m_bEnable=true;
}
IUIControl::~IUIControl(void)
{
}
//��ʼ������
void IUIControl::init(char *szCaption,int id,int x,int y,int width,int height)
{
	m_nID=id;
	setCaption(szCaption);
	m_rect=CGameRect(x,y,width,height);
}

/////////////////////struct IUIControl end////////////

//////////class CUIPicture start/////////////////

CUIPicture::CUIPicture()
{
	m_bTransparent=false;
	m_backBmp=0;	//����ͼƬ
	m_hBackDC=0;
}
CUIPicture::CUIPicture(HBITMAP bmp,int id,int x,int y,int width,int height)
{
	m_bTransparent=false;
	init(bmp,id,x,y,width,height);
}
CUIPicture::~CUIPicture()
{
	if (m_hBackDC != NULL)
	{
		DeleteDC(m_hBackDC);
		m_hBackDC=NULL;
	}
}
bool CUIPicture::init(HBITMAP bmp,int id,int x,int y,int width,int height)
{
	IUIControl::init("picture",id,x,y,width,height);
	m_backBmp=bmp;
	m_hBackDC=CreateCompatibleDC(0);
	return true;
}
//������Ϣ
LRESULT CUIPicture::MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}
//��ʾ
void CUIPicture::render(HDC hDC)
{
	if (NULL==m_backBmp || !m_bVisible)
	{
		return ;
	}
	SelectObject(m_hBackDC,m_backBmp);
	if (m_bTransparent)
	{
		BITMAP bm;
		GetObject(m_backBmp,sizeof(bm),&bm);
		TransparentBlt(hDC,m_rect.x,m_rect.y,m_rect.w,m_rect.h,
			m_hBackDC,0,0,bm.bmWidth,bm.bmHeight,GetPixel(m_hBackDC,0,0));
	}
	else
	{
		BitBlt(hDC,m_rect.x,m_rect.y,m_rect.w,m_rect.h,m_hBackDC,0,0,SRCCOPY);
	}
}
	
//////////class CUIPicture end/////////////////

///////////class CUIDialog start///////////////
CUIDialog::CUIDialog()
{
	m_bTransparent=false;
	m_enventCallBack=NULL;
	m_bCaptionEnable=false;		//�Ƿ���ʾ������
}

CUIDialog::~CUIDialog()
{
	destroy();
}

//������Ϣ
LRESULT CUIDialog::MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	if(!m_bVisible || !m_bEnable)
	{
		return 0;
	}

	POINT pt;
	pt.x=GET_X_LPARAM(lParam);
	pt.y=GET_Y_LPARAM(lParam);
	parentToThis(&pt);
	LPARAM nLParam=MAKELPARAM(pt.x,pt.y);
	LRESULT res=0;
	std::vector<IUIControl*>::iterator it,itEnd;
	it=m_vecCntrol.begin();
	itEnd=m_vecCntrol.end();
	for ( ; it!=itEnd; ++it)
	{
		res=(*it)->MessageProc(hWnd,msg,wParam,nLParam);
		if(res != 0)
		{
			break;
		}
	}
	if (res!=0 && m_enventCallBack!=NULL)
	{
		m_enventCallBack((*it)->m_nID,msg,wParam,nLParam);
	}
	return res;
}

//��ʾ
void CUIDialog::render(HDC hDC)
{
#if 1
	if(!m_bVisible)
	{
		return ;
	}

	HDC hBackDC=CreateCompatibleDC(0);
	//��������λͼ
	HBITMAP hBackBmp=CreateCompatibleBitmap(hDC,m_rect.w,m_rect.h);
	SelectObject(hBackDC,hBackBmp);

	HFONT font=createFontEx(15,m_szFontName);
	HFONT oldFont=(HFONT)SelectObject(hBackDC,font);
	SetTextColor(hBackDC,m_crForeColor);
	SetBkMode(hBackDC,TRANSPARENT);

	COLORREF crTrans=GetPixel(hBackDC,0,0);
	if (!m_bTransparent)
	{//��ʾ����
		//�û�ˢ���Ʊ���
		HBRUSH hBrush=CreateSolidBrush(m_crBackColor);
		HBRUSH oldBrush=(HBRUSH)SelectObject(hBackDC,hBrush);
		HPEN hPen=CreatePen(PS_SOLID,1,m_crForeColor);
		HPEN oldPen=(HPEN)SelectObject(hBackDC,hPen);

		Rectangle(hBackDC,0,0,m_rect.w,m_rect.h);

		SelectObject(hBackDC,oldPen);
		SelectObject(hBackDC,oldBrush);
		DeleteObject(hBrush);
		DeleteObject(hPen);

		if(m_bCaptionEnable)	
		{//���Ʊ���
			CGameRect rc=CGameRect(0,0,m_rect.w,30);
			DrawText(hBackDC,m_szCaption,strlen(m_szCaption),
				rc.ToRECT(),DT_SINGLELINE|DT_VCENTER|DT_CENTER);
		}
	}
	//�����ӿؼ�
	{
		std::vector<IUIControl*>::iterator it,itEnd;
		it=m_vecCntrol.begin();
		itEnd=m_vecCntrol.end();
		for ( ; it!=itEnd; ++it)
		{
			(*it)->render(hBackDC);
		}
	}
	if (m_bTransparent)
	{
		TransparentBlt(hDC,m_rect.x,m_rect.y,m_rect.w,m_rect.h,
			hBackDC,0,0,m_rect.w,m_rect.h,crTrans);
	}
	else
	{
		BitBlt(hDC,m_rect.x,m_rect.y,m_rect.w,m_rect.h,hBackDC,0,0,SRCCOPY);
	}

	SelectObject(hBackDC,oldFont);
	DeleteObject(font);

	DeleteObject(hBackBmp);
	DeleteDC(hBackDC);

#endif
}
//��ӿؼ�����ÿؼ�
void CUIDialog::addControl(IUIControl *pCtrl)
{
	if (pCtrl != NULL)
	{
		m_vecCntrol.push_back(pCtrl);
	}
}
IUIControl* CUIDialog::getControl( int id )
{
	std::vector<IUIControl*>::iterator it,itEnd;
	it=m_vecCntrol.begin();
	itEnd=m_vecCntrol.end();
	for ( ; it!=itEnd; ++it)
	{
		if ((*it)->m_nID == id)
		{
			return (*it);
		}
	}
	return NULL;
}

void CUIDialog::destroy(void)
{
	std::vector<IUIControl*>::iterator it,itEnd;
	it=m_vecCntrol.begin();
	itEnd=m_vecCntrol.end();
	for ( ; it!=itEnd; ++it)
	{
		delete (*it);
	}
	m_vecCntrol.clear();
}
//����ת��
void CUIDialog::thisToParent(POINT *pt)
{
	pt->x += m_rect.x;
	pt->y += m_rect.y;
}
void CUIDialog::thisToParent(RECT *rc)
{
	rc->left += m_rect.x;
	rc->top += m_rect.y;
	rc->right += m_rect.x;
	rc->bottom += m_rect.y;
}
void CUIDialog::parentToThis(POINT *pt)
{
	pt->x -= m_rect.x;
	pt->y -= m_rect.y;
}
void CUIDialog::parentToThis(RECT *rc)
{
	rc->left -= m_rect.x;
	rc->top -= m_rect.y;
	rc->right -= m_rect.x;
	rc->bottom -= m_rect.y;
}
///////////class CUIDialog end///////////////


///////////class CUIButton start///////////////
CUIButton::CUIButton()
{
	m_crBackColor=RGB(0,0,255);
	m_crForeColor=RGB(255,0,255);
}
CUIButton::~CUIButton()
{

}
CUIButton::CUIButton(char *szText,int id,int x,int y,int width,int height)
{
	IUIControl::init(szText,id,x,y,width,height);
	m_crBackColor=RGB(0,0,255);
	m_crForeColor=RGB(255,0,255);
}
//������Ϣ
LRESULT CUIButton::MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_bDown=false;
	m_bOver=false;
	if(!m_bVisible || !m_bEnable)
	{
		return 0;
	}
	POINT pt;
	pt.x=GET_X_LPARAM(lParam);
	pt.y=GET_Y_LPARAM(lParam); 
	if (KEYDOWN(VK_LBUTTON))
	{
		if(m_rect.IsIn(&pt))
		{
			m_bDown=true;
		}
	}
	if (msg == WM_LBUTTONUP)
	{
		if(m_rect.IsIn(&pt))
		{
			m_bDown=false;
			return 1;
		}
	}
	else if (msg ==WM_MOUSEMOVE)
	{
		if(m_rect.IsIn(&pt))
		{
			m_bOver=true;
		}
		else
		{
			m_bOver=false;
		}
	}
	return 0;
}
//��ʾ
void CUIButton::render(HDC hDC)
{
	if(!m_bVisible)
	{
		return;
	}
//	HDC hMemDC=CreateCompatibleDC(0);
	COLORREF crEige=getBlendColor(m_crBackColor,0.1);//��Ե��ɫ
	COLORREF crDrawColor;//������ɫ
	if(m_bDown)
	{
		crDrawColor=getBlendColor(m_crBackColor,0.2);
	}
	else if (m_bOver)
	{
		crDrawColor=m_crBackColor;
	}
	else
	{
		crDrawColor=getBlendColor(m_crBackColor,0.8);
	}
	HBRUSH brush=CreateSolidBrush(crDrawColor);
	HPEN pen=CreatePen(PS_SOLID,1,crEige);
	HBRUSH oldBrush=(HBRUSH)SelectObject(hDC,brush);
	HPEN olePen=(HPEN)SelectObject(hDC,pen);
	//ֱ�ӻ��Ƶ�hdc�ϡ�
	Rectangle(hDC,m_rect.x,m_rect.y,m_rect.x+m_rect.w,m_rect.y+m_rect.h);
	SelectObject(hDC,oldBrush);
	SelectObject(hDC,olePen);
	DeleteObject(brush);
	DeleteObject(pen);
	
	{//���Ʊ���
		DrawText(hDC,m_szCaption,strlen(m_szCaption),
			m_rect.ToRECT(),DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	}
//	DeleteDC(hMemDC);
}

///////////class CUIButton start///////////////