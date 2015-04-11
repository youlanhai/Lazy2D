//GameObject.h
#pragma once

class CBitmapManager;

class CGameObject
{
public:
	int nID;					//Ψһ���
	int nShowID;				//���������
	int nState;					//����״̬
	std::string strName;		//����
	int nSpeed;					//�ƶ��ٶ�
	int nWidth;
	int nHeight;				//��С
	int nDirection;				//����
	POINT ptPosition;			//��ͼ�ϵ�����
	POINT ptOldPosition;		//�ڵ�ͼ�Ͼ���λ�õ�����
	POINT ptPositionInPixture;	//ͼƬ�ϵ���������
	CGameRect actionRect;		//�����
	int nLife;
	int nMaxLife;				//����ֵ
	int nMagic;
	int nMaxMagic;				//ħ��ֵ
	int nAttack;				//����
	int nDefence;				//����
	int nExperence;				//����
	int nLevel;					//����
public:
	CGameObject();
	~CGameObject();
	void Initialize
	(
		int kind,std::string name,
		int state=1,int speed=8,
		int life=100,int magic=50,
		int attack=20,int defence=10,
		int level=1
	);
	virtual void Show(HDC hBackDC);
	static void InitializeCommen(HINSTANCE hInstance,std::string strBmpListFileName);
	static int	nIDCounter;		//ID������
	static CBitmapManager bmpManager;		//λͼ������
};