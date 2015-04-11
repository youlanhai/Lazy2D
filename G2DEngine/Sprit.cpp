//Sprit.cpp
#include "Commen.h"
#include "Sprit.h"

//�����ĸ�����
const int direction[4][2]={{-1,0},{0,-1},{0,1},{1,0}};

//��ʼ���������
std::vector<CMessage> CSprit::commenMsgLst=std::vector<CMessage>();//������Ϣ�ֿ�
CAStarSearch*	CSprit::pathSystem=NULL;	//·������ϵͳ
CGameMap*			CSprit::mapSystem=NULL;		//��ͼϵͳ
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
		mapSystem->RemoveGameObject(this);//���Լ���ռ�ݵ�λ���ͷ�	
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
//����10��״̬��0������1���У�2���䣬3������4ִ������5ִ��������
//6ִ�����7�־壬8���ܣ�9����Σ��
//4����Ϣ��0û�¿�����1Σ�վ��棬2����3������4����
void CSprit::AI(CGameObject* obj)
{
	if(nLife<=0)
		nState=0;
	if(nState==0)
		return;
	if(nState<9)//������Σ��״̬
	{
		ProccessMessage();
		int srcI=ptPosition.y/mapSystem->nSquareHeight;//���㵱ǰ������ͼλ�õ�����
		int srcJ=ptPosition.x/mapSystem->nSquareWidth;
		int destI=currentTask.nYIndex;//����Ŀ���ĵ�ͼ����
		int destJ=currentTask.nXIndex;
		int di=abs(srcI-destI);//��ǰ����Ŀ���Ĳ�ֵ
		int dj=abs(srcJ-destJ);
		switch(currentTask.nMessage)//��ǰ����
		{
		case 0://û�¿���
		case 1://Σ�վ���
			NothigthToDo();//Ѳ��
		/*	obj=CheckArround(actionW);
			if(obj)
			{//������ң���תΪ����״̬
				SendMessage(2,obj->x,obj->y);
				nState=3;
			}
			*/
			break;
		case 2://����
		case 3://����
		case 4://����
			{
				if(path.empty() && (di>4 || dj>4))
				{//��δ����Ŀ�����·���ߣ�������������
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
				SendMessage(2);//�ٻ�
		}
		if(nLife < 0.3*nMaxLife)
		{//������Σ��
			nState=9;
		}
	}
	else if(nState>=9)
	{
		if(rand()%100-20>0)//80%�ļ����ܷ�������
		{
			//����������Ϣ
			SendMessage(3);
		}
	}
}
//������Ϣ
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
//������Ϣ
void CSprit::ProccessMessage()
{
//����10��״̬��0������1���У�2���䣬3������4ִ������5ִ��������
//6ִ�����7�־壬8���ܣ�9����Σ��
//4����Ϣ��1Σ�վ��棬2����3������4����
	std::vector<CMessage>::iterator it,itEnd=commenMsgLst.end(),itTemp;
	it=commenMsgLst.begin();
	CMessage message;
	while(it!=itEnd)
	{
		itTemp=it++;
		message=*itTemp;
		if(nState==0 && message.nID==nID)
		{//����Լ������ˣ������Լ����͵���Ϣ
			if(message.nMessage<=4)//������ǳ�����Ϣ�����ͳ�����Ϣ
				SendMessage(message.nMessage+4);
			continue;
		}
		if(currentTask.nMessage==message.nMessage || message.nID==nID)//��ͬ����Ϣ
			continue;
		switch(message.nMessage)
		{
		case 1://Σ�վ�����Ϣ
			{
				if(nState==1)//����״̬
				{
					nState=2;//����״̬
					actionRect.w=15;
					actionRect.h=15;
					currentTask=message;
				}
			}
			break;
		case 2://������Ϣ
			{
				if((nState==1 && rand()%100-30>0)||(nState==2 && rand()%100-50>0))
				{
					CMessage m=currentTask;//��¼��������
					currentTask=message;
					if(SearchPath())//�жϿɴ�
					{
						nState=4;//תΪִ������״̬
					}
					else
					{
						currentTask=m;//�����������ɣ���ִ�о�����
					}
				}
			}
			break;
		case 3://������Ϣ
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
		case 4://������Ϣ
			{
				if(nState<8 && rand()%100-20>0)//80%�ļ��ʶ�����������Ӧ
				{
					CMessage m=currentTask;
					currentTask=message;
					if(SearchPath())//�ɵ���Ŀ��
					{
						nState=6;//תΪִ������״̬
					}
					else
					{
						currentTask=m;
					}
				}
			}
			break;
		case 5://�������
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
		case 6://�������
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
		case 7://�������
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
		case 8://�������
			if(nState==6 || currentTask.nID==message.nID)//��������ִ��Ҫ����������״̬
			{
				nState=1;//תΪ����״̬
				path.clear();//������·����Ϣ���
				actionRect.w=8;
				actionRect.h=8;
				actionRect.x=ptPosition.x/mapSystem->nXCount;
				actionRect.y=ptPosition.y/mapSystem->nYCount;
				currentTask=CMessage(0,0,0,0,0);//�������
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
		//��õ�ͼ�ϵ�һ����Χ�ڵ����λ�õ�����
		
		x=rand()%(2*actionRect.w)-actionRect.w+actionRect.x;
		y=rand()%(2*actionRect.h)-actionRect.h+actionRect.y;
		if(mapSystem->IsSquareCanPass(x,y)==false )//����ͨ��
			return;
		currentTask.nXIndex=x;
		currentTask.nYIndex=y;
		if(!SearchPath())
			return;//λ�ò��ɴ�
	}
	int d=-1;
	x=ptPosition.x/mapSystem->nSquareWidth;//���㵱ǰ��ĵ�ͼ����
	y=ptPosition.y/mapSystem->nSquareHeight;
	pNode=path.front();//���·�������һ��Ԫ��
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
	if(!Move(d))//�������ϰ����·����Ϣ��ʧ��·���Ѿ�����
		path.clear();
}

int CSprit::SearchPath()//Ѱ·
{
	//��õ�ǰλ�õĵ�ͼ����
	int y=ptPosition.y/mapSystem->nSquareHeight;
	int x=ptPosition.x/mapSystem->nSquareWidth;
	//Ѱ·
	mapSystem->RemoveGameObject(this);//�Ƚ��Լ���λ���ͷţ�Ȼ����Ѱ·
	PathNode* pn=pathSystem->SearchPath(currentTask.nXIndex,currentTask.nYIndex,x,y,mapSystem);
	mapSystem->AddGameObject(this);
	//������·�����뵽�����·������
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
bool CSprit::Move(int dir)//�ƶ�
{
	bool bMoveSuccess=true;
	if(this->nDirection==dir)
	{//���ϴ��ƶ��ķ�����ͬ
		if(++ptPositionInPixture.x >=4)
			ptPositionInPixture.x=0;
	}
	else if(dir>=0 && dir<4)//���ϴ��ƶ��ķ���ͬ
	{
		ptPositionInPixture.x=0;//��ʾ����ͼƬ�ĵ�һ��
		this->nDirection=dir;
	}
	else//û���ƶ����������������
	{
		if(ptPositionInPixture.x==1)
			ptPositionInPixture.x=2;
		else if(ptPositionInPixture.x==3)
			ptPositionInPixture.x=0;
	}
	//�����Ƚ��Լ���ǰ��ռ�ݵ�λ���ͷţ�Ȼ���ƶ����ƶ���ɺ��ٽ��Լ�����Ϣ��ӵ���ͼ�С�
	mapSystem->RemoveGameObject(this);
	{
		if(dir==0)//�����ƶ�
		{
			ptPositionInPixture.y=3;
			ptPosition.y-=nSpeed;
			
			if(ptPosition.y<0 || !mapSystem->IsThereCanPass(&ptPosition))
			{//�������ϰ�,�����ƶ�
				ptPosition.y+=nSpeed;
				bMoveSuccess=false;
			}
		}
		else if(dir==1)//�����ƶ�
		{
			ptPositionInPixture.y=1;
			ptPosition.x-=nSpeed;
			
			if(ptPosition.x<0 || !mapSystem->IsThereCanPass(&ptPosition))
			{//�������ϰ�,�����ƶ�
				ptPosition.x+=nSpeed;
				bMoveSuccess=false;
			}
		}
		else if(dir==2)//�����ƶ�
		{
			ptPositionInPixture.y=2;
			ptPosition.x+=nSpeed;
			
			if(!mapSystem->IsThereCanPass(&ptPosition))
			{//�������ϰ�,�����ƶ�
				ptPosition.x-=nSpeed;
				bMoveSuccess=false;
			}
		}
		else if(dir==3)//�����ƶ�
		{
			ptPositionInPixture.y=0;
			ptPosition.y+=nSpeed;
			
			if(!mapSystem->IsThereCanPass(&ptPosition))
			{//�������ϰ�,�����ƶ�
				ptPosition.y-=nSpeed;
				bMoveSuccess=false;
			}
		}
	}
	mapSystem->AddGameObject(this);//���Լ�����Ϣ��ӵ���ͼ�С�
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
			if(obj && obj->nID!=nID)//���־���
				return obj;
		}
	return NULL;
}
int CSprit::UpdateLevel()//���µȼ�
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
int CSprit::Attack(CGameObject *obj)//����
{
	return 0;
}
int CSprit::CallFor(CGameObject* obj)//�ٻ�ͬ��
{
	//����������Ϣ
	SendMessage(1);
	return 0;
}
int CSprit::Runaway(CGameObject *obj)//����
{
	return 0;
}

void CSprit::MessageManager()//��Ϣ������
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