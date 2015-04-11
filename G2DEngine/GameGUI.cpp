#include "Commen.h"
#include "GameGUI.h"

#include <Windowsx.h>

/////////////////////struct IUIControl start//////////////////
IUIControl::IUIControl(void)
{
	strcpy(m_szClassName,"IUIControl");
	setCaption("UICotrol");
	setFontName("����");
	m_nID=0;
	m_crBackColor=RGB(255,255,255);		//����ɫ
	m_crForeColor=0;			//ǰ��ɫ
	m_bVisible=true;			//�����Ƿ�ɼ�
	m_bEnable=true;
	m_bManaged=true;
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

/////////////////////struct IUIControl end/////////////////////


////////////////////class CUIPane start////////////////////////

CUIPane::CUIPane()
{
	strcpy(m_szClassName,"CUIPane");
	m_enventCallBack=NULL;
}
CUIPane::~CUIPane()
{
	destroy();
}
void CUIPane::init(int id,int width,int height)
{
	IUIControl::init("pane",id,0,0,width,height);
}
//������Ϣ
LRESULT CUIPane::MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(!m_bVisible || !m_bEnable)
	{
		return 0;
	}
	if (msg == WM_UI_REORDER)//�ӿؼ��������ӿؼ�����WM_UI_REORDER��Ϣʱ��ִ�д˴���
	{
		IUIControl *pCtrl=getControl(wParam);
		if (pCtrl == NULL)
		{
			return 0;
		}
		m_lstControl.remove(pCtrl);
		if (lParam == 1)//��ʾ
		{
			m_lstControl.push_front(pCtrl);
		}
		else//����
		{
			m_lstControl.push_back(pCtrl);
		}
		return 1;
	}
	//�����ӿؼ���Ϣ
	LRESULT res=0;
	std::list<IUIControl*>::iterator it,itEnd;
	it=m_lstControl.begin();
	itEnd=m_lstControl.end();
	for ( ; it!=itEnd; ++it)
	{
		res=(*it)->MessageProc(hWnd,msg,wParam,lParam);
		if(res != 0)
		{
			if (m_enventCallBack!=NULL)
			{
				m_enventCallBack(res,msg,wParam,lParam);
			}
			break;
		}
	}
	//�ӿؼ��������򡣵���������е�ĳһ�������ʱ����ִ�д˴���
	if (res!=0)
	{ 
		IUIControl *pCtrl=getControl(res);
		//����Dialogִ��
		if (pCtrl != NULL && strcmp(pCtrl->getClassName(),"CUIDialog")==0)
		{
			m_lstControl.erase(it);
			m_lstControl.push_front(pCtrl);
		}
	}
	return res;
}

//��ʾ
void CUIPane::render(HDC hDC)
{
	if(!m_bVisible)
	{
		return ;
	}
	//�����ӿؼ�
	std::list<IUIControl*>::reverse_iterator it,itEnd;
	it=m_lstControl.rbegin();
	itEnd=m_lstControl.rend();
	for ( ; it!=itEnd; ++it)
	{
		(*it)->render(hDC);
	}

}
//��ӿؼ�����ÿؼ�
void CUIPane::addControl(IUIControl *pCtrl)
{
	if (pCtrl != NULL)
	{
		m_lstControl.push_back(pCtrl);
	}
}
//�����ӿؼ������ܲ鵽�ӿؼ����ӿؼ���
IUIControl* CUIPane::getControl( int id )
{
	std::list<IUIControl*>::iterator it,itEnd;
	it=m_lstControl.begin();
	itEnd=m_lstControl.end();
	for ( ; it!=itEnd; ++it)
	{
		if ((*it)->getID() == id)
		{
			return (*it);
		}
	}
	return NULL;
}

void CUIPane::destroy(void)
{
	std::list<IUIControl*>::iterator it,itEnd;
	it=m_lstControl.begin();
	itEnd=m_lstControl.end();
	for ( ; it!=itEnd; ++it)
	{
		if ((*it)->getManaged())
		{
			delete (*it);
		}
	}
	m_lstControl.clear();
}

///////////class CUIPane end///////////////////

///////////class CUIDialog start///////////////
CUIDialog::CUIDialog()
{
	strcpy(m_szClassName,"CUIDialog");
	//m_bTransparent=false;
	//m_enventCallBack=NULL;
	m_bCaptionEnable=false;		//�Ƿ���ʾ������
	m_uBlendAlpha=255;
	m_hWnd=NULL;
}

CUIDialog::~CUIDialog()
{
	destroy();
}


void CUIDialog::init(HWND hWnd,char *szCaption,int id,int x,int y,int width,int height)
{
	m_hWnd=hWnd;
	IUIControl::init(szCaption,id,x,y,width,height);
}

//������Ϣ
LRESULT CUIDialog::MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(!m_bVisible || !m_bEnable)
	{
		return 0;
	}
	//����ת��
	POINT pt;
	pt.x=GET_X_LPARAM(lParam);
	pt.y=GET_Y_LPARAM(lParam);
	parentToThis(&pt);
	LPARAM nLParam=MAKELPARAM(pt.x,pt.y);
	//�����ӿؼ���Ϣ
	LRESULT res=0;
	std::vector<IUIControl*>::iterator it,itEnd;
	it=m_vecCntrol.begin();
	itEnd=m_vecCntrol.end();
	for ( ; it!=itEnd; ++it)
	{
		res=(*it)->MessageProc(hWnd,msg,wParam,nLParam);
		if(res != 0)//�ӿؼ��ػ���Ϣ
		{
			return (*it)->getID();
		}
	}
	//�ػ������Ϣ
	if (msg == WM_LBUTTONDOWN ||msg == WM_LBUTTONUP)
	{
		thisToParent(&pt);
		if (m_rect.IsIn(&pt))
		{
			return m_nID;
		}
	}
	return 0;
}

//��ʾ
void CUIDialog::render(HDC hDC)
{
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
	//if (!m_bTransparent)
	{//��ʾ����
		//�û�ˢ���Ʊ���
		HBRUSH hBrush=CreateSolidBrush(m_crBackColor);
		HBRUSH oldBrush=(HBRUSH)SelectObject(hBackDC,hBrush);
		HPEN hPen=CreatePen(PS_SOLID,1,m_crForeColor);
		HPEN oldPen=(HPEN)SelectObject(hBackDC,hPen);

		Rectangle(hBackDC,0,0,m_rect.w,m_rect.h);


		if(m_bCaptionEnable)	
		{//���Ʊ���
			Rectangle(hBackDC,0,0,m_rect.w,30);//���Ʊ�����
			CGameRect rc=CGameRect(0,0,m_rect.w,30);
			DrawText(hBackDC,m_szCaption,strlen(m_szCaption),
				rc.ToRECT(),DT_SINGLELINE|DT_VCENTER|DT_CENTER);
		}
		SelectObject(hBackDC,oldPen);
		SelectObject(hBackDC,oldBrush);
		DeleteObject(hBrush);
		DeleteObject(hPen);
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
// 	if (m_bTransparent)
// 	{
// 		TransparentBlt(hDC,m_rect.x,m_rect.y,m_rect.w,m_rect.h,
// 			hBackDC,0,0,m_rect.w,m_rect.h,crTrans);
// 	}
// 	else
	{
		//��͸����ʾ�Ի�����
		BLENDFUNCTION bf;
		bf.AlphaFormat=0;
		bf.BlendFlags=0;
		bf.BlendOp=AC_SRC_OVER;
		bf.SourceConstantAlpha=m_uBlendAlpha;//͸����0-255
		
		AlphaBlend(hDC,m_rect.x,m_rect.y,m_rect.w,m_rect.h,
			hBackDC,0,0,m_rect.w,m_rect.h,bf);
		//BitBlt(hDC,m_rect.x,m_rect.y,m_rect.w,m_rect.h,hBackDC,0,0,SRCCOPY);
	}

	SelectObject(hBackDC,oldFont);
	DeleteObject(font);

	DeleteObject(hBackBmp);
	DeleteDC(hBackDC);
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
		if ((*it)->getID() == id)
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
        if ((*it)->getManaged())
        {
            delete (*it);
        }
	}
	m_vecCntrol.clear();
}

//�����Ƿ�ɼ�
void CUIDialog::setVisible(bool v)
{
	if (v)
	{
		SendMessage(m_hWnd,WM_UI_REORDER,m_nID,1);
	}
	else
	{
		SendMessage(m_hWnd,WM_UI_REORDER,m_nID,0);
	}
	m_bVisible=v;
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
	strcpy(m_szClassName,"CUIButton");
	m_crBackColor=RGB(0,0,255);
	m_crForeColor=RGB(255,0,255);
}
CUIButton::~CUIButton()
{

}
CUIButton::CUIButton(char *szText,int id,int x,int y,int width,int height)
{
	strcpy(m_szClassName,"CUIButton");
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
		crDrawColor=getBlendColor(m_crBackColor,0.6);
	}
	HBRUSH brush=CreateSolidBrush(crDrawColor);
	HPEN pen=CreatePen(PS_SOLID,2,crEige);
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

///////////class CUIButton end///////////////

///////////class CUIStatic start///////////////
CUIStatic::CUIStatic()
{
	strcpy(m_szClassName,"CUIStatic");
	m_bTransparent=true;
	m_dwDrawStyle=DT_CENTER|DT_TOP |DT_WORDBREAK;
	m_strText="�����setText����Ҫ��ʾ���ı�";
    m_nFontHeight = 15;
}
CUIStatic::~CUIStatic()
{

}
CUIStatic::CUIStatic(char *szText,int id,int x,int y,int width,int height)
{
	strcpy(m_szClassName,"CUIStatic");
	IUIControl::init("static",id,x,y,width,height);
	m_strText=szText;
	m_dwDrawStyle=DT_CENTER|DT_TOP |DT_WORDBREAK;
	m_bTransparent=true;
    m_nFontHeight = 15;
}
//������Ϣ
LRESULT CUIStatic::MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}
//��ʾ
void CUIStatic::render(HDC hDC)
{
	if(!m_bVisible)
	{
		return;
	}
	if(!m_bTransparent)
	{
		HBRUSH brush=CreateSolidBrush(m_crBackColor);
		HPEN pen=CreatePen(PS_SOLID,2,m_crBackColor);
		HBRUSH oldBrush=(HBRUSH)SelectObject(hDC,brush);
		HPEN olePen=(HPEN)SelectObject(hDC,pen);

		Rectangle(hDC,m_rect.x,m_rect.y,m_rect.x+m_rect.w,m_rect.y+m_rect.h);

		SelectObject(hDC,oldBrush);
		SelectObject(hDC,olePen);
		DeleteObject(brush);
		DeleteObject(pen);
	}

	{//���Ʊ���
        COLORREF crOldColor = SetTextColor(hDC,m_crForeColor);
		HFONT font=createFontEx(m_nFontHeight,m_szFontName);
		HFONT oldFont=(HFONT)SelectObject(hDC,font);
		DrawText(hDC,m_strText.c_str(),m_strText.size(),
			m_rect.ToRECT(),m_dwDrawStyle);
		SelectObject(hDC,oldFont);
		DeleteObject(font);
        SetTextColor(hDC,crOldColor);
	}
}
///////////class CUIStatic end///////////////


//////////class CUIPicture start/////////////////

CUIPicture::CUIPicture()
{
	strcpy(m_szClassName,"CUIPicture");
	m_bStretch=false;
	m_bTransparent=false;
	m_backBmp=NULL;	//����ͼƬ
	m_hBackDC=NULL;
}
CUIPicture::CUIPicture(HBITMAP bmp,int id,int x,int y,int width,int height)
{
	strcpy(m_szClassName,"CUIPicture");
	m_bStretch=false;
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
	BITMAP bm;
	GetObject(m_backBmp,sizeof(bm),&bm);
	if (m_bTransparent)
	{
		int dw=min(m_rect.w,bm.bmWidth);
		int dh=min(m_rect.h,bm.bmHeight);
		if (m_bStretch)
		{
			dw=m_rect.w;
			dh=m_rect.h;
		}
		int sw=min(m_rect.w,bm.bmWidth);
		int sh=min(m_rect.h,bm.bmHeight);
		if (m_bStretch)
		{
			sw=bm.bmWidth;
			sh=bm.bmHeight;
		}
		TransparentBlt(hDC,m_rect.x,m_rect.y,dw,dh,
			m_hBackDC,0,0,sw,sh,GetPixel(m_hBackDC,0,0));
	}
	else if(m_bStretch)
	{
		StretchBlt(hDC,m_rect.x,m_rect.y,m_rect.w,m_rect.h,
			m_hBackDC,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
	}
	else
	{
		BitBlt(hDC,m_rect.x,m_rect.y,m_rect.w,m_rect.h,
			m_hBackDC,0,0,SRCCOPY);
	}
}

//////////class CUIPicture end/////////////////
