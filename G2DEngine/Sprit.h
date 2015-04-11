//Sprit.h
#pragma once
#include "AStarSerach.h"
#include "Map.h"
#include "GameObject.h"

typedef unsigned long ulong;

/***************************
��Ϣ�ṹ����������֮���ͨ��
***************************/
class  CMessage
{
public:
	int nMessage;	//��Ϣ����
	int nID;		//�����ߵ�ID
	int nXIndex;	//�����ߵĵ�ͼ����λ��,�б�
	int nYIndex;	//�б�
	ulong ulParam;	//���Ӳ���
	CMessage()//���캯��
	{
		nMessage=nID=0;
		nXIndex=nYIndex=0;
		nCount=0;
	}
	CMessage(int message,int id,int xIndex,int yIndex,ulong param=0)
	{
		nMessage=message;
		nID=id;
		nXIndex=xIndex;
		nYIndex=yIndex;
		ulParam=param;
		nCount=0;
	}
	CMessage(const CMessage& msg)//�������캯��
	{
		memcpy(this,&msg,sizeof(CMessage));
	}
	void AddCount(){++nCount;}		//������Ϣ�����ü���
	int GetCount(){return nCount;}	//������ü���
private:
	int nCount;//���cout>=2ʱ����ɾ��
};


/**********************************
������

**********************************/
class CSprit:public CGameObject
{
public:
	bool bAttack;		//����Ƿ񱻹���
	CMessage currentTask;		//��ǰִ�е�����
	std::list<PathNode*> path;//��ǰ��Ҫ�ƶ���·��
public:
	CSprit();
	~CSprit();
	void SetPosition(POINT* pt);//�趨�������꣬һ������£������ܵ�ʹ�øú����趨����
	void SetPosition(int x,int y);
	void	AI(CGameObject* obj);						//����
	CMessage SendMessage(int message,int x=-1,int y=-1,ulong param=0);//������Ϣ
	void	ProccessMessage();							//������Ϣ
	int		UpdateLevel();							//���µȼ�
	void	NothigthToDo();							//����
	int		Attack(CGameObject *obj);					//����
	int		CallFor(CGameObject* obj);					//�ٻ�ͬ��
	int		Runaway(CGameObject *obj);					//����
	int		SearchPath();							//Ѱ·
	bool	Move(int dir);							//�ƶ�
	CGameObject*	CheckArround(int range=1);
public:
	//��������
	static std::vector<CMessage> commenMsgLst;	//������Ϣ�ֿ�
	static CAStarSearch*		pathSystem;		//·������ϵͳ
	static CGameMap*			mapSystem;		//��ͼϵͳ
	//��ʼ���������
	static void InitializeCommenSystem(CAStarSearch* path,CGameMap* map);
	static void MessageManager();//��Ϣ������
};