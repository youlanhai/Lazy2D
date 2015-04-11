//CAStarSearch.cpp
#include "Commen.h"
#include "AStarSerach.h"
#include "Map.h"

//定义了上左右下4个方向
const int pDirection[4][2]={{-1,0},{0,-1},{0,1},{1,0}};

CAStarSearch::CAStarSearch()
{
	nXCount=0;
	nYCount=0;
	ppPath=NULL;
}
CAStarSearch::~CAStarSearch()
{
	Destroy();
}

//清空路径结构数据
void CAStarSearch::ClearPathData()	
{
	if(ppPath)
	{
		LPathNode p=NULL;
		for(int y=0;y<nYCount;++y)
			for(int x=0;x<nXCount;++x)
			{//其估价值清零
				p=&ppPath[y][x];
				p->nYIndex=y;
				p->nXIndex=x;
				p->nMark=0;
				p->nValueToStart=0;
				p->nTotalValue=0;
				p->pParent=0;
			}
	}
	open.clear();//清空open表中的数据	
}

//销毁路径结构
void CAStarSearch::Destroy()	
{
	if(ppPath)
	{
		for(int i=0;i<nYCount;++i)
			delete [] ppPath[i];
		delete [] ppPath;
		ppPath=NULL;
	}
	open.clear();
}
//创建路径结构，col为行数,row为列数
bool CAStarSearch::CreatePath(int nXCount,int nYCount)
{
	if(nXCount<=0 || nYCount<=0)
		return false;
	Destroy();
	this->nXCount=nXCount;
	this->nYCount=nYCount;
	//分配二维空间
	ppPath=new LPathNode[nYCount];
	for(int i=0;i<nYCount;++i)
	{
		ppPath[i]=new PathNode[nXCount];
		memset(ppPath[i],0,sizeof(PathNode)*nXCount);
	}
	return true;
}

/***************************
寻路函数
nStartXIndex,nStartYIndex,：起点的列下标、行下标。
nEndXindex,nEndYIndex：终点的列下标、行下标。
***************************/
LPathNode CAStarSearch::SearchPath
(
	int nStartXIndex,
	int nStartYIndex,
	int nEndXIndex,
	int nEndYIndex,
	CGameMap* mapSystem		//地图系统，用于检测可通行
)
{
	if(nStartXIndex<0 || nStartXIndex>=nXCount || nStartYIndex<0 || nStartYIndex>=nYCount)
		return NULL;
	ClearPathData();//清除上一次寻路留下的数据
	LPathNode pParent=&ppPath[nStartYIndex][nStartXIndex];//记录当前结点

	open.push_back(pParent);//将当前点加入open表
	ppPath[nStartYIndex][nStartXIndex].nMark=1;//=1表示将该结点放入了open表中

	std::list<LPathNode>::iterator it,itEnd,itTemp;
	while(!open.empty())
	{
		/////////////找出最小价值的点////////////////
		itTemp=open.begin();
		itEnd=open.end();
		for(it=itTemp;it!=itEnd;++it)
		{
			//让itTemp获得最小价值的点的指针
			if((*itTemp)->nTotalValue > (*it)->nTotalValue)
				itTemp=it;
		}
		pParent=*itTemp;	//让pParent获得最小估价的点的指针
		open.erase(itTemp);	//将该点从open表中删除
		pParent->nMark=2;	//=2表示已经访问过。
		///////////////////////////////////////////////
		if(pParent->nYIndex==nEndYIndex && pParent->nXIndex==nEndXIndex)
			return pParent;
		/////////////////对周围四个方格进行访问/////////
		for(int d=0;d<4;++d)
		{
			int i=pParent->nYIndex+pDirection[d][0];//行号
			int j=pParent->nXIndex+pDirection[d][1];//列号
			if(i<0 || i>=nYCount || j<0 || j>=nXCount)
				continue;
			
			LPathNode v=&ppPath[i][j];
			///////////在此添加地图障碍处理////////////
			if(mapSystem->IsSquareCanPass(j,i)==false)
				continue;
			///////////////////////////////////////////
			/********************************************
			对当前点到目的点的价值估算，
			估算方法：当前点到目的点相差方格个数
			*********************************************/  
			//nValueToEnd为当前点到目的点的估价值
			int nValueToEnd=abs(nEndYIndex-i)+abs(nEndXIndex-j);
			if(v->nMark==1 || v->nMark==2)//在open表或close表中，则根据权值进行修改
			{
				if(pParent->nValueToStart+1+nValueToEnd < v->nTotalValue)
				{//如果新路径的估价值小于旧有路径的估价值，则修改旧有的
					v->nValueToStart=pParent->nValueToStart+1;
					v->nTotalValue=v->nValueToStart+nValueToEnd;
					v->pParent=pParent;
				}
			}
			else if(v->nMark==0)//如果还未访问，则添加到open表中准备访问
			{
				v->nMark=1;
				v->nValueToStart=pParent->nValueToStart+1;
				v->nTotalValue=v->nValueToStart+nValueToEnd;
				v->pParent=pParent;
				open.push_back(v);
			}

		}
	}
	return NULL;
}