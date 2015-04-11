//Game.cpp
#include "../G2DEngine/Commen.h"
#include "../G2DEngine/FPS.h"
#include "../G2DEngine/Keyboard.h"
#include "Game.h"

enum GUIID
{
	GUI_BTN_TEST=100,
};

CGame	g_game;//��Ϸ�����
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
///////////////////������///////////////////////////////////////
int WINAPI WinMain( HINSTANCE hInstance,HINSTANCE,LPSTR,int)
{
	g_log.init("log.txt");
	if(g_game.init(hInstance,
		"GameEngine          http://hi.baidu.com/������",
		640,480))//��ʼ����Ϸ��
	{
		g_game.mainLoop();//��Ϸѭ��
	}
	Sleep(100);
	return 0;
}
////////////////class CGame start//////////////////////////////

//ע�ᴰ����
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
//���ڹ���
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

void CGame::destroy()			//������Դ
{
	SafeDelete(m_pFPS);
	SafeDelete(m_pKeybord);
	if (m_backFont != NULL)
	{
		DeleteObject(m_backFont);
	}
	
}
bool CGame::initGame()		//��ʼ����Ϸ
{
	gameState=GAME_INIT;
	RECT rc;
	GetClientRect(m_hWnd,&rc);
	m_clientRect=CGameRect(rc.left,rc.top,rc.right,rc.bottom);
	
	m_backFont=createFontEx(15,"����");
	if (m_backFont == NULL)
	{
		g_log.write("��������ʧ�ܣ�");
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
	m_dialog.addControl(new CUIButton("���ǰ�ť",GUI_BTN_TEST,0,20,100,30));
	m_dialog.addControl(new CUIStatic("���Ǿ�̬�ı���ǩ��ʹ�øÿؼ����Է������Ի���ؼ�����ʾ�ı���\
									  \r\n�˴��л��С�",0,0,60,200,100));
	m_dialog.enableCaption(true);
	m_dialog.setManaged(false);//ע�⾲̬��������������Դ���Զ�����
	m_pane.addControl(&m_dialog);

	return true;
}
void CGame::update()			//����
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

void CGame::render()			//��Ⱦ
{
	FillRect(m_hBackDC,m_clientRect.ToRECT(),Graphic::BlackBrush);
	////////////////////////////��ʼ����///////////////////////
	
	renderUI();
	renderText();
	////////////////////////////�ύ��ǰ̨/////////////////////
	BitBlt(m_hDeviceContext,0,0,m_nWidth,m_nHeight,m_hBackDC,0,0,SRCCOPY);
	//����fps
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