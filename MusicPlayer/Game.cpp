//Game.cpp
#include "../G2DEngine/Commen.h"
#include "../G2DEngine/FPS.h"
#include "../G2DEngine/Keyboard.h"
#include "../G2DEngine/DIBBitmap.h"

#include "Particle.h"
#include "BrushPanel.h"
#include "LRCLoader.h"
#include "Mp3Player.h"
#include "BitmapViewer.h"
#include "Timer.h"
#include "Cursor.h"
#include "Hero.h"

#include "Game.h"
#include "resource.h"


LRESULT OnUIEvent(int id, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(id)
    {
    case GUID_MP3:
        break;
    default: break;
    }
    return 0;
}



///////////////////������///////////////////////////////////////
int WINAPI WinMain( HINSTANCE hInstance,HINSTANCE,LPSTR,int)
{
    changeWorkDirToModulePath();

    CGame *pGame = new CGame();//��Ϸ�����
    if (pGame->create(hInstance,
        "PLAN-A          http://blog.csdn.net/you_lan_hai",
        800,600))//��ʼ����Ϸ��
	{
        pGame->mainLoop();//��Ϸѭ��
	}
    delete pGame;
	return 0;
}

//////////////////////////////////////////////////////////////////////////

////////////////class CGame start//////////////////////////////
CGame::CGame()
{
    m_pMP3=NULL;
}

CGame::~CGame()
{
}

void CGame::onClose()			//������Դ
{
}


//ע�ᴰ����
void CGame::onRegisterClass(WNDCLASSEX *pwc)	//ע�ᴰ����
{
    pwc->hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_ICON1));
}

bool CGame::onInit(void)
{
    srand(timeGetTime());

    if(!initUI())
    {
        return false;
    }

    gameState=GAME_INIT;

    RECT rc;
    getClientRect(&rc);
    m_clientRect=CGameRect(rc.left,rc.top,rc.right,rc.bottom);

    if (m_backFont.createFontEx(15,"����"))
    {
        g_log.write("��������ʧ�ܣ�");
    }
    else
    {
        m_dcDevice.selectObject(m_backFont.getHFont());
    }

    m_pTimer=new CTimer();
    m_pTimer->set(GTID_GAME_LOOP,50);

    m_pKeybord=new CKeyboard();

    m_pBrush=new CBrushPanel();
    m_pBrush->createRandom(300);

    if(!m_bmpManager.load(m_hInstance,"background/background.dat"))
    {
        g_log.write("��ʼ��λͼ������ʧ�ܣ�\
                    ��ȷ�������ļ�background/background.dat�Ƿ���ڣ�");
        return false;
    }

    if(!m_bmpSprit.load(m_hInstance,"sprit/sprit.dat"))
    {
        g_log.write("��ʼ��λͼ������ʧ�ܣ�\
                    ��ȷ�������ļ�sprit/sprit.dat�Ƿ���ڣ�");
        return false;
    }
    CHero::setBmpManager(&m_bmpSprit);
    m_pJJ = new CHero();
    m_pJJ->init(0);
    m_pJJ->setSpeed(100);
    m_pJJ->setPos(m_nWidth/2,m_nHeight/2);

    m_pBmpViewer=new CBitmapViewer();
    m_pBmpViewer->setBitmap(m_bmpManager.GetAt(0)->hBitmap);
    m_pBmpViewer->setRect(&(CGameRect(0,0,m_nWidth,m_nHeight)));
    m_pBmpViewer->setFadeStyle(4+8);
    m_pBmpViewer->setFadeTime(1500);


    m_pExplode = new CExplode(300,
        &Vector2D(m_nWidth/2.0f,m_nHeight/2.0f),
        m_pBrush.get());

    m_pSnow = new CSnow();
    m_pSnow->init(100,&rc);

     ShowCursor(FALSE);

    m_pFire = new CFire();
    if (!m_pFire->init(m_dcDevice.getHdc(),m_nWidth,128))
    {
        g_log.write("��������ʧ�ܣ�");
        return false;
    }
    m_pFire->reset();
    m_pFire->setRedution(3);
    m_pFire->setTransparent(false);

    return true;
}

bool CGame::initUI()
{
    int h = 100;
    m_pane.init(1,m_nWidth,m_nHeight);
    m_pane.setEventCallBack(OnUIEvent);
    m_pane.addControl(&m_dlgTalk);

    m_dlgTalk.init(m_hWnd,"",3,0,m_nHeight-h,m_nWidth,h);
    m_dlgTalk.setManaged(false);
    m_dlgTalk.setBackColor(0);
    m_dlgTalk.setBlendAlpha(100);


    m_pMP3 = new CMp3Player();
    m_pMP3->init("�ı�",GUID_TALK,0,0,m_nWidth,h);
    m_pMP3->setForeColor(RGB(255,255,255));
    m_pMP3->setFontName("����");
    m_pMP3->setFontSize(30);
    m_pMP3->setDrawStyle(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
    m_pMP3->setBackColor(RGB(0,50,255));
    m_pMP3->setForeColor(RGB(255,0,0));

    m_dlgTalk.addControl(m_pMP3);

    if (!m_pMP3->loadMp3("music/aiyaaiya.mp3"))
    {
        g_log.write("���ظ�����music/aiyaaiya.mp3��ʧ�ܣ�");
        return false;
    }

    m_pMP3->setRepeat(true);
    m_pMP3->play();

    return true;
}

void CGame::onTick()			//����
{
	m_pFPS->updateFPS();
    m_pTimer->update();
    
#if 1
    if (!m_pTimer->isOver(GTID_GAME_LOOP))
    {
        return ;
    }
    int nEslipse = (m_pTimer->at(GTID_GAME_LOOP))->nElapse;
#else
    static int oldUpdateTime = timeGetTime();
    int curTime = timeGetTime();
    int nEslipse = curTime - oldUpdateTime;

    if (nEslipse < 50 )
    {
        return ;
    }
    oldUpdateTime = curTime;
#endif

	m_pKeybord->update();


    if(m_pBmpViewer->update(nEslipse))
    {
        int index = rand()%m_bmpManager.GetCount();
        m_pBmpViewer->setBitmap(m_bmpManager.atBitmap(index));
    }

#if 1
    if(m_pExplode->isEmpty())//�����ը
    {
        POINT pt;
        pt.x = rand()%m_nWidth;
        pt.y = rand()%m_nHeight;

        m_pExplode->init(300,&Vector2D(pt),m_pBrush.get());
    }

    m_pExplode->update(nEslipse);
    m_pSnow->update(nEslipse);

#endif

    m_pMP3->update(nEslipse);

    if (m_pKeybord->isKeyUp(VK_F2))//���ػ���ʾ���
    {
       m_dlgTalk.setVisible(!m_dlgTalk.getVisible());
    }
    else if(m_pKeybord->isKeyUp(VK_F3))//��ͣ/����
    {
       if (m_pMP3->isPlaying())
       {
           m_pMP3->pause();
       }
       else
       {
           m_pMP3->play();
       }
    }
    else if(m_pKeybord->isKeyUp(VK_F4))//��ͣ/����
    {
        m_pMP3->stop();
    }
    //��ͼ����
    if(m_pKeybord->isKeyUp(VK_F8))
    {
        CDIBBitmap bmp;
        if(!bmp.create(m_dcDevice.getHdc(),m_dcDevice.getBackBitmap()->getHBitmap()))
        {
            g_log.write("��ͼʧ�ܣ�");
        }
        else
        {
            CreateDirectory(TEXT("shot"),NULL);
            char buffer[64];
            SYSTEMTIME tm;
            GetLocalTime(&tm);
            sprintf_s(buffer,64,"shot/%d_%d_%d_%d_%d_%d.bmp",
                tm.wYear,tm.wMonth,tm.wDay,tm.wHour,tm.wMinute,tm.wSecond);
            if(!bmp.save(buffer))
            {
                g_log.writex("����λͼ��%s��ʧ��!",buffer);
            }
        }
    }

    m_pFire->update(nEslipse);


    if (m_pKeybord->isKeyDown('W'))
    {
        m_pJJ->moveY(false);
    }
    else if (m_pKeybord->isKeyDown('S'))
    {
        m_pJJ->moveY(true);
    }
    if (m_pKeybord->isKeyDown('A'))
    {
        m_pJJ->moveX(false);
    }
    else if (m_pKeybord->isKeyDown('D'))
    {
        m_pJJ->moveX(true);
    }
    m_pJJ->update(nEslipse,m_clientRect.ToRECT());


    m_pTimer->reset(GTID_GAME_LOOP);
}

void CGame::onDraw()			//��Ⱦ
{
	m_dcDevice.fillSolidRect(m_clientRect.ToRECT(),RGB(255,255,255));
	////////////////////////////��ʼ����///////////////////////

    m_pBmpViewer->render(m_dcDevice.getHdc());

#if 0
    {//���ƻ���
        int y= m_nHeight - m_pFire->getDibBmp().getHeight();
        m_pFire->render(m_dcDevice.getHdc(),0,y);
    }

    m_pJJ->render(m_dcDevice.getHdc());
#endif

    m_pExplode->render(m_dcDevice.getHdc());
    m_pSnow->render(m_dcDevice.getHdc());


	renderUI();
	renderText();
	////////////////////////////�ύ��ǰ̨/////////////////////
	m_dcDevice.present();
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
	m_dcDevice.textOut(0,0,buffer,strlen(buffer));
}
void CGame::renderUI()
{
    m_pane.render(m_dcDevice.getHdc());
}
//////////////////class Game end//////////////////////////////