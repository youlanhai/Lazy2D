//GameButton.h
#pragma once
#include "BitmapManager.h"

typedef enum
{
	BUTTON_NORMAL=0,	//一般状态
	BUTTON_ACTIVE,		//活动状态
	BUTTON_DOWN			//按下状态
}ButtonState;

typedef enum
{
	BUTTON_PUSH=0,	//下压式按钮，背景为公共3态位图
	BUTTON_DRAW,	//下压式按钮，背景为手动添加的3态位图
	BUTTON_STATIC,	//带背景，不处理消息
	BUTTON_TEXT,	//显示文本，无背景
}ButtonStyle;

typedef void (*GameButtonCall)(void* pParam);
/***********************************************
按钮类：
使用方法：
1.初始化公共组件InitializeCommen
2.初始化按钮InitButton
3.处理消息
a.给函数指针pButtonCall赋值，函数指针原型： void GameButtonCall(void* pParam);
构造函数指针方法：
1.调用全局函数：将全局函数的原型定义为void GameButtonCall(void* pParam);
2.调用类成员函数：给要处理消息的类增加相应成员函数，
再定义一个全局函数或静态成员方法，类型为void GameButtonCall(void* pParam);
在该函数中，将pParam强制转换为要处理的成员函数的类的指针，使用转换后的指针调用
成员函数。
b.处理鼠标消息ProcessMouseMessage，
注意给参数pParam传参时，一般为拥有按钮对象的对象的this指针。
***********************************************/
class CGameButton
{
public:
	CGameButton();
	~CGameButton();
	CGameButton(std::string caption,ButtonStyle style,GameButtonCall pFuction=NULL);
	void InitButton											//初始化按钮
	(
		std::string caption,ButtonStyle style,
		COLORREF foreColor=0,COLORREF backColor=0,
		GameButtonCall pFuction=NULL//回调函数指针
	);
	void SetCaption(std::string caption){strCaption=caption;}//设置标题
	void SetColor(COLORREF foreColor,COLORREF backColor);	//设置颜色
	void SetRect(int x,int y,int w=0,int h=0);				//设置显示区域
	void SetRect(CGameRect* rc);							//设置显示区域
	void SetButtonCall(GameButtonCall pFuction);			//设置回调函数
	void SetStyle(ButtonStyle style){bsStyle=style;}		//设置按钮风格
	void SetBitmap											//设置按钮显示位图
	(
		CBitmapManager* bmpManager,
		int nStartPosition=0
	);
	void Show(HDC hDC);										//显示按钮
	ButtonStyle GetStyle(){return bsStyle;}					//获得按钮风格
	ButtonState GetState(){return bsState;}					//获得按钮状态
	CGameRect*  GetRect(){return &rect;}					//获得显示区域
	ButtonState ProcessMouseMessage							//处理鼠标消息
	(
		POINT* ptMouse,	//鼠标坐标
		bool bDown,		//鼠标是否按下
		void* pParam	//附加参数，传递给回调函数的参数
	);
	static bool InitializeCommen							//初始化通用位图
	(
		HINSTANCE hInstance,
		std::string strBmpListName
	);
private:
	ButtonStyle	bsStyle;					//按钮风格
	ButtonState	bsState;					//按钮状态
	std::string strCaption;					//按钮标题
	COLORREF	crForeColor;				//标题颜色
	COLORREF	crBackColor;				//按钮为static时，若不添加位图，则应设置此项
	CGameRect	rect;						//按钮区域
	CBitmapInformation		bmpButton[3];	//按钮的三态位图
	GameButtonCall			pButtonCall;	//按钮要处理的消息函数指针
	static CBitmapManager	bmpCommenButton;//公共按钮三态位图
};

inline void CGameButton::SetColor(COLORREF foreColor,COLORREF backColor)
{
	crForeColor=foreColor;
	crBackColor=backColor;
	if(crForeColor==crBackColor)
	{
		crForeColor=RGB(255,0,0);
		crBackColor=RGB(20,20,20);
	}
}
inline void CGameButton::SetButtonCall(GameButtonCall pFuction)
{
	pButtonCall=pFuction;
}