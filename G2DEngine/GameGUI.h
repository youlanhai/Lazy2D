#pragma once

#include <vector>
#include <list>

/////////UI�ӿ�/////////////////
/* ���ã�IUIControlΪ���пؼ���Ļ��࣬ʵ�����пؼ�ͳһ����
** �÷����������������ʵ��MessageProc��render�������󷽷���
** ��������ʹ��ʱ������û����init�������г�ʼ��������д��ʼ��������
** ����MessageProc��������ؼ���Ϣ��
** ����render�������ƿؼ���
*/
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
	//���������
	inline char* getClassName(void){ return m_szClassName; }
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
	//����ID��ID�����ظ��������пؼ�id������Ϊ0��
	inline void setID(int id){ m_nID=id; }
	inline int  getID( void ){ return m_nID; }
	//�����ڴ����
	inline void setManaged(bool m){ m_bManaged=m; }
	inline bool getManaged( void ){ return m_bManaged; }
protected:
	char			m_szClassName[128];		//������
	char			m_szCaption[128];		//��ť����
	char			m_szFontName[128];		//��������
	COLORREF		m_crBackColor;			//ǰ��ɫ
	COLORREF		m_crForeColor;			//����ɫ
	CGameRect		m_rect;					//��������
	bool			m_bVisible;				//�����Ƿ�ɼ�
	bool			m_bEnable;				//�Ƿ����
	int				m_nID;					//��š�
	bool			m_bManaged;				//�Ƿ��Զ������ڴ�
};

class CUIDialog;
//////////�����///////////////////
/* �����԰����ӿؼ���ʵ������UI��Դ��ͳһ����
** ÿһ��Ӧ�ó������ֻ����һ��������
*/
class CUIPane : public IUIControl
{
public:
	typedef LRESULT (*PUIEventCall)(int id, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	CUIPane();
	~CUIPane();

	/*��ʼ��
	����С����򴰿ڴ�Сһ�£����������Ϊ(0,0)��(�벻Ҫ��������!)��
	���Ⱥ͸߶�����򴰿ڴ�Сһ�¡�
	*/
	void init(int id,int width,int height);

	//������Ϣ
	LRESULT MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//��ʾ
	void render(HDC hDC);

	//��ӿؼ�����ÿؼ�
	void addControl(IUIControl *pCtrl);
	IUIControl* getControl( int id );

	//������Դ
	void destroy(void);

	//������Ϣ��Ӧ�ص�����
	inline void setEventCallBack(PUIEventCall p){ m_enventCallBack=p; }
protected:
	std::list<IUIControl*>		m_lstControl;		//�ؼ��б�
	PUIEventCall				m_enventCallBack;	//��Ϣ��Ӧ�ص�����
};

//////////�Ի�����/////////////////
/* ���԰����ӿؼ�
** ���Ҫ�ñ���ͼ���ƣ���ʹ��addControl��������һ��picture�ؼ����ɣ�
** ע���ʵ�ʱҪ���ñ�������
*/
class CUIDialog : public IUIControl
{
public:
	//typedef LRESULT (*PUIEventCall)(int id, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	CUIDialog();
	~CUIDialog();

	void init(HWND hWnd,char *szCaption,int id,int x,int y,int width,int height);

	//������Ϣ
	LRESULT MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//��ʾ
	void render(HDC hDC);

	//��ӿؼ�����ÿؼ�
	void addControl(IUIControl *pCtrl);
	IUIControl* getControl( int id );

	//�ͷ���Դ
	void destroy(void);

	//�����Ƿ�ɼ�
	void setVisible(bool v);

	//����ת��
	void thisToParent(POINT *pt);
	void thisToParent(RECT *rc);
	void parentToThis(POINT *pt);
	void parentToThis(RECT *rc);

	//�Ƿ�͸������͸����������͸�������ӿؼ���͸����
//	inline void setTransparent(bool t){ m_bTransparent=t; }

	//������Ϣ��Ӧ�ص�����
//	inline void setEventCallBack(PUIEventCall p){ m_enventCallBack=p; }

	//���ñ������ɼ�
	inline void enableCaption(bool e){ m_bCaptionEnable=e; }
	inline bool getCaptionEnable(void){ return m_bCaptionEnable; }

	//����͸����ʾϵ��
	inline void setBlendAlpha(UINT alpha){ m_uBlendAlpha=alpha; }
	inline UINT getBlendAlpha(void){ return m_uBlendAlpha; }
protected:
	std::vector<IUIControl*>	m_vecCntrol;	//�ؼ��б�
	UINT						m_uBlendAlpha;//͸����ʾϵ��
	bool						m_bCaptionEnable;//�Ƿ���ʾ������
//	bool						m_bTransparent;	//�����Ƿ�͸��
//	PUIEventCall				m_enventCallBack;//��Ϣ��Ӧ�ص�����
	HWND						m_hWnd;
};

////////////////��ť��//////////////////////////
class CUIButton : public IUIControl
{
public:
	CUIButton();
	~CUIButton();
	CUIButton(char *szText,int id,int x,int y,int width,int height);
	//������Ϣ
	LRESULT MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//��ʾ
	void render(HDC hDC);
protected:
	bool	m_bDown;
	bool	m_bOver;
};


////////////�ı���ǩ��////////////////////////////
class CUIStatic : public IUIControl
{
public:
	CUIStatic();
	~CUIStatic();
	CUIStatic(char *szText,int id,int x,int y,int width,int height);
	//������Ϣ
	LRESULT MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//��ʾ
	void render(HDC hDC);

	inline void setTransparent(bool t){ m_bTransparent=t; }

    inline void setFontSize(int size){ m_nFontHeight = size; }
    inline int getFontSize(void){ return m_nFontHeight; }

	inline void setText(const char *szText){ m_strText=szText; }
	inline const char* getText(void){ return m_strText.c_str(); }

	inline void setDrawStyle(DWORD style){ m_dwDrawStyle=style; }
	inline DWORD getDrawStyle(void){ return m_dwDrawStyle; }
protected:
	std::string		m_strText;
	bool			m_bTransparent;	//������͸��
	DWORD			m_dwDrawStyle;	//������ʽ
    int             m_nFontHeight;
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

	//���ñ���ͼƬ�� (bmp����Դ�������ⲿ�ͷ�!)
	inline void setBackBmp(HBITMAP bmp){ m_backBmp=bmp; }
	inline HBITMAP getBackBmp( void ){ return m_backBmp; }

	//�Ƿ�͸������
	inline void setTransparent(bool t){ m_bTransparent=t; }

	//�Ƿ����촦��
	inline void setStretch( bool s){ m_bStretch=s; }
protected:
	HDC			m_hBackDC;		//����DC
	bool		m_bTransparent;	//�Ƿ�͸������
	bool		m_bStretch;		//�Ƿ����촦��
	HBITMAP		m_backBmp;		//����ͼƬ
};
