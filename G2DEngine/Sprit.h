//Sprit.h
#pragma once
#include "AStarSerach.h"
#include "Map.h"
#include "GameObject.h"

typedef unsigned long ulong;

/***************************
消息结构：用于物体之间的通信
***************************/
class  CMessage
{
public:
	int nMessage;	//消息内容
	int nID;		//发送者的ID
	int nXIndex;	//发送者的地图索引位置,列标
	int nYIndex;	//行标
	ulong ulParam;	//附加参数
	CMessage()//构造函数
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
	CMessage(const CMessage& msg)//拷贝构造函数
	{
		memcpy(this,&msg,sizeof(CMessage));
	}
	void AddCount(){++nCount;}		//增加消息的引用计数
	int GetCount(){return nCount;}	//获得引用计数
private:
	int nCount;//如果cout>=2时则将其删除
};


/**********************************
精灵类

**********************************/
class CSprit:public CGameObject
{
public:
	bool bAttack;		//标记是否被攻击
	CMessage currentTask;		//当前执行的任务
	std::list<PathNode*> path;//当前所要移动的路径
public:
	CSprit();
	~CSprit();
	void SetPosition(POINT* pt);//设定精灵坐标，一般情况下，尽可能的使用该函数设定坐标
	void SetPosition(int x,int y);
	void	AI(CGameObject* obj);						//智能
	CMessage SendMessage(int message,int x=-1,int y=-1,ulong param=0);//发送消息
	void	ProccessMessage();							//处理消息
	int		UpdateLevel();							//更新等级
	void	NothigthToDo();							//空闲
	int		Attack(CGameObject *obj);					//攻击
	int		CallFor(CGameObject* obj);					//召唤同伴
	int		Runaway(CGameObject *obj);					//逃跑
	int		SearchPath();							//寻路
	bool	Move(int dir);							//移动
	CGameObject*	CheckArround(int range=1);
public:
	//公共属性
	static std::vector<CMessage> commenMsgLst;	//公共消息仓库
	static CAStarSearch*		pathSystem;		//路径搜索系统
	static CGameMap*			mapSystem;		//地图系统
	//初始化公共组件
	static void InitializeCommenSystem(CAStarSearch* path,CGameMap* map);
	static void MessageManager();//消息管理器
};