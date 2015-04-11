//GameForm.h
#pragma once
#include "BitmapManager.h"
class CGameButton;

typedef enum
{
	FORM_CAPTION=0,	//有标题和关闭按钮的窗体
	FORM_POPUP,		//没有标题栏的窗体
	FORM_NONE		//不显示窗体
}FormStyle;//窗体风格

typedef enum
{
	FORM_HIDE=0,
	FORM_SHOW,
	FORM_DESTROY
}FormState;//窗体状态

class CGameForm
{
public:
	CGameForm();
	~CGameForm();
	void InitForm												//初始化窗体
	(
		std::string caption,FormStyle style,
		int x=0,int y=0,
		int width=0,int height=0,
		COLORREF foreColor=0,COLORREF backColor=0
	);
	void SetCaption(std::string caption);						//设置标题
	void SetColor(COLORREF foreColor=0,COLORREF backColor=0);	//设置颜色
	void SetRect(int x,int y,int w=0,int h=0);					//设置显示区域
	void SetRect(CGameRect* rc);								//设置显示区域
	void SetState(FormState state){fsState=state;}				//设置窗体状态
	void SetStyle(FormStyle style){fsStyle=style;}				//设置窗体风格
	void SetBitmap												//设置显示位图
	(
		CBitmapManager* bmpManager,
		int nStartPosition=0
	);
	void Show(HDC hDC);											//显示
	FormState GetState(){return fsState;}						//获得状态
	CGameRect* GetRect(){return &rect;}							//获得显示区域
	void ProcessMessage(HWND hWnd,void* pParam);				//处理消息
	void ClearButton();											//清空按钮
	void AddButton(CGameButton* button);						//添加按钮
	void ClientToForm(POINT* pt);
	void ClientToForm(RECT* rc);
	void ClientToForm(CGameRect* rc);
public:
	std::vector<CGameButton*> vecButtons;//按钮组
private:
	std::string	strCaption;				//按钮标题
	COLORREF	crBackColor;			//前景色
	COLORREF	crForeColor;			//背景色
	FormState	fsState;				//窗体状态
	FormStyle	fsStyle;				//窗体风格
	CGameRect	rect;					//按钮区域
	CGameRect	rcClose;				//关闭按钮
	CBitmapInformation	bmpBackGround;	//背景图片
};