//Game.h
#pragma once
#include "../G2DEngine/App.h"
#include "../G2DEngine/GameGUI.h"
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

/****************************************************
��Ϸ��
��Ϸ��ʼ������Ϸ�����̵ȡ�
******************************************************/
class CGame : public CApp
{
public:
	CGame();						//���캯��
	~CGame();						//��������
	//���ڹ���
	static LRESULT CALLBACK windowProc(
		HWND hWnd,UINT msg,
		WPARAM wParam,LPARAM lParam);
	void registerClass();	//ע�ᴰ����
	bool initGame();		//��ʼ����Ϸ
	bool initUI();
	void render();			//��Ⱦ
	void renderText();
	void renderUI();
	void update();			//����
	void destroy();			//������Դ

public:
	GameState	gameState;			//��Ϸ״̬
	CGameRect	m_clientRect;

	HFONT		m_backFont;
	CFPS		*m_pFPS;
	CKeyboard	*m_pKeybord;

	CBitmapManager m_bmpManager;
	CUIDialog	m_dialog;
	CUIPane		m_pane;
};