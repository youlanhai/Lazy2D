#pragma once

#include "../G2DEngine/BitmapManager.h"

#include <vector>

/////////UI�ӿ�/////////////////
struct IUIControl
{
public:
	IUIControl(void);
	~IUIControl(void);
	//��ʼ������
	void init(char *szCaption,int id,int x=0,int y=0,int width=0,int height=0);
	//������Ϣ
	virtual LRESULT MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)=0;
	//��ʾ
	virtual void render(HDC hDC)=0;
	//���ñ���
	inline void setCaption(char *szCaption){ strcpy(m_szCaption,szCaption); }
	inline char* getCaption(void){ return m_szCaption; }
	//�����Ƿ����
	inline void setEnable(bool e){ m_bEnable=e; }
	inline bool getEnable(void){ return m_bEnable; }
	//��������
	inline void setFontName(char *szName){ strcpy(m_szFontName,szName);}
	inline char* getFontName(void){ return m_szFontName; }
	//����ǰ����ɫ
	inline void setForeColor(COLORREF cr){ m_crForeColor=cr; }	
	inline COLORREF getForeColor(void){ return m_crForeColor; }
	//���ñ���ɫ
	inline void setBackColor(COLORREF cr){ m_crBackColor=cr; };	//������ɫ
	inline COLORREF getBackColor(void){ return m_crBackColor; }
	//���ÿؼ�����
	inline void setRect(const CGameRect & rc){ m_rect=rc; }
	inline CGameRect & getRect(void){return m_rect; }
	//���ÿɼ�
	inline void setVisible(bool v){ m_bVisible=v; }
	inline bool getVisible(void){ return m_bVisible; }
	//����ID
	inline void setID(int id){ m_nID=id; }
	inline int  getID( void ){ return m_nID; }
public:
	char			m_szCaption[256];		//��ť����
	char			m_szFontName[256];		//��������
	COLORREF		m_crBackColor;			//ǰ��ɫ
	COLORREF		m_crForeColor;			//����ɫ
	CGameRect		m_rect;					//��������
	bool			m_bVisible;				//�����Ƿ�ɼ�
	bool			m_bEnable;				//�Ƿ����
	int				m_nID;					//���
};
//////////ͼƬ�ؼ�/////////////////
class CUIPicture : public IUIControl
{
public:
	CUIPicture();
	~CUIPicture();

	//bmp����Դ�������ⲿ�ͷ�!
	CUIPicture(HBITMAP bmp,int id,int x,int y,int width,int height);

	//��ʼ��
	bool init(HBITMAP bmp,int id,int x,int y,int width,int height);

	//������Ϣ
	LRESULT MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//��ʾ
	void render(HDC hDC);

	//���ñ���ͼƬ.bmp����Դ�������ⲿ�ͷ�!
	inline void setBackBmp(HBITMAP bmp){ m_backBmp=bmp; }
	inline HBITMAP getBackBmp( void ){ return m_backBmp; }

	//�Ƿ�͸������
	inline void setTransparent(bool t){ m_bTransparent=t; }
protected:
	HDC			m_hBackDC;
	bool		m_bTransparent;	//�Ƿ�͸������
	HBITMAP		m_backBmp;		//����ͼƬ
};
//////////�Ի�����/////////////////
/* ���Ҫ�ñ���ͼ���ƣ�ֻ���ȼ���һ��picture�ؼ�����.
*/
class CUIDialog : public IUIControl
{
public:
	typedef LRESULT (*PUIEventCall)(int id, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	CUIDialog();
	~CUIDialog();
	//������Ϣ
	LRESULT MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//��ʾ
	void render(HDC hDC);
	//��ӿؼ�����ÿؼ�
	void addControl(IUIControl *pCtrl);
	IUIControl* getControl( int id );

	void destroy(void);

	//����ת��
	void thisToParent(POINT *pt);
	void thisToParent(RECT *rc);
	void parentToThis(POINT *pt);
	void parentToThis(RECT *rc);

	//�Ƿ�͸������
	inline void setTransparent(bool t){ m_bTransparent=t; }

	//������Ϣ��Ӧ�ص�����
	inline void setEventCallBack(PUIEventCall p){ m_enventCallBack=p; }

	//���ñ������ɼ�
	inline void enableCaption(bool e){ m_bCaptionEnable=e; }
	inline bool getCaptionEnable(void){ return m_bCaptionEnable; }
protected:
	std::vector<IUIControl*>	m_vecCntrol;	//�ؼ��б�
	bool						m_bTransparent;	//�����Ƿ�͸��
	PUIEventCall				m_enventCallBack;//��Ϣ��Ӧ�ص�����
	bool						m_bCaptionEnable;//�Ƿ���ʾ������
};

////////////////��ť��//////////////////////////
class CUIButton : public IUIControl
{
public:
	CUIButton();
	~CUIButton();
	CUIButton(char *szText,int id,int x=0,int y=0,int width=0,int height=0);
	//������Ϣ
	LRESULT MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//��ʾ
	void render(HDC hDC);
protected:
	bool	m_bDown;
	bool	m_bOver;
};