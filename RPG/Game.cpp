//Game.cpp
#include "Commen.h"
#include "Game.h"
#include "MySound.h"
#include "FPS.h"
#include "Keyboard.h"

const UINT32 MIN_FRAME_ELAPSE = 1000 / 60;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    changeWorkDirToModulePath();
    CGame *pGame = new CGame(); //游戏类对象
    if (pGame->create(hInstance,
        "2D RPG          http://blog.csdn.net/you_lan_hai",
        640, 480))//初始化游戏类
    {
        pGame->mainLoop();//游戏循环
    }
    delete pGame;
    return 0;
}

void ButtonCallStart(void* pParam)
{
	CGame* pThis=(CGame*)pParam;
	pThis->Start();
}
void ButtonCallContinue(void* pParam)
{
	CGame* pThis=(CGame*)pParam;
	pThis->Continue();
}
void ButtonCallExit(void* pParam)
{
	CGame* pThis=(CGame*)pParam;
	pThis->Exit();
}

CGame::CGame()
{
}

CGame::~CGame()
{
	if(hFont)
	{
		DeleteObject(hFont);
	}
}

void CGame::onClose()
{

}

void CGame::onTick()
{
    static UINT uOldTime = 0;
    UINT uCurrentTime = timeGetTime();
    if (uCurrentTime - uOldTime < 100)
    {
        return;
    }
    uOldTime = uCurrentTime;

    m_pKeyboard->update();

    switch (gameState)
    {
    case GAME_INIT:
        break;
    case GAME_HEADER:
        if (animation.IsFinished())
        {
            gameState = GAME_MENU;
        }
        break;
    case GAME_MENU:
        LoopBGMusic(0);
        menuForm.ProcessMessage(m_hWnd, this);
        break;
    case GAME_RUN:
        LoopBGMusic(0);
        onRun();
        break;
    case GAME_SPEAKER:
        LoopBGMusic(0);
        if (speaker.IsFinished())
        {
            gameState = GAME_RUN;
        }
        if (m_pKeyboard->isKeyDown(VK_SPACE))
        {
            speaker.AddPtr();
        }
        break;
    case GAME_FIGHT:
        LoopBGMusic(1);
        switch (fightSystem.GetResult())
        {
        case 0:
            break;
        case 1://战胜
            player.UpdateLevel();
            LoadWave(0, "sound/win.wav");

            StopAllMid();
            PlaySound(0);

            gameState = GAME_WIN;
            break;
        case 2://战败
            LoadWave(0, "sound/over.wav");
            StopAllMid();
            PlaySound(0);

            gameState = GAME_OVER;
            break;
        }
        break;
    case GAME_OVER:
        if (!IsSounPlaying(0))
        {
            StopSound(0);
            DeleteSound(0);
            gameState = GAME_HEADER;
        }
        break;
    case GAME_WIN:
        if (!IsSounPlaying(0))
        {
            StopSound(0);
            DeleteSound(0);
            gameState = GAME_RUN;
        }
        break;

    case GAME_END:
        StopAllMid();
        StopAllSound();
        DeleteAllMid();
        DeleteAllSound();
        PostQuitMessage(0);
        break;
    default:
        break;
    }
}

void CGame::onRun()
{
    int dir = -1;
    if (m_pKeyboard->isKeyPress('W'))
    {
        dir = 0;
    }
    else if (m_pKeyboard->isKeyPress('S'))
    {
        dir = 3;
    }

    if (m_pKeyboard->isKeyPress('A'))
    {
        dir = 1;
    }
    else if (m_pKeyboard->isKeyPress('D'))
    {
        dir = 2;
    }
    player.Move(dir);

    std::list<CSprit*>::iterator it = spritManager.spritList.begin();
    while (it != spritManager.spritList.end())
    {
        CSprit* pSprit = *it;
        if (pSprit->nState != 0)
        {
            pSprit->AI(&player);
            ++it;
        }
        else
        {
            it = spritManager.spritList.erase(it);
            delete pSprit;
        }
    }

    CGameObject* pObject = player.CheckArround(2);
    if (pObject && pObject->nState != 0)
    {
        if (pObject->nShowID < 1)
        {
            if (m_pKeyboard->isKeyPress(VK_SPACE))
            {
                std::string strWords = "speaker/";
                strWords += pObject->strName + ".dat";
                if (speaker.ReadFromFile(m_hInstance, "speaker/speaker.dat", strWords))
                {
                    gameState = GAME_SPEAKER;
                }
            }
        }
        else
        {
            player.nMagic = player.nMaxMagic;
            fightSystem.SetFighter(&player, pObject);
            StopMid(0);
            LoadMid(1, "sound/fight.mid");
            gameState = GAME_FIGHT;
        }
    }

    if (scene.CheckSwitchScene(&player.ptPosition))
    {//切换场景
        player.SetPosition(&scene.ptPlayerPosition);
        mapSytem.ReadMapFromFile(scene.mapName);
        pathSystem.CreatePath(mapSytem.nXCount, mapSytem.nYCount);
        spritManager.ReadFromFile(scene.spritName);
        StopMid(0);
        DeleteMid(0);
        char buffer[100];
        strcpy(buffer, scene.backMusic.c_str());
        LoadMid(0, buffer);
        PlayMid(0);
    }
}

bool CGame::onInit()
{
	{//初始化公共组件
		if(!CEffect::InitializeCommen(m_hInstance,"fight/fight.dat"))
        {
            ::MessageBox(NULL, "加载资源失败，请检查程序启动目录！", "错误", MB_ICONERROR);
            return false;
        }

		CGameMenu::InitMenu(m_hWnd,m_hInstance,"menu/menu.dat");
		CGameObject::InitializeCommen(m_hInstance,"sprit/sprit.dat");	
		CSprit::InitializeCommenSystem(&pathSystem,&mapSytem);
	}
	{//初始化音乐
		InitSound(m_hWnd,10);
		InitMusic(m_hWnd,5);
	
		LoadMid(0,"sound/menu.mid");
		PlayMid(0);
	}
	{//初始化场景
        scene.ReadFromFile("scene/center1.sce");

		mapSytem.Initialize(m_hInstance,"object/object.dat","ground/ground.dat");
		mapSytem.ReadMapFromFile(scene.mapName);
	//	mapSytem.SetShowObstacle(true);
		pathSystem.CreatePath(mapSytem.nXCount,mapSytem.nYCount);
	}
	{//初始化战斗系统
		fightSystem.Initialize(m_hWnd,m_hInstance);
		fightSystem.SetBackGroundBitmap("ground/ground.dat");
		//fightSystem.SetFighterHeadBitmap("");
		fightSystem.SetFightFormBitmap("menu/menu.dat");
		//fightSystem.SetGoodsFormBitmap("");
		fightSystem.SetMagicList("fight/MagicList.dat");
		fightSystem.SetUIBitmap("ui/ui.dat");

	}
	{//初始化动画系统
		animation.Initialize(m_hInstance,"animation/animation.dat");
	}
	{//初始化精灵管理器
		spritManager.ReadFromFile(scene.spritName);
	}
	{//初始化对话系统
		speaker.ReadFromFile(m_hInstance,"speaker/speaker.dat","speaker/想.dat");
	}
	{//初始化玩家信息
		player.Initialize(0,"想",1,10,1000,500,50,20,1);
		player.SetPosition(300,100);
	}
	{//初始化字体
		hFont=CreateFont(
			20,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			"Arial");                 // lpszFacename
        SelectObject(m_dcDevice.getHdc(), hFont);
	}
	//初始化菜单窗口
	{	
		menuButtonBitmap.CreateBitmapFromFileList(m_hInstance,"ui/menubutton.dat");

		RECT	rcClient;
		GetClientRect(m_hWnd,&rcClient);

		menuForm.InitForm("",FORM_POPUP);
		menuForm.SetRect(0,0,rcClient.bottom,rcClient.right);
		menuForm.SetColor(RGB(255,255,255),RGB(0,0,0));
		menuForm.SetState(FORM_SHOW);

		CGameButton *pButton=NULL;
		int x=rcClient.right/2;
		int dy=50;
		int y=0;
		LBitmapInfo bmi=menuButtonBitmap.GetAt(0);
		if(bmi)
		{
			x=(rcClient.right - bmi->nWidth)/2;
			y=(rcClient.bottom-bmi->nHeight)/4;
		}
		pButton=new CGameButton("",BUTTON_STATIC);
		pButton->SetBitmap(&menuButtonBitmap,0);
		pButton->SetRect(x,y);
		menuForm.AddButton(pButton);
		
		bmi=menuButtonBitmap.GetAt(1);
		if(bmi)
		{
			x=(rcClient.right - bmi->nWidth)/2;
			dy=bmi->nHeight+10;
		}
		y=rcClient.bottom-4*dy;
		pButton=new CGameButton("",BUTTON_DRAW,ButtonCallStart);
		pButton->SetBitmap(&menuButtonBitmap,1);
		pButton->SetRect(x,y);
		menuForm.AddButton(pButton);
		
		pButton=new CGameButton("",BUTTON_DRAW,ButtonCallContinue);
		pButton->SetBitmap(&menuButtonBitmap,4);
		pButton->SetRect(x,y+dy);
		menuForm.AddButton(pButton);
		
		pButton=new CGameButton("",BUTTON_DRAW,ButtonCallExit);
		pButton->SetBitmap(&menuButtonBitmap,7);
		pButton->SetRect(x,y+2*dy);
				menuForm.AddButton(pButton);
	}

	gameState=GAME_HEADER;
//	gameState=GAME_RUN;
    return true;
}
void CGame::Start()
{
	speaker.ReadFromFile(m_hInstance,"speaker/speaker.dat","speaker/想.dat");
	gameState=GAME_SPEAKER;
//	gameState=GAME_RUN;
	StopMid(0);
	DeleteMid(0);
	char buffer[100];
	strcpy(buffer,scene.backMusic.c_str());
	LoadMid(0,buffer);
}
void CGame::Continue()
{
	gameState=GAME_RUN;
	StopMid(0);
	DeleteMid(0);
	char buffer[MAX_PATH];
	strcpy(buffer,scene.backMusic.c_str());
	LoadMid(0,buffer);
}
void CGame::Exit()
{
	gameState=GAME_END;
}
void CGame::onDraw()				//游戏循环
{
    //限制FPS不超过60帧/S
    static UINT uOldTime = 0;
    UINT uCurrentTime = timeGetTime();
    UINT elapse = uCurrentTime - uOldTime;
    if (elapse < MIN_FRAME_ELAPSE)
    {
        Sleep(MIN_FRAME_ELAPSE - elapse);
        return;
    }
    uOldTime = uCurrentTime;

    m_pFPS->updateFPS();

    RECT rcWindowRect;
    GetClientRect(m_hWnd, &rcWindowRect);
    m_dcDevice.fillSolidRect(&rcWindowRect, 0);
    /////////在此添加绘制代码///////////////////////
    
    onRender();

    //////////////////////////////////////////////
    char szShowFPS[64];
    sprintf(szShowFPS, "fps:%.2f", m_pFPS->getFPS());
    m_dcDevice.textOut(0, 0, szShowFPS, strlen(szShowFPS));
    ////////拷贝掩码DC的内容到屏幕////////////
    m_dcDevice.present();
}

void CGame::LoopBGMusic(int index)
{
    if (!IsPlaying(index))
        PlayMid(index);
}

void CGame::onRender()
{
    HDC backDC = m_dcDevice.getHdc();

	RECT	rcClient;
	GetClientRect(m_hWnd,&rcClient);
	switch(gameState)
	{
	case GAME_INIT:
		break;
	case GAME_HEADER:
        animation.DrawHeader(backDC, &rcClient);
		break;
	case GAME_MENU:
        menuForm.Show(backDC);
		break;
	case GAME_RUN:
        mapSytem.Show2(backDC, &player, &rcClient);
		break;
	case GAME_SPEAKER:
        mapSytem.Show2(backDC, &player, &rcClient);
        speaker.Show(backDC, &rcClient);
		break;
	case GAME_FIGHT:
        fightSystem.FightLoop(backDC);
		break;		
	default:
		break;
	}
}
bool CGame::onWindowMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            SetGameState(GAME_MENU);
            return true;
        }
        break;
    case WM_CLOSE:
        return true;

    default:
        return false;
    }

    return false;
}
//////////////////class GameEngine end//////////////////////////////