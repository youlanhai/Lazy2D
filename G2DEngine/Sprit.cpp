//Sprit.cpp
#include "Commen.h"
#include "Sprit.h"

//定义四个方向
const int direction[4][2]={{-1,0},{0,-1},{0,1},{1,0}};

//初始化公共组件
std::vector<CMessage> CSprit::commenMsgLst=std::vector<CMessage>();//公共消息仓库
CAStarSearch*	CSprit::pathSystem=NULL;	//路径搜索系统
CGameMap*			CSprit::mapSystem=NULL;		//地图系统
void CSprit::InitializeCommenSystem(CAStarSearch* path,CGameMap* map)
{
	pathSystem=path;
	mapSystem=map;
}

CSprit::CSprit()
{
	bAttack=false;
	srand((unsigned)time(NULL));
}

CSprit::~CSprit()
{
	if(mapSystem)
		mapSystem->RemoveGameObject(this);//将自己所占据的位置释放	
}
void CSprit::SetPosition(POINT* pt)
{
	mapSystem->RemoveGameObject(this);
	ptPosition.x=pt->x;
	ptPosition.y=pt->y;
	mapSystem->AddGameObject(this);
}
void CSprit::SetPosition(int x,int y)
{
	POINT pt;
	pt.x=x;
	pt.y=y;
	SetPosition(&pt);
}
//精灵10种状态：0死亡，1空闲，2警戒，3攻击，4执行请求，5执行求助，
//6执行命令，7恐惧，8逃跑，9生命危险
//4种消息：0没事可做，1危险警告，2请求，3求助，4命令
void CSprit::AI(CGameObject* obj)
{
	if(nLife<=0)
		nState=0;
	if(nState==0)
		return;
	if(nState<9)//非生命危险状态
	{
		ProccessMessage();
		int srcI=ptPosition.y/mapSystem->nSquareHeight;//计算当前所处地图位置的索引
		int srcJ=ptPosition.x/mapSystem->nSquareWidth;
		int destI=currentTask.nYIndex;//计算目标点的地图索引
		int destJ=currentTask.nXIndex;
		int di=abs(srcI-destI);//当前点与目标点的差值
		int dj=abs(srcJ-destJ);
		switch(currentTask.nMessage)//当前任务
		{
		case 0://没事可做
		case 1://危险警告
			NothigthToDo();//巡逻
		/*	obj=CheckArround(actionW);
			if(obj)
			{//发现玩家，则转为攻击状态
				SendMessage(2,obj->x,obj->y);
				nState=3;
			}
			*/
			break;
		case 2://请求
		case 3://求助
		case 4://命令
			{
				if(path.empty() && (di>4 || dj>4))
				{//还未到达目标就无路可走，则重新搜索。
					SearchPath();
				}
				else if(di<=4 && dj<=4)
				{
					
					actionRect.w=8;
					actionRect.h=8;
					actionRect.x=currentTask.nXIndex;
					actionRect.y=currentTask.nYIndex;
					nState=1;
					currentTask=CMessage(0,0,0,0,0);
				}
				NothigthToDo();

			}
			break;
		default:
			break;
		}
		if(nState==3 || nState==7)
		{
			if(rand()%100-50>0)
				SendMessage(2);//召唤
		}
		if(nLife < 0.3*nMaxLife)
		{//有生命危险
			nState=9;
		}
	}
	else if(nState>=9)
	{
		if(rand()%100-20>0)//80%的几率能发出求助
		{
			//发送求助消息
			SendMessage(3);
		}
	}
}
//发送消息
CMessage CSprit::SendMessage(int message,int x,int y,ulong param)
{
	CMessage ms;
	if(x<0 || y<0)
	{	
		x=ptPosition.x;
		y=ptPosition.y;
	}
	y=y/mapSystem->nSquareHeight;
	x=x/mapSystem->nSquareWidth;
	ms=CMessage(message,nID,x,y,param);
	CSprit::commenMsgLst.push_back(ms);
	return ms;
}
//处理消息
void CSprit::ProccessMessage()
{
//精灵10种状态：0死亡，1空闲，2警戒，3攻击，4执行请求，5执行求助，
//6执行命令，7恐惧，8逃跑，9生命危险
//4种消息：1危险警告，2请求，3求助，4命令
	std::vector<CMessage>::iterator it,itEnd=commenMsgLst.end(),itTemp;
	it=commenMsgLst.begin();
	CMessage message;
	while(it!=itEnd)
	{
		itTemp=it++;
		message=*itTemp;
		if(nState==0 && message.nID==nID)
		{//如果自己死亡了，则撤销自己发送的消息
			if(message.nMessage<=4)//如果不是撤销消息，则发送撤销消息
				SendMessage(message.nMessage+4);
			continue;
		}
		if(currentTask.nMessage==message.nMessage || message.nID==nID)//相同的消息
			continue;
		switch(message.nMessage)
		{
		case 1://危险警告消息
			{
				if(nState==1)//空闲状态
				{
					nState=2;//警告状态
					actionRect.w=15;
					actionRect.h=15;
					currentTask=message;
				}
			}
			break;
		case 2://请求消息
			{
				if((nState==1 && rand()%100-30>0)||(nState==2 && rand()%100-50>0))
				{
					CMessage m=currentTask;//记录旧有任务
					currentTask=message;
					if(SearchPath())//判断可达
					{
						nState=4;//转为执行请求状态
					}
					else
					{
						currentTask=m;//如果任务不能完成，则执行旧任务
					}
				}
			}
			break;
		case 3://求助消息
			{
				if(nState<3 || (nState==3 && nLife>0.5*nMaxLife) || (nState==4 && rand()%100-50>0))
				{
					CMessage m=currentTask;
					currentTask=message;
					if(SearchPath())
					{
						nState=5;
					}
					else
					{
						currentTask=m;
					}
				}
			}
			break;
		case 4://命令消息
			{
				if(nState<8 && rand()%100-20>0)//80%的几率对求助作出响应
				{
					CMessage m=currentTask;
					currentTask=message;
					if(SearchPath())//可到达目标
					{
						nState=6;//转为执行命令状态
					}
					else
					{
						currentTask=m;
					}
				}
			}
			break;
		case 5://解除警告
			if(nState==2 || currentTask.nID==message.nID)
			{
				nState=1;
				path.clear();
				actionRect.w=8;
				actionRect.h=8;
				actionRect.x=ptPosition.x/mapSystem->nXCount;
				actionRect.y=ptPosition.y/mapSystem->nYCount;
				currentTask=CMessage(0,0,0,0,0);
			}
			break;
		case 6://解除请求
			if(nState==4 || currentTask.nID==message.nID)
			{
				nState=1;
				path.clear();
				actionRect.w=8;
				actionRect.h=8;
				actionRect.x=ptPosition.x/mapSystem->nXCount;
				actionRect.y=ptPosition.y/mapSystem->nYCount;
				currentTask=CMessage(0,0,0,0,0);
			}
			break;
		case 7://解除求助
			if(nState==5 || currentTask.nID==message.nID)
			{
				nState=1;
				path.clear();
				actionRect.w=8;
				actionRect.h=8;
				actionRect.x=ptPosition.x/mapSystem->nXCount;
				actionRect.y=ptPosition.y/mapSystem->nYCount;
				currentTask=CMessage(0,0,0,0,0);
			}
			break;
		case 8://解除命令
			if(nState==6 || currentTask.nID==message.nID)//处于正在执行要解除的命令的状态
			{
				nState=1;//转为空闲状态
				path.clear();//将旧有路径信息清除
				actionRect.w=8;
				actionRect.h=8;
				actionRect.x=ptPosition.x/mapSystem->nXCount;
				actionRect.y=ptPosition.y/mapSystem->nYCount;
				currentTask=CMessage(0,0,0,0,0);//任务清空
			}
			break;
		default:
			break;
		}
	}
}


void CSprit::NothigthToDo()
{
	int x,y;
	PathNode* pNode=NULL;
	if(path.empty())
	{
		//获得地图上的一个范围内的随机位置的索引
		
		x=rand()%(2*actionRect.w)-actionRect.w+actionRect.x;
		y=rand()%(2*actionRect.h)-actionRect.h+actionRect.y;
		if(mapSystem->IsSquareCanPass(x,y)==false )//不能通过
			return;
		currentTask.nXIndex=x;
		currentTask.nYIndex=y;
		if(!SearchPath())
			return;//位置不可达
	}
	int d=-1;
	x=ptPosition.x/mapSystem->nSquareWidth;//计算当前点的地图索引
	y=ptPosition.y/mapSystem->nSquareHeight;
	pNode=path.front();//获得路径链表第一个元素
	///////////////////////////////////
	if(pNode)
	{
		if(x == pNode->nXIndex && y > pNode->nYIndex)//up
			d=0;
		else if(x == pNode->nXIndex && y < pNode->nYIndex)//down
			d=3;		
		else if(y == pNode->nYIndex && x > pNode->nXIndex)//left
			d=1;
		else if(y == pNode->nYIndex && x < pNode->nXIndex)//right
			d=2;
		else
			path.pop_front();
	}
	if(!Move(d))//遇上了障碍物或路径信息丢失或路径已经走完
		path.clear();
}

int CSprit::SearchPath()//寻路
{
	//获得当前位置的地图索引
	int y=ptPosition.y/mapSystem->nSquareHeight;
	int x=ptPosition.x/mapSystem->nSquareWidth;
	//寻路
	mapSystem->RemoveGameObject(this);//先将自己的位置释放，然后再寻路
	PathNode* pn=pathSystem->SearchPath(currentTask.nXIndex,currentTask.nYIndex,x,y,mapSystem);
	mapSystem->AddGameObject(this);
	//将搜索路径加入到精灵的路径当中
	if(pn)
	{
		path.clear();
		while(pn)
		{
			path.push_back(pn);
			pn=pn->pParent;
		}
		return 1;
	}
	return 0;
}
bool CSprit::Move(int dir)//移动
{
	bool bMoveSuccess=true;
	if(this->nDirection==dir)
	{//跟上次移动的方向相同
		if(++ptPositionInPixture.x >=4)
			ptPositionInPixture.x=0;
	}
	else if(dir>=0 && dir<4)//跟上次移动的方向不同
	{
		ptPositionInPixture.x=0;//显示动作图片的第一张
		this->nDirection=dir;
	}
	else//没有移动，则修正动作误差
	{
		if(ptPositionInPixture.x==1)
			ptPositionInPixture.x=2;
		else if(ptPositionInPixture.x==3)
			ptPositionInPixture.x=0;
	}
	//必须先将自己先前所占据的位置释放，然后移动，移动完成后再将自己的信息添加到地图中。
	mapSystem->RemoveGameObject(this);
	{
		if(dir==0)//向上移动
		{
			ptPositionInPixture.y=3;
			ptPosition.y-=nSpeed;
			
			if(ptPosition.y<0 || !mapSystem->IsThereCanPass(&ptPosition))
			{//遇到了障碍,则撤销移动
				ptPosition.y+=nSpeed;
				bMoveSuccess=false;
			}
		}
		else if(dir==1)//向左移动
		{
			ptPositionInPixture.y=1;
			ptPosition.x-=nSpeed;
			
			if(ptPosition.x<0 || !mapSystem->IsThereCanPass(&ptPosition))
			{//遇到了障碍,则撤销移动
				ptPosition.x+=nSpeed;
				bMoveSuccess=false;
			}
		}
		else if(dir==2)//向右移动
		{
			ptPositionInPixture.y=2;
			ptPosition.x+=nSpeed;
			
			if(!mapSystem->IsThereCanPass(&ptPosition))
			{//遇到了障碍,则撤销移动
				ptPosition.x-=nSpeed;
				bMoveSuccess=false;
			}
		}
		else if(dir==3)//向下移动
		{
			ptPositionInPixture.y=0;
			ptPosition.y+=nSpeed;
			
			if(!mapSystem->IsThereCanPass(&ptPosition))
			{//遇到了障碍,则撤销移动
				ptPosition.y-=nSpeed;
				bMoveSuccess=false;
			}
		}
	}
	mapSystem->AddGameObject(this);//将自己的信息添加到地图中。
	return bMoveSuccess;
}
CGameObject* CSprit::CheckArround(int range)
{
	int y=ptPosition.y/mapSystem->nSquareHeight;
	int x=ptPosition.x/mapSystem->nSquareWidth;
	for(int m=y-range;m<y+range;++m)
		for(int n=x-range;n<x+range;++n)
		{
			CGameObject* obj=mapSystem->GetGameObject(n,m);
			if(obj && obj->nID!=nID)//发现精灵
				return obj;
		}
	return NULL;
}
int CSprit::UpdateLevel()//更新等级
{
	if(nExperence>(nLevel+1)*(nLevel+1)*10)
	{
		++nLevel;
		nAttack+=nLevel*10;
		nDefence+=nLevel*5;
		nMaxLife+=nLevel*20;
		nMaxMagic+=nLevel*10;
		nLife=nMaxLife;
		nMagic=nMaxMagic;
		return nLevel;
	}
	return 0;
}
int CSprit::Attack(CGameObject *obj)//攻击
{
	return 0;
}
int CSprit::CallFor(CGameObject* obj)//召唤同伴
{
	//发送请求消息
	SendMessage(1);
	return 0;
}
int CSprit::Runaway(CGameObject *obj)//逃跑
{
	return 0;
}

void CSprit::MessageManager()//消息管理器
{
	std::vector<CMessage>::iterator it,itEnd=commenMsgLst.end(),itTemp;
	it=commenMsgLst.begin();
	while(it!=itEnd)
	{
		itTemp=it++;
		if(itTemp->GetCount()>=1)
		{
			commenMsgLst.erase(itTemp);
		}
		else
			itTemp->AddCount();
	}
}