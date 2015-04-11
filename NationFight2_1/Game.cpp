//Game.cpp
#include "../G2DEngine/Commen.h"
#include "../G2DEngine/FPS.h"
#include "../G2DEngine/Keyboard.h"
#include "../SoundEngine/DxMusic.h"
#include "../SoundEngine/DxSound.h"

#include "NFScript.h"
#include "Background.h"
#include "Player.h"
#include "Enemy.h"
#include "Game.h"
#include "resource.h"

#include <algorithm>
//#define NATIONFIGHT_TEST

CGame	g_game;//��Ϸ�����
const int g_pInjure[14]={10,50,30,40,50,60,70,80,90,100,110,130,300,500};

enum GAME_UI_ID
{
	GUID_MENU_START=1000,
	GUID_MENU_HELP,
	GUID_MENU_EXIT,
	GUID_HELP_OK
};

LRESULT OnUIEvent(int id, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(id)
	{
	case GUID_MENU_START:
		{
			g_game.onMenuStart();
			break;
		}
	case GUID_MENU_HELP:
		{
			g_game.onMenuHelp();
			break;
		}
	case GUID_MENU_EXIT:
		{
			PostQuitMessage(0);
			break;
		}
	case GUID_HELP_OK:
		{
			g_game.m_Help.setVisible(false);
			break;
		}
	}
	return 0;
}

///////////////////������///////////////////////////////////////
int WINAPI WinMain( HINSTANCE hInstance,HINSTANCE,LPSTR,int)
{
    changeWorkDirToModulePath();
	g_log.init("log.txt");
	if(g_game.create(hInstance,
		"NationFight2.1",
		600,740))//��ʼ����Ϸ��
	{
		g_game.mainLoop();//��Ϸѭ��
	}
	Sleep(100);
	return 0;
}
////////////////class CGame start//////////////////////////////

//ע�ᴰ����
void CGame::onRegisterClass(WNDCLASSEX *pwc)
{
    pwc->hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_ICON1));
}

//���ڹ���
bool CGame::onWindowMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	g_game.m_pane.MessageProc(hWnd,msg,wParam,lParam);
	return 0;
}

CGame::CGame()
{
	m_pSound=NULL;
    m_pMusic=NULL;
	m_pBackground=NULL;
	m_pPlayer=NULL;//�����ֶ�delete
    m_pBoss = NULL;//�����ֶ�delete
	m_backFont=NULL;
	m_pScript=NULL;
	m_updateCounter=0;
}
CGame::~CGame()
{
}

void CGame::onClose()			//������Դ
{
	if (m_backFont != NULL)
	{
		DeleteObject(m_backFont);
	}
	SafeDelete(m_pSound);
    SafeDelete(m_pMusic);
	SafeDelete(m_pBackground);
	SafeDelete(m_pScript);
}
bool CGame::onInit()		//��ʼ����Ϸ
{
	srand(timeGetTime());
	m_updateTimeInterval = 30;//30ms
	gameState=GAME_INIT;
	RECT rc;
	GetClientRect(m_hWnd,&rc);
	m_clientRect=CGameRect(rc.left,rc.top,rc.right,rc.bottom);
	
	m_backFont=createFontEx(15,"����");
	if (m_backFont == NULL)
	{
		g_log.write("��������ʧ�ܣ�");
	}
    m_dcDevice.selectObject(m_backFont);

    if (!m_heroBmp.CreateBitmapFromFileList(m_hInstance, "res/hero.dat"))
    {
        ::MessageBox(NULL, "�޷��ҵ���Դ�ļ��������������·����",
            "����", MB_ICONERROR);
        return false;
    }

	m_enemyBmp.CreateBitmapFromFileList(m_hInstance,"res/enemy.dat");
	m_bulletBmp.CreateBitmapFromFileList(m_hInstance,"res/bullet.dat");
	m_backBmp.CreateBitmapFromFileList(m_hInstance,"res/back.dat");
	m_explodeBmp.CreateBitmapFromFileList(m_hInstance,"res/explode.dat");
	m_prizeBmp.CreateBitmapFromFileList(m_hInstance,"res/prize.dat");

	m_pBackground=new CBackground();
	m_pBackground->init(m_nWidth,m_nHeight);

	m_pScript=new CNFScript();

	m_pane.init(1,m_nWidth,m_nHeight);
	m_pane.setEventCallBack(OnUIEvent);

	int w=200;
	int h=220;
	m_Menu.init(m_hWnd,"menu",2,(m_nWidth-w)/2,(m_nHeight-h)/2,w,h);
	m_Menu.setBackColor(RGB(0,0,255));
	int x=(w-125)/2;
	int y=30;
	w=125;
	h=30;
	m_Menu.addControl(new CUIStatic("Menu",0,x,y,w,h));
	m_Menu.addControl(new CUIButton("��ʼ",GUID_MENU_START,x,y+=50,w,h));
	m_Menu.addControl(new CUIButton("����",GUID_MENU_HELP,x,y+=40,w,h));
	m_Menu.addControl(new CUIButton("�˳�",GUID_MENU_EXIT,x,y+=40,w,h));
	m_Menu.setManaged(false);
	m_Menu.setBlendAlpha(220);
	m_pane.addControl(&m_Menu);

	w=300;
	h=200;
	m_Help.init(m_hWnd,"����",3,(m_nWidth-w)/2,(m_nHeight-h)/2+30,w,h);
	m_Help.setBackColor(RGB(255,255,255));
	m_Help.setBlendAlpha(100);
	m_Help.setVisible(false);
	m_Help.enableCaption(true);
	CUIStatic *pStatic=new CUIStatic("WSAD�ƶ���J��ͨ�ӵ���K����ը����L����ը����\
		\r\n\r\n��ӭ�����ҵĲ��ͣ�http://hi.baidu.com/������",
		0,20,40,w-40,h-100);
	pStatic->setDrawStyle(DT_LEFT|DT_TOP|DT_WORDBREAK);
	m_Help.addControl(pStatic);
	m_Help.addControl(new CUIButton("ȷ��",GUID_HELP_OK,(w-125)/2,h-40,125,30));
	m_Help.setManaged(false);
	m_pane.addControl(&m_Help);

    m_pSound=new CDxSound();
    m_pSound->init(m_hWnd);
    m_pSound->loadFromFile("res/sound.dat");

    m_pMusic=new CDxMusic();
    m_pMusic->init(m_hWnd);
    {
        m_pMusic->load(0,"res/sound/menu.mid");
        m_pMusic->load(1,"res/sound/backsound.mid");
        m_pMusic->load(2,"res/sound/boss.mid");
    }
    m_nMusicID = 0;

	setGameState(GAME_MENU);
	return true;
}
void CGame::setGameState(GameState gs)
{
	switch(gs)
	{
	case GAME_INIT:		//��ʼ��
		break;

	case GAME_HEADER:	//��ͷ����
		break;

	case GAME_MENU:		//��ʼ�˵�
        m_Menu.setVisible(true);
        m_pBackground->bFlow=true;
        m_pBackground->nBackBmpIndex=0;
        m_pMusic->stopAll();
        m_nMusicID = 0;

		break;

	case GAME_RUN:		//��Ϸ����
        m_pMusic->stopAll();
        if (m_pBoss != NULL)
        {
            m_nMusicID = 2;
        }
        else m_nMusicID = 1;
		break;

	case GAME_END:		//��Ϸ����
		onEnd();
		break;

	case GAME_OVER:		//ս��ʧ��

        onEnd();
        m_pBackground->bFlow=false;
        m_pBackground->nBackBmpIndex=2;

		break;

	case GAME_WIN:		//ս��ʤ��

        onEnd();
        m_pBackground->bFlow=false;
        m_pBackground->nBackBmpIndex=3;

		break;

	default:break;
	}
	gameState=gs;
}
void CGame::onTick()			//����
{
	m_pFPS->updateFPS();
	int curTime=timeGetTime();
	static int oldUpdateTime=0;
	if(curTime - oldUpdateTime < m_updateTimeInterval)
	{
		return;
	}
	oldUpdateTime=curTime;

	++m_updateCounter;

	m_pKeyboard->update();
	m_pBackground->update();

    if (!m_pMusic->isPlaying(m_nMusicID))//ѭ�����ű�������
    {
        m_pMusic->play(m_nMusicID);
    }

	switch(gameState)
	{
	case GAME_INIT:		//��ʼ��
		break;

	case GAME_HEADER:	//��ͷ����
		break;

	case GAME_MENU:		//��ʼ�˵�
		onMenu();
		break;

	case GAME_RUN:		//��Ϸ����
		onRun();
		break;

	case GAME_END:		//��Ϸ����
		//onEnd();
		break;

	case GAME_OVER:		//ս��ʧ��
		onOver();
		break;

	case GAME_WIN:		//ս��ʤ��
		onWin();
		break;

	default:break;
	}
}

void CGame::onDraw()			//��Ⱦ
{
    HDC hBackDC = m_dcDevice.getHdc();
    m_dcDevice.fillSolidRect(m_clientRect.ToRECT(), 0);
    m_pBackground->render(hBackDC);
	////////////////////////////��ʼ����///////////////////////
	
    m_plyerManager.render(hBackDC);
    m_enemyManager.render(hBackDC);
    m_bulletManager.render(hBackDC);
    m_enBulletManager.render(hBackDC);
    m_explodeManager.render(hBackDC);
    m_prizeManager.render(hBackDC);

    m_pane.render(hBackDC);

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
	int y=0;
	const int h=17;
	char buffer[50];
	sprintf(buffer,"fps:%.2f",m_pFPS->getFPS());
    m_dcDevice.textOut(0, 0, buffer, strlen(buffer));
	if (GAME_RUN == gameState)
	{
		sprintf(buffer,"����:%d/%d",m_pPlayer->life,m_pPlayer->max_life);
        m_dcDevice.textOut(0, y += h, buffer, strlen(buffer));

		int level=m_pPlayer->level;
		int nextLevelScore=(level*level*level+4)*100;
		sprintf(buffer,"����:%d/%d",m_pPlayer->score,nextLevelScore);
        m_dcDevice.textOut(0, y += h, buffer, strlen(buffer));

		sprintf(buffer,"�ȼ�:%d",m_pPlayer->level);
        m_dcDevice.textOut(0, y += h, buffer, strlen(buffer));

		sprintf(buffer,"ը��:%d",m_pPlayer->bomb);
        m_dcDevice.textOut(0, y += h, buffer, strlen(buffer));

		sprintf(buffer,"����:%d,����%d",m_pPlayer->attack,m_pPlayer->defence);
        m_dcDevice.textOut(0, y += h, buffer, strlen(buffer));
		
	}
	else if (GAME_OVER == gameState)
	{
		strcpy(buffer,"����ˣ�");
        m_dcDevice.textOut(m_nWidth / 2 - 20, m_nHeight / 2, buffer, strlen(buffer));
	}
    if (m_pFPS->getFPS() > 60.0f)
    {
        Sleep(1);
    }

}
void CGame::onMenu()
{
//     if (!m_pMusic->isPlaying(0))
//     {
//         m_pMusic->play(0);
//     }
}
void CGame::onRun()
{
	if(m_pKeyboard->isKeyDown(VK_ESCAPE))//�˵�
	{
		setGameState(GAME_MENU);
		return;
	}
	else if (m_pKeyboard->isKeyDown(VK_F1))//����
	{
		onMenuHelp();
	}
	if (m_pKeyboard->isKeyDown(VK_LBUTTON))//�������
	{
		m_pBackground->bShake=true;
		m_pSound->play(0,false,false,false);
	}

	if (m_pKeyboard->isKeyPress('W'))//ǰ��
	{
		m_pPlayer->moveLook(true);
	}
    else if (m_pKeyboard->isKeyPress('S'))//����
	{
		m_pPlayer->moveLook(false);
	}
    if (m_pKeyboard->isKeyPress('A'))//����
	{
		m_pPlayer->moveRight(false);
	}
    else if (m_pKeyboard->isKeyPress('D'))//����
	{
		m_pPlayer->moveRight(true);
	}
	if (m_pKeyboard->isKeyPress('J'))//�����ӵ�
	{
		if (m_updateCounter%3 == 0 )
		{
			LBitmapInfo bmi=m_bulletBmp.GetAt(m_pPlayer->equipID);
			m_bulletManager.add(new Bullet(m_pPlayer->equipID,
				m_pPlayer->x+(m_pPlayer->w-(bmi->nWidth/bmi->nXFrame))/2,
				m_pPlayer->y-bmi->nHeight,
				0,-(m_pPlayer->vy+2)));
			m_pSound->play(1);
		}
	}
	if(m_pKeyboard->isKeyDown('K') && m_pPlayer->bomb>0)//��������ը��
	{
		static int end=-200;
		int bv=15;//ը���ٶ�
		if(m_updateCounter-end > m_clientRect.h/bv)
		{
			--m_pPlayer->bomb;
			end=m_updateCounter;
			LBitmapInfo bmi=g_game.m_bulletBmp.GetAt(12);
			int oldx=(bmi->nWidth/bmi->nXFrame)+15;
			for(int i=0;i<=m_clientRect.w/oldx;++i)
			{
				m_bulletManager.add(new Bullet(12,oldx*i,m_clientRect.h,0,-bv));
			}
			m_pSound->play(10);
		}
	}
	if(m_pKeyboard->isKeyDown('L') && m_pPlayer->bomb>0)//����ը��
	{
		static int end=-1000;
		int bv=20;//ը���ٶ�
		if(m_updateCounter-end > m_clientRect.h/bv)
		{
			m_enBulletManager.destroy();
			--m_pPlayer->bomb;
			end=m_updateCounter;
			LBitmapInfo bmi=g_game.m_bulletBmp.GetAt(12);
			int oldx=(bmi->nWidth/bmi->nXFrame)+15;
			for(int i=0;i<=m_clientRect.w/oldx;++i)
			{
				m_bulletManager.add(new Bullet(11,oldx*i,m_clientRect.h,0,-bv));
			}
			m_pSound->play(10);
			for (int i=0;i<50;++i)
			{
				m_explodeManager.add(new Explose(2,rand()%m_nWidth,rand()%m_nHeight));
			}
		}
	}

#ifdef NATIONFIGHT_TEST
	if (m_updateCounter%30 == 0)
	{
		int id=rand()%10;
		m_enemyManager.add(new Enemy(id,id,
			rand()%m_nWidth,
			-m_enemyBmp.GetAt(id)->nHeight,
			rand()%10 - 5,rand()%10));
	}
#endif

	CheckHit();

	if (0 == m_pPlayer->state)
	{
		setGameState(GAME_OVER);
		return;
	}
    else if(m_pBoss!=NULL &&  0==m_pBoss->state)
    {
        setGameState(GAME_WIN);
    }

	m_plyerManager.update(m_pPlayer,m_clientRect.ToRECT());
	
    m_enemyManager.update(NULL,m_clientRect.ToRECT());
    m_pBoss = (Enemy*)m_enemyManager.getByName("boss");

	m_bulletManager.update(NULL,m_clientRect.ToRECT());
	m_enBulletManager.update(NULL,m_clientRect.ToRECT());
	m_explodeManager.update(NULL,m_clientRect.ToRECT());
	m_prizeManager.update(NULL,m_clientRect.ToRECT());

    m_pScript->run();

}
void CGame::onEnd()
{
	m_plyerManager.destroy();
	m_enemyManager.destroy();
	m_bulletManager.destroy();
	m_enBulletManager.destroy();
	m_explodeManager.destroy();
	m_prizeManager.destroy();
	m_pPlayer = NULL;
    m_pBoss = NULL;
    m_pScript->unloadScript();
}
void CGame::onOver()
{
	static int count=0;
	++count;
	if (count*m_updateTimeInterval >3000)
	{
		setGameState(GAME_MENU);
		count=0;
	}
}
void CGame::onWin()
{
	static int count=0;
	++count;
	if (count*m_updateTimeInterval >3000)
	{
		setGameState(GAME_MENU);
		count=0;
	}
}
void CGame::onMenuStart()
{
	setGameState(GAME_RUN);
	m_Menu.setVisible(false);
	if(m_pPlayer == NULL)
	{
		m_pPlayer=new Player("����һ��",3,1,1,m_nWidth/2,m_nHeight/2,10,10,500,100,50);
		m_plyerManager.add(m_pPlayer);

		if(!m_pScript->loadScript("res/nationfight2_1.script"))
		{
			g_log.write("���ؽű�ʧ�ܣ�");
		}
	}
}
void CGame::onMenuHelp()
{
	m_Help.setVisible(true);
}

void CGame::CheckHit()
{
	std::list<Object*>::iterator enbu_it=m_enBulletManager.lstObj.begin();
	std::list<Object*>::iterator enbu_itEnd=m_enBulletManager.lstObj.end();
	//��������л��ӵ���ײ
	int injure=0;
	for( ; enbu_it!=enbu_itEnd ; ++enbu_it)
	{
		Bullet *bu=(Bullet*)(*enbu_it);
		if(bu->state!=0 && m_pPlayer->state==1)//���״̬��0������1���2����״̬
		{	
			if(CheckHit(m_pPlayer,bu))
			{
				injure = g_pInjure[bu->showID] - int(m_pPlayer->defence*0.2);
				if(injure<0)
				{
					injure=10;
				}
				m_pPlayer->life -= injure;
				if(m_pPlayer->life<=0)//�������
				{
					m_pPlayer->state=0;
					m_pBackground->bShake=true;
					m_explodeManager.add(new Explose(1,m_pPlayer->x,m_pPlayer->y));
				}
				else
				{
					m_explodeManager.add(new Explose(2,bu->x,bu->y));
				}
				bu->state=0;
			}
		}
	}

	std::list<Object*>::iterator it=m_bulletManager.lstObj.begin();
	std::list<Object*>::iterator it_end=m_bulletManager.lstObj.end();
	std::list<Object*>::iterator en_it=m_enemyManager.lstObj.begin();
	std::list<Object*>::iterator en_it_end=m_enemyManager.lstObj.end();
	//���л�������ӵ���ײ
	for( ; it != it_end; ++it)
	{
		Bullet *bu=(Bullet*)(*it);
		for(en_it=m_enemyManager.lstObj.begin(); en_it!=en_it_end; ++en_it)
		{
			Enemy *en=(Enemy*)*en_it;
			if(en->state!=0 && CheckHit(bu,en))
			{
				injure=g_pInjure[bu->showID] - int(en->defence*0.2);
				if(injure < 0)
				{
					injure=10;
				}
				en->life -= injure;
				if(en->life<=0)//�л�����
				{
					en->state=0;
					m_pPlayer->AddScore(en->level*10);
					int id=0;//��ը����
					if(en->level >= 4)
					{
						id=1;
						m_pBackground->bShake=true;
					}
					m_explodeManager.add(new Explose(id,en->x,en->y));

					if(rand()%100 <= 70)
					{
						int d=rand()%10;
						if(d<=4)
                        {
                            d=0;
                        }
						else if(d<=6)
                        {
                            d=1;
                        }
						else if(d==7)
                        {
                            d=2;
                        }
						else if(d==8)
                        {
                            d=3;
                        }
						else
                        {
                            d=4;
                        }
						m_prizeManager.add(new Prize(d,en->x,en->y));
					}
					//m_prizeManager.add(new Prize(rand()%5,en->x,en->y));
				}
				else
				{
					m_explodeManager.add(new Explose(2,bu->x,bu->y));
				}
				bu->state=0;
			}
		}
	}
	//��������л���ײ
	for(en_it=m_enemyManager.lstObj.begin(); en_it!=en_it_end; ++en_it)
	{
		Enemy *en= (Enemy *)*en_it;
		if(m_pPlayer->state==1 && en->state!=0 && CheckHit(m_pPlayer,en))
		{
			injure=m_pPlayer->attack - en->defence;
			if(injure<0)
			{
				injure=10;
			}
			en->life-=injure;
			if(en->life<=0)//�л�����
			{
				en->state=0;
				int id=0;
				if(en->level > 4)
				{
					id=1;
					m_pBackground->bShake=true;
				}
				m_explodeManager.add(new Explose(id,en->x,en->y));
			}

			injure=en->attack - m_pPlayer->defence;
			if(injure < 0)
			{
				injure=10;
			}
			m_pPlayer->life -= injure;
			if(m_pPlayer->life <= 0)//�������
			{
				m_pPlayer->state=0;
				m_pBackground->bShake=true;
				m_explodeManager.add(new Explose(1,m_pPlayer->x,m_pPlayer->y));
			}
		}
	}

	std::list<Object*>::iterator pr_it=m_prizeManager.lstObj.begin();
	std::list<Object*>::iterator pr_it_end=m_prizeManager.lstObj.end();
	for( ; pr_it!=pr_it_end ; ++pr_it)
	{//�������뽱����ײ
		Prize *pr=(Prize *)*pr_it;
		if(pr->state!=0 && m_pPlayer->state!=0)
		{	
			if(CheckHit(m_pPlayer,pr))
			{
				switch(pr->showID)
				{
				case 0:
					m_pPlayer->AddScore(100);
					break;
				case 1:
					{
						++m_pPlayer->equipID;
						if (m_pPlayer->equipID>11)
						{
							m_pPlayer->equipID=11;
							m_pPlayer->AddLife(50);
						}
					}
					break;
				case 2:
					m_pPlayer->AddLife(100);
					break;
				case 3:
					{
						AddHero();//����Ӣ�ۻ�
					}
					break;
				case 4:
					{
						m_pPlayer->AddBomb(1);
					}
					break;
				}
				pr->state=0;
				m_pSound->play(9,false,true);
			}
		}
	}
}

bool CGame::CheckHit(Object *obj1,Object *obj2)
{
    CRect rc1 = obj1->getRect();
    CRect rc2 = obj2->getRect();

    //�����жϲ��ཻ
	if((rc1.right < rc2.left) 
		|| (rc1.left > rc2.right)
		|| (rc1.bottom < rc2.top) 
		|| (rc1.top > rc2.bottom))
	{
		return false;
	}

    //����ཻ����
    CRect rc;
    rc.left = max(rc1.left,rc2.left);
    rc.top = max(rc1.top,rc2.top);
    rc.right = min(rc1.right,rc2.right);
    rc.bottom = min(rc1.bottom,rc2.bottom);

    HDC hBackDC = m_dcDevice.getHdc();
    FillRect(hBackDC, &rc, Graphic::BlackBrush);
    obj1->show(hBackDC);
    obj2->show(hBackDC);
    for (int y=rc.top; y<=rc.bottom; ++y)
    {
        for (int x=rc.left; x<=rc.right; ++x)
        {
            if (GetPixel(hBackDC, x, y) != RGB(0, 0, 0))
            {
                return true;
            }
        }
    }
	return true;
}

void CGame::AddHero()//���Ӣ��
{

	if (g_game.m_plyerManager.getByName("left") == NULL)
	{
		g_game.m_plyerManager.add(new Player("left",2,1,1));
	}
	else if (g_game.m_plyerManager.getByName("right") == NULL)
	{
		g_game.m_plyerManager.add(new Player("right",2,1,1));
	}
	else
	{
		g_game.m_plyerManager.add(new Player("Ӣ��",2,1,1,
			rand()%g_game.m_nWidth,g_game.m_nHeight,
			rand()%10-5,-5));
	}
}
void CGame::AddBullet(int x,int y,int index)//����ӵ�
{
	if (index == 0)
	{//������
		int v=10;
		int dx=m_bulletBmp.GetAt(5)->nWidth+2;
		m_bulletManager.add(new Bullet(5,x,y,-v,-v));
		m_bulletManager.add(new Bullet(5,x,y,0,-v));
		m_bulletManager.add(new Bullet(5,x,y,v,-v));
	}
	else if (index == 1)
	{//һֱ�߷��ӵ�
		LBitmapInfo bmi=m_bulletBmp.GetAt(6);
		int dx=bmi->nWidth/bmi->nXFrame + 2;
		int v=7;
		m_enBulletManager.add(new Bullet(6,x-2*dx,y,0,v));
		m_enBulletManager.add(new Bullet(6,x-dx,y,0,v));
		m_enBulletManager.add(new Bullet(6,x,y,0,v));
		m_enBulletManager.add(new Bullet(6,x+dx,y,0,v));
		m_enBulletManager.add(new Bullet(6,x+2*dx,y,0,v));
	}
	else if(index == 2)
	{//һֱ�߷��ӵ�,ˮƽ��ɢ
		LBitmapInfo bmi=m_bulletBmp.GetAt(7);
		int dx=bmi->nWidth/bmi->nXFrame + 2;
		int v=4;
		m_enBulletManager.add(new Bullet(7,x-2*dx,y,-v*2,v));
		m_enBulletManager.add(new Bullet(7,x-dx,y,-v,v));
		m_enBulletManager.add(new Bullet(7,x,y,0,v));
		m_enBulletManager.add(new Bullet(7,x+dx,y,v,v));
		m_enBulletManager.add(new Bullet(7,x+2*dx,y,v*2,v));
	}
	else if(index == 3)
	{//һֱ�߷��ӵ�,��ֱ��ɢ
		int v=6;
		LBitmapInfo bmi=m_bulletBmp.GetAt(8);
		int dx=bmi->nWidth/bmi->nXFrame + 2;
		m_enBulletManager.add(new Bullet(8,x-2*dx,y,0,v+4));
		m_enBulletManager.add(new Bullet(8,x-dx,y,0,v+2));
		m_enBulletManager.add(new Bullet(8,x,y,0,v));
		m_enBulletManager.add(new Bullet(8,x+dx,y,0,v+2));
		m_enBulletManager.add(new Bullet(8,x+2*dx,y,0,v+4));
	}
	else if(index == 4)
	{//9�����ӵ�
		int v=5;
		m_enBulletManager.add(new Bullet(9,x,y,-v,-v));
		m_enBulletManager.add(new Bullet(9,x,y,0,-v));
		m_enBulletManager.add(new Bullet(9,x,y,v,-v));
		m_enBulletManager.add(new Bullet(9,x,y,-v,0));
		m_enBulletManager.add(new Bullet(9,x,y,v,0));
		m_enBulletManager.add(new Bullet(9,x,y,-v,v));
		m_enBulletManager.add(new Bullet(9,x,y,0,v));
		m_enBulletManager.add(new Bullet(9,x,y,v,v));
	}
}
//////////////////class Game end//////////////////////////////