//CAStarSearch.cpp
#include "Commen.h"
#include "AStarSerach.h"
#include "Map.h"

//��������������4������
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

//���·���ṹ����
void CAStarSearch::ClearPathData()	
{
	if(ppPath)
	{
		LPathNode p=NULL;
		for(int y=0;y<nYCount;++y)
			for(int x=0;x<nXCount;++x)
			{//�����ֵ����
				p=&ppPath[y][x];
				p->nYIndex=y;
				p->nXIndex=x;
				p->nMark=0;
				p->nValueToStart=0;
				p->nTotalValue=0;
				p->pParent=0;
			}
	}
	open.clear();//���open���е�����	
}

//����·���ṹ
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
//����·���ṹ��colΪ����,rowΪ����
bool CAStarSearch::CreatePath(int nXCount,int nYCount)
{
	if(nXCount<=0 || nYCount<=0)
		return false;
	Destroy();
	this->nXCount=nXCount;
	this->nYCount=nYCount;
	//�����ά�ռ�
	ppPath=new LPathNode[nYCount];
	for(int i=0;i<nYCount;++i)
	{
		ppPath[i]=new PathNode[nXCount];
		memset(ppPath[i],0,sizeof(PathNode)*nXCount);
	}
	return true;
}

/***************************
Ѱ·����
nStartXIndex,nStartYIndex,���������±ꡢ���±ꡣ
nEndXindex,nEndYIndex���յ�����±ꡢ���±ꡣ
***************************/
LPathNode CAStarSearch::SearchPath
(
	int nStartXIndex,
	int nStartYIndex,
	int nEndXIndex,
	int nEndYIndex,
	CGameMap* mapSystem		//��ͼϵͳ�����ڼ���ͨ��
)
{
	if(nStartXIndex<0 || nStartXIndex>=nXCount || nStartYIndex<0 || nStartYIndex>=nYCount)
		return NULL;
	ClearPathData();//�����һ��Ѱ·���µ�����
	LPathNode pParent=&ppPath[nStartYIndex][nStartXIndex];//��¼��ǰ���

	open.push_back(pParent);//����ǰ�����open��
	ppPath[nStartYIndex][nStartXIndex].nMark=1;//=1��ʾ���ý�������open����

	std::list<LPathNode>::iterator it,itEnd,itTemp;
	while(!open.empty())
	{
		/////////////�ҳ���С��ֵ�ĵ�////////////////
		itTemp=open.begin();
		itEnd=open.end();
		for(it=itTemp;it!=itEnd;++it)
		{
			//��itTemp�����С��ֵ�ĵ��ָ��
			if((*itTemp)->nTotalValue > (*it)->nTotalValue)
				itTemp=it;
		}
		pParent=*itTemp;	//��pParent�����С���۵ĵ��ָ��
		open.erase(itTemp);	//���õ��open����ɾ��
		pParent->nMark=2;	//=2��ʾ�Ѿ����ʹ���
		///////////////////////////////////////////////
		if(pParent->nYIndex==nEndYIndex && pParent->nXIndex==nEndXIndex)
			return pParent;
		/////////////////����Χ�ĸ�������з���/////////
		for(int d=0;d<4;++d)
		{
			int i=pParent->nYIndex+pDirection[d][0];//�к�
			int j=pParent->nXIndex+pDirection[d][1];//�к�
			if(i<0 || i>=nYCount || j<0 || j>=nXCount)
				continue;
			
			LPathNode v=&ppPath[i][j];
			///////////�ڴ���ӵ�ͼ�ϰ�����////////////
			if(mapSystem->IsSquareCanPass(j,i)==false)
				continue;
			///////////////////////////////////////////
			/********************************************
			�Ե�ǰ�㵽Ŀ�ĵ�ļ�ֵ���㣬
			���㷽������ǰ�㵽Ŀ�ĵ��������
			*********************************************/  
			//nValueToEndΪ��ǰ�㵽Ŀ�ĵ�Ĺ���ֵ
			int nValueToEnd=abs(nEndYIndex-i)+abs(nEndXIndex-j);
			if(v->nMark==1 || v->nMark==2)//��open���close���У������Ȩֵ�����޸�
			{
				if(pParent->nValueToStart+1+nValueToEnd < v->nTotalValue)
				{//�����·���Ĺ���ֵС�ھ���·���Ĺ���ֵ�����޸ľ��е�
					v->nValueToStart=pParent->nValueToStart+1;
					v->nTotalValue=v->nValueToStart+nValueToEnd;
					v->pParent=pParent;
				}
			}
			else if(v->nMark==0)//�����δ���ʣ�����ӵ�open����׼������
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