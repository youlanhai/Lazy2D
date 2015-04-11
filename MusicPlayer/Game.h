//Game.h
#pragma once
#include "../G2DEngine/App.h"
#include "../G2DEngine/GDI.h"

enum GAME_UI_ID
{
    GUID_STATIC = 100,
    GUID_TALK ,
    GUID_MP3,
    GUID_FORCE_WORD = 0xffffffff
};

enum GAME_TIMER_ID
{
    GTID_GAME_LOOP = 100,
    GTID_FORCE_DWORD = 0xffffffff
};


/**********************
��Ϸ״̬
**********************/
enum GameState
{
    GAME_INIT,		//��ʼ��
    GAME_HEADER,	//��ͷ����
    GAME_MENU,		//��ʼ�˵�
    GAME_RUN,		//��Ϸ����
    GAME_END,		//��Ϸ����
    GAME_SPEAKER,	//�Ի�
    GAME_FIGHT,		//ս��
    GAME_OVER,		//ս��ʧ��
    GAME_WIN		//ս��ʤ��
};

class CFPS;
class CKeyboard;

class CSnow;
class CExplode;

class CBrushPanel;
class CLRCLoader;
class CMp3Player;
class CBitmapViewer;
class CTimer;
class CHero;


/****************************************************
��Ϸ��
��Ϸ��ʼ������Ϸ�����̵ȡ�
******************************************************/
class CGame : public CApp
{
public:
	CGame();						//���캯��
	~CGame();						//��������

protected:
    /*ע�ᴰ����*/
    void onRegisterClass(WNDCLASSEX *pwc);	

    /*��ʼ����Ϸ*/
    bool onInit() ; 

    /*����*/
    void onTick() ;	

    /*��Ⱦ*/
    void onDraw() ;	

    /*������Դ*/	
    void onClose() ;	

	void renderText();
	void renderUI();

    bool initUI();

public:
    GameState	gameState;			//��Ϸ״̬
    CGameRect	m_clientRect;
    CFont		m_backFont;

    RefPtr<CTimer>          m_pTimer;
    RefPtr<CKeyboard>	    m_pKeybord;

    CBitmapManager m_bmpManager;
    CBitmapManager m_bmpSprit;

    CUIPane     m_pane;
    CUIDialog   m_dlgTalk;
    CMp3Player  *m_pMP3;

    RefPtr<CBitmapViewer>   m_pBmpViewer;
    RefPtr<CExplode>        m_pExplode;
    RefPtr<CSnow>           m_pSnow;
    RefPtr<CBrushPanel>     m_pBrush;
    RefPtr<CFire>           m_pFire;
    RefPtr<CHero>           m_pJJ;
    
};