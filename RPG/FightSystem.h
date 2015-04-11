//FightSystem.h
#pragma once
#include "Effect.h"
#include "MagicSystem.h"
#include "GameMenu.h"
#include "GameObject.h"
#include "GameButton.h"
#include "GameForm.h"
/*
class CGameObject;
class CEffect;
class CMagicSystem;
class CGameMenu;
*/
typedef enum EnumFightMode
{
	FM_ATTACK,	//����
	FM_DEFENCE,	//����
	FM_MAGIC,	//����
	FM_GOODS,	//��Ʒ
	FM_ESCAPE,	//����
	FM_NONE		//û��ѡ��
}FightMode;//ս��ģʽ

/***************************
ս����
***************************/
class CFightSystem
{
public:
	CFightSystem();
	~CFightSystem();
	bool Initialize//��ʼ��
	(
		HWND hWnd,
		HINSTANCE hInstance
	);
	void SetFighter(CGameObject* player,CGameObject* enemy);	//����սʿ
	bool SetBackGroundBitmap(std::string strFileName);			//���ñ���ͼƬ
	bool SetFighterHeadBitmap(std::string strFileName);			//����ͷ��ͼƬ��
	bool SetFightFormBitmap(std::string strFileName);			//����ս������ͼƬ��
	bool SetGoodsFormBitmap(std::string strFileName);			//������Ʒ����ͼƬ��
	bool SetUIBitmap(std::string strFileName);
	bool SetMagicList(std::string strFileName);					//����ħ���б�
	void SetFightMode(FightMode fmMode){fmFightMode=fmMode;}	//����ս��ģʽ
	void SetBackGroundRect(int x,int y,int width,int height);
	int FightLoop(HDC hDC);			//ս��ѭ��
	void ChoiceFightMode(HDC hDC);	//ѡ��ս��ģʽ
	void ChoiceMagic();				//ѡ��ħ��
	int Attack();					//����
	int Deffence();					//����
	int Magic();					//ħ��
	int UseGoods();					//ʹ����Ʒ
	int Escape();					//����
	void DrawBackGround(HDC hDC);	//���Ʊ���
	void DrawFighterState(HDC hDC);	//���ƾ���״̬
	void DrawFighterState2(HDC hDC,int x,int y,CGameObject* pSprit);

    bool IsFighting() const { return bFighting; }
    int GetResult() const { return fightResult; }
private:
	HWND		hWindowHandle;		//���ھ��
	HINSTANCE	hInstance;			//ʵ�����
	int			nRound;				//�غϼ���
	bool		bPlayerDecision;	//��Ҿ���
	bool		bFighting;			//ս��������
	CGameRect	rcBackGround;		//��������
	CMagicSystem magicSystem;		//ħ��ϵͳ
	CBitmapManager bmpBackground;	//����ͼƬ��
	CBitmapManager bmpFighterHead;	//����ս��ͷ��
	CGameObject* pPlayer;			//���
	CGameObject* pEnemy;			//����
	CGameRect	rcPlayer;
	CGameRect	rcEnemy;
	char szShowPlayerInformation[256];
	char szShowEnemyInformation[256];
	CEffect		playerEffect;		//��Ҷ�����Ч
	CEffect		enemyEffect;		//���˶�����Ч
	CEffect		magicEffect;		//ħ����Ч
	bool		bSpritShowEnd;		//���鶯����Ч����
	bool		bMagicShowEnd;		//ħ����Ч����
	FightMode	fmFightMode;		//��ս��ʽ
	int			nUseMagicID;		//ħ����ʽ
	CGameForm	fightForm;			//ѡ��ս��ģʽ����
	CGameForm   goodsForm;			//ѡ��ħ�������崰��
	CBitmapManager	fightBitmap;	//ѡ��ս��ģʽ����ͼƬ��
	CBitmapManager	goodsBitmap;	//ѡ��ħ�������崰��ͼƬ��
	CBitmapManager	uiBitmap;		//UIͼƬ
    int             fightResult;
};
