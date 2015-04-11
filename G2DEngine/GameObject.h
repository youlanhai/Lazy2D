//GameObject.h
#pragma once

class CBitmapManager;

class CGameObject
{
public:
	int nID;					//唯一编号
	int nShowID;				//精灵的类型
	int nState;					//精灵状态
	std::string strName;		//名称
	int nSpeed;					//移动速度
	int nWidth;
	int nHeight;				//大小
	int nDirection;				//方向
	POINT ptPosition;			//地图上的坐标
	POINT ptOldPosition;		//在地图上旧有位置的索引
	POINT ptPositionInPixture;	//图片上的索引坐标
	CGameRect actionRect;		//活动区域
	int nLife;
	int nMaxLife;				//生命值
	int nMagic;
	int nMaxMagic;				//魔法值
	int nAttack;				//攻击
	int nDefence;				//防御
	int nExperence;				//经验
	int nLevel;					//级别
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
	static int	nIDCounter;		//ID计数器
	static CBitmapManager bmpManager;		//位图管理器
};