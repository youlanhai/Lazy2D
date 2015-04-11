#pragma once

#include <vector>
#include <list>

/////////UI接口/////////////////
/* 作用：IUIControl为所有控件类的基类，实现所有控件统一管理
** 用法：所有派生类必须实现MessageProc、render两个抽象方法，
** 所有类在使用时，需调用基类的init方法进行初始化或者重写初始化方法，
** 调用MessageProc方法处理控件消息，
** 调用render方法绘制控件。
*/
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
	//获得类名称
	inline char* getClassName(void){ return m_szClassName; }
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
	//设置ID。ID可以重复，但所有控件id都不能为0！
	inline void setID(int id){ m_nID=id; }
	inline int  getID( void ){ return m_nID; }
	//设置内存管理
	inline void setManaged(bool m){ m_bManaged=m; }
	inline bool getManaged( void ){ return m_bManaged; }
protected:
	char			m_szClassName[128];		//类名称
	char			m_szCaption[128];		//按钮标题
	char			m_szFontName[128];		//字体名称
	COLORREF		m_crBackColor;			//前景色
	COLORREF		m_crForeColor;			//背景色
	CGameRect		m_rect;					//窗体区域
	bool			m_bVisible;				//窗体是否可见
	bool			m_bEnable;				//是否可用
	int				m_nID;					//编号。
	bool			m_bManaged;				//是否自动管理内存
};

class CUIDialog;
//////////面板类///////////////////
/* 面板可以包含子控件，实现所有UI资源的统一管理，
** 每一个应用程序最好只包含一个面板对象。
*/
class CUIPane : public IUIControl
{
public:
	typedef LRESULT (*PUIEventCall)(int id, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	CUIPane();
	~CUIPane();

	/*初始化
	面板大小与程序窗口大小一致，所以坐标点为(0,0)点(请不要更改坐标!)，
	其宽度和高度与程序窗口大小一致。
	*/
	void init(int id,int width,int height);

	//处理消息
	LRESULT MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//显示
	void render(HDC hDC);

	//添加控件，获得控件
	void addControl(IUIControl *pCtrl);
	IUIControl* getControl( int id );

	//销毁资源
	void destroy(void);

	//设置消息响应回调函数
	inline void setEventCallBack(PUIEventCall p){ m_enventCallBack=p; }
protected:
	std::list<IUIControl*>		m_lstControl;		//控件列表
	PUIEventCall				m_enventCallBack;	//消息响应回调函数
};

//////////对话框类/////////////////
/* 可以包含子控件
** 如果要用背景图绘制，可使用addControl方法加入一个picture控件即可，
** 注意适当时要禁用标题栏。
*/
class CUIDialog : public IUIControl
{
public:
	//typedef LRESULT (*PUIEventCall)(int id, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	CUIDialog();
	~CUIDialog();

	void init(HWND hWnd,char *szCaption,int id,int x,int y,int width,int height);

	//处理消息
	LRESULT MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//显示
	void render(HDC hDC);

	//添加控件，获得控件
	void addControl(IUIControl *pCtrl);
	IUIControl* getControl( int id );

	//释放资源
	void destroy(void);

	//设置是否可见
	void setVisible(bool v);

	//坐标转换
	void thisToParent(POINT *pt);
	void thisToParent(RECT *rc);
	void parentToThis(POINT *pt);
	void parentToThis(RECT *rc);

	//是否透明处理。透明：仅窗体透明，其子控件不透明。
//	inline void setTransparent(bool t){ m_bTransparent=t; }

	//设置消息响应回调函数
//	inline void setEventCallBack(PUIEventCall p){ m_enventCallBack=p; }

	//设置标题栏可见
	inline void enableCaption(bool e){ m_bCaptionEnable=e; }
	inline bool getCaptionEnable(void){ return m_bCaptionEnable; }

	//设置透明显示系数
	inline void setBlendAlpha(UINT alpha){ m_uBlendAlpha=alpha; }
	inline UINT getBlendAlpha(void){ return m_uBlendAlpha; }
protected:
	std::vector<IUIControl*>	m_vecCntrol;	//控件列表
	UINT						m_uBlendAlpha;//透明显示系数
	bool						m_bCaptionEnable;//是否显示标题栏
//	bool						m_bTransparent;	//窗体是否透明
//	PUIEventCall				m_enventCallBack;//消息响应回调函数
	HWND						m_hWnd;
};

////////////////按钮类//////////////////////////
class CUIButton : public IUIControl
{
public:
	CUIButton();
	~CUIButton();
	CUIButton(char *szText,int id,int x,int y,int width,int height);
	//处理消息
	LRESULT MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//显示
	void render(HDC hDC);
protected:
	bool	m_bDown;
	bool	m_bOver;
};


////////////文本标签类////////////////////////////
class CUIStatic : public IUIControl
{
public:
	CUIStatic();
	~CUIStatic();
	CUIStatic(char *szText,int id,int x,int y,int width,int height);
	//处理消息
	LRESULT MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//显示
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
	bool			m_bTransparent;	//背景否透明
	DWORD			m_dwDrawStyle;	//绘制样式
    int             m_nFontHeight;
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

	//设置背景图片。 (bmp的资源必须由外部释放!)
	inline void setBackBmp(HBITMAP bmp){ m_backBmp=bmp; }
	inline HBITMAP getBackBmp( void ){ return m_backBmp; }

	//是否透明处理
	inline void setTransparent(bool t){ m_bTransparent=t; }

	//是否拉伸处理
	inline void setStretch( bool s){ m_bStretch=s; }
protected:
	HDC			m_hBackDC;		//掩码DC
	bool		m_bTransparent;	//是否透明背景
	bool		m_bStretch;		//是否拉伸处理
	HBITMAP		m_backBmp;		//背景图片
};
