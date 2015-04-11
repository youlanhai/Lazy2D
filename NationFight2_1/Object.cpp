//Object.cpp
#include "../G2DEngine/Commen.h"
#include "Object.h"

int Object::nIDCounter=0;

Object::Object()
{
	nID = ++nIDCounter;
	strcpy(name,"null");	//����
	showID=0;				//��ʾͼƬ��id
	equipID=0;				//װ�����
	state=1;				//״̬
	x=y=0;					//λ��
	w=h=0;					//��Ⱥ͸߶�
	vx=vy=5;				//x,y�����ϵ��ٶ�
	life=max_life=500;		//����
	attack=20;				//������
	defence=10;				//������
	level=1;				//�ȼ�

	picX=picY=0;			//ͼƬ�ϵ�����
	frameID=0;				//��̬ͼƬ�ڼ�֡
	frame=0;				//��ʾ��֡��
	oldTime=0;
}
#if 0
void Object::SetShowID(int index,int id)
{
	LBitmapInfo bmi;
	showID=id;
	if(index==0)
	{
		bmi=g_game.m_heroBmp.GetAt(showID);
	}
	else if(index==1)
	{
		bmi=g_game.m_enemyBmp.GetAt(showID);
	}
	else if(index==2)
	{
		bmi=g_game.m_bulletBmp.GetAt(showID);
	}
	else if(index==3)
	{
		bmi=g_game.m_explodeBmp.GetAt(showID);
	}
	else
	{
		bmi=g_game.m_prizeBmp.GetAt(showID);
	}
	w=bmi->nWidth/bmi->nXFrame;
	h=bmi->nHeight/bmi->nYFrame;
}
#endif