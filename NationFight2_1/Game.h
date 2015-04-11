//Game.h
#pragma once
#include "../G2DEngine/Commen.h"
#include "../G2DEngine/App.h"
#include "../G2DEngine/GameGUI.h"
#include "../G2DEngine/BitmapManager.h"
#include "../SoundEngine/DxMusic.h"
#include "../SoundEngine/DxSound.h"
#include "SpritManager.h"

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
class CDxSound;
class CDxMusic;
class CBackground;
class Player;
class Enemy;
class CNFScript;

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

    void onRegisterClass(WNDCLASSEX *pwc) override;	//ע�ᴰ����
    bool onInit() override;		//��ʼ����Ϸ
    void onDraw() override;			//��Ⱦ
    void onTick() override;			//����
    void onClose() override;			//������Դ
	//���ڹ���
	bool onWindowMessage(
		HWND hWnd,UINT msg,
		WPARAM wParam,LPARAM lParam) override;

	void renderText();

	void setGameState(GameState gs);

public:
	void AddHero();//���Ӣ��
	void AddBullet(int x,int y,int index);//����ӵ�
	void CheckHit();
	bool CheckHit(Object *obj1,Object *obj2);
public:
	HFONT		m_backFont;
	int			m_updateTimeInterval;	//����ʱ����
	int			m_updateCounter;		//���µ�֡��
	GameState	gameState;				//��Ϸ״̬
	CGameRect	m_clientRect;
    int         m_nMusicID;//�������֡�-1�����֡�

	CDxSound	*m_pSound;
    CDxMusic    *m_pMusic;
	CBackground	*m_pBackground;
	CNFScript	*m_pScript;

	Player		*m_pPlayer;
    Enemy       *m_pBoss;

	CUIPane		m_pane;
	CUIDialog	m_Menu;
	CUIDialog	m_Help;

	CSpritManager	m_plyerManager;
	CSpritManager	m_enemyManager;
	CSpritManager	m_bulletManager;
	CSpritManager	m_enBulletManager;//�����ӵ�
	CSpritManager	m_explodeManager;
	CSpritManager	m_prizeManager;

	CBitmapManager	m_heroBmp;
	CBitmapManager	m_enemyBmp;
	CBitmapManager  m_bulletBmp;
	CBitmapManager	m_backBmp;
	CBitmapManager	m_explodeBmp;
	CBitmapManager	m_prizeBmp;

public:
	void onMenu();
	void onRun();
	void onEnd();
	void onOver();
	void onWin();
	void onMenuStart();
	void onMenuHelp();
};