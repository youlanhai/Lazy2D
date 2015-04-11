//Game.h
#pragma once
#include "BitmapManager.h"
#include "Animation.h"
#include "Scene.h"
#include "Map.h"
#include "AStarSerach.h"
#include "Sprit.h"
#include "SpritManager.h"
#include "FightSystem.h"
#include "Speaker.h"

#include "App.h"

/**********************
��Ϸ״̬
**********************/
typedef enum
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
}GameState;

/****************************************************
��Ϸ��
��Ϸ��ʼ������Ϸ�����̵ȡ�
******************************************************/
class CGame : public CApp
{
public:
	CGame();						//���캯��
	~CGame();						//��������
	
	void Start();
	void Continue();
	void Exit();

protected:

    virtual bool onInit();//��ʼ����Ϸ
    virtual void onTick();		//����
    virtual void onDraw();		//��Ⱦ
    virtual void onClose();		//������Դ
    virtual bool onWindowMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    void onRun();
	void onRender();				//��Ϸѭ��

	void SetGameState(GameState state){gameState=state;}

    void LoopBGMusic(int index);
private:
	CScene			scene;
	CAStarSearch	pathSystem;
    CGameMap		mapSytem;
	CFightSystem	fightSystem;
	CSpritManager	spritManager;
	CAnimation		animation;
	CSpeaker		speaker;	//�Ի�ϵͳ
	CSprit			player;
	HFONT			hFont;//����
	CGameForm		menuForm;
	CBitmapManager	menuButtonBitmap;
	GameState	    gameState;			//��Ϸ״̬
};