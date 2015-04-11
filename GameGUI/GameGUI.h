#pragma once

#include "../G2DEngine/BitmapManager.h"

#include <vector>

/////////UI接口/////////////////
struct IUIControl
{
public:
	IUIControl(void);
	~IUIControl(void);
	//初始化窗体
	void init(char *szCaption,int id,int x=0,int y=0,int width=0,int height=0);
	//处理消息
	virtual LRESULT MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)=0;
	//显示
	virtual void render(HDC hDC)=0;
	//设置标题
	inline void setCaption(char *szCaption){ strcpy(m_szCaption,szCaption); }
	inline char* getCaption(void){ return m_szCaption; }
	//设置是否可用
	inline void setEnable(bool e){ m_bEnable=e; }
	inline bool getEnable(void){ return m_bEnable; }
	//设置字体
	inline void setFontName(char *szName){ strcpy(m_szFontName,szName);}
	inline char* getFontName(void){ return m_szFontName; }
	//设置前景颜色
	inline void setForeColor(COLORREF cr){ m_crForeColor=cr; }	
	inline COLORREF getForeColor(void){ return m_crForeColor; }
	//设置背景色
	inline void setBackColor(COLORREF cr){ m_crBackColor=cr; };	//设置颜色
	inline COLORREF getBackColor(void){ return m_crBackColor; }
	//设置控件区域
	inline void setRect(const CGameRect & rc){ m_rect=rc; }
	inline CGameRect & getRect(void){return m_rect; }
	//设置可见
	inline void setVisible(bool v){ m_bVisible=v; }
	inline bool getVisible(void){ return m_bVisible; }
	//设置ID
	inline void setID(int id){ m_nID=id; }
	inline int  getID( void ){ return m_nID; }
public:
	char			m_szCaption[256];		//按钮标题
	char			m_szFontName[256];		//字体名称
	COLORREF		m_crBackColor;			//前景色
	COLORREF		m_crForeColor;			//背景色
	CGameRect		m_rect;					//窗体区域
	bool			m_bVisible;				//窗体是否可见
	bool			m_bEnable;				//是否可用
	int				m_nID;					//编号
};
//////////图片控件/////////////////
class CUIPicture : public IUIControl
{
public:
	CUIPicture();
	~CUIPicture();

	//bmp的资源必须由外部释放!
	CUIPicture(HBITMAP bmp,int id,int x,int y,int width,int height);

	//初始化
	bool init(HBITMAP bmp,int id,int x,int y,int width,int height);

	//处理消息
	LRESULT MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//显示
	void render(HDC hDC);

	//设置背景图片.bmp的资源必须由外部释放!
	inline void setBackBmp(HBITMAP bmp){ m_backBmp=bmp; }
	inline HBITMAP getBackBmp( void ){ return m_backBmp; }

	//是否透明处理
	inline void setTransparent(bool t){ m_bTransparent=t; }
protected:
	HDC			m_hBackDC;
	bool		m_bTransparent;	//是否透明处理
	HBITMAP		m_backBmp;		//背景图片
};
//////////对话框类/////////////////
/* 如果要用背景图绘制，只需先加入一个picture控件即可.
*/
class CUIDialog : public IUIControl
{
public:
	typedef LRESULT (*PUIEventCall)(int id, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	CUIDialog();
	~CUIDialog();
	//处理消息
	LRESULT MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//显示
	void render(HDC hDC);
	//添加控件，获得控件
	void addControl(IUIControl *pCtrl);
	IUIControl* getControl( int id );

	void destroy(void);

	//坐标转换
	void thisToParent(POINT *pt);
	void thisToParent(RECT *rc);
	void parentToThis(POINT *pt);
	void parentToThis(RECT *rc);

	//是否透明处理
	inline void setTransparent(bool t){ m_bTransparent=t; }

	//设置消息响应回调函数
	inline void setEventCallBack(PUIEventCall p){ m_enventCallBack=p; }

	//设置标题栏可见
	inline void enableCaption(bool e){ m_bCaptionEnable=e; }
	inline bool getCaptionEnable(void){ return m_bCaptionEnable; }
protected:
	std::vector<IUIControl*>	m_vecCntrol;	//控件列表
	bool						m_bTransparent;	//窗体是否透明
	PUIEventCall				m_enventCallBack;//消息响应回调函数
	bool						m_bCaptionEnable;//是否显示标题栏
};

////////////////按钮类//////////////////////////
class CUIButton : public IUIControl
{
public:
	CUIButton();
	~CUIButton();
	CUIButton(char *szText,int id,int x=0,int y=0,int width=0,int height=0);
	//处理消息
	LRESULT MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//显示
	void render(HDC hDC);
protected:
	bool	m_bDown;
	bool	m_bOver;
};