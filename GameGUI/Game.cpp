//Game.cpp
#include "../G2DEngine/Commen.h"
#include "../G2DEngine/FPS.h"
#include "../G2DEngine/Keyboard.h"
#include "Game.h"

enum GUIID
{
	GUI_BTN_TEST=100,
};

CGame	g_game;//游戏类对象
LRESULT OnEvent(int id, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(id)
	{
	case GUI_BTN_TEST:
		MessageBox(NULL,"OK","",MB_OK);
		break;
	default:
		break;
	}
	return 0;
}
///////////////////主函数///////////////////////////////////////
int WINAPI WinMain( HINSTANCE hInstance,HINSTANCE,LPSTR,int)
{
	g_log.init("log.txt");
	if(g_game.init(hInstance,
		"GameEngine          http://hi.baidu.com/游蓝海",
		640,480))//初始化游戏类
	{
		g_game.mainLoop();//游戏循环
	}
	Sleep(100);
	return 0;
}
////////////////class CGame start//////////////////////////////

//注册窗口类
void CGame::registerClass()
{
	WNDCLASSEX wc;
	memset(&wc,0,sizeof(wc));
	wc.cbSize=sizeof(wc);
	wc.lpfnWndProc=(WNDPROC)windowProc;
	wc.hInstance=m_hInstance;
	wc.lpszClassName="GameEngin";
	wc.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hIcon=LoadIcon(NULL,IDI_INFORMATION);
	wc.hIconSm=wc.hIcon;
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);
	wc.style=CS_CLASSDC;
	RegisterClassEx(&wc);
}
//窗口过程
LRESULT CALLBACK CGame::windowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	g_game.m_pane.MessageProc(hWnd,msg,wParam,lParam);
	switch(msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;
	case WM_MOUSEWHEEL:
		{
			int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			int t=g_game.m_dialog.getBlendAlpha()+(zDelta>0?10:-10);
			if (t>255)
			{
				t=255;
			}
			else if (t<0)
			{
				t=0;
			}
			g_game.m_dialog.setBlendAlpha(t);
			break;
		}
	default:
		return DefWindowProc(hWnd,msg,wParam,lParam);
	}
	return 0;
}

CGame::CGame()
{
	m_pFPS=NULL;
	m_pKeybord=NULL;
	m_backFont=NULL;
}
CGame::~CGame()
{
}

void CGame::destroy()			//销毁资源
{
	SafeDelete(m_pFPS);
	SafeDelete(m_pKeybord);
	if (m_backFont != NULL)
	{
		DeleteObject(m_backFont);
	}
	
}
bool CGame::initGame()		//初始化游戏
{
	gameState=GAME_INIT;
	RECT rc;
	GetClientRect(m_hWnd,&rc);
	m_clientRect=CGameRect(rc.left,rc.top,rc.right,rc.bottom);
	
	m_backFont=createFontEx(15,"宋体");
	if (m_backFont == NULL)
	{
		g_log.write("创建字体失败！");
	}
	else SelectObject(m_hBackDC,m_backFont);

	m_pFPS=new CFPS();

	m_pKeybord=new CKeyboard();

	m_bmpManager.CreateBitmapFromFileList(m_hInstance,"bmp.dat");

	return initUI();
}
bool CGame::initUI()
{
	m_pane.init(1,m_nWidth,m_nHeight);
	m_pane.setEventCallBack(OnEvent);

	m_dialog.init(m_hWnd,"dialog",2,0,0,m_nWidth,m_nHeight);
	CUIPicture *pPic=new CUIPicture(m_bmpManager.GetAt(0)->hBitmap,0,0,0,m_nWidth,m_nHeight);
	pPic->setTransparent(true);
	pPic->setStretch(true);
	m_dialog.addControl(pPic);
	m_dialog.addControl(new CUIButton("这是按钮",GUI_BTN_TEST,0,20,100,30));
	m_dialog.addControl(new CUIStatic("这是静态文本标签，使用该控件可以方便的向对话框控件上显示文本。\
									  \r\n此处有换行。",0,0,60,200,100));
	m_dialog.enableCaption(true);
	m_dialog.setManaged(false);//注意静态变量，需设置资源非自动管理
	m_pane.addControl(&m_dialog);

	return true;
}
void CGame::update()			//更新
{
	m_pFPS->updateFPS();
	int curTime=timeGetTime();
	static int oldUpdateTime=0;
	if(curTime - oldUpdateTime <50)
	{
		return;
	}
	oldUpdateTime = curTime ;
	m_pKeybord->update();

}

void CGame::render()			//渲染
{
	FillRect(m_hBackDC,m_clientRect.ToRECT(),Graphic::BlackBrush);
	////////////////////////////开始绘制///////////////////////
	
	renderUI();
	renderText();
	////////////////////////////提交到前台/////////////////////
	BitBlt(m_hDeviceContext,0,0,m_nWidth,m_nHeight,m_hBackDC,0,0,SRCCOPY);
	//限制fps
	if (m_pFPS->getFPS()>60.0f)
	{
		Sleep(1);
	}
}
void CGame::renderText()
{
	char buffer[50];
	sprintf(buffer,"fps:%.2f",m_pFPS->getFPS());
	TextOut(m_hBackDC,0,0,buffer,strlen(buffer));
}
void CGame::renderUI()
{
	m_dialog.render(m_hBackDC);
}
//////////////////class Game end//////////////////////////////