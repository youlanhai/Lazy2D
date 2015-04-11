//Object.cpp
#include "../G2DEngine/Commen.h"
#include "Object.h"

int Object::nIDCounter=0;

Object::Object()
{
	nID = ++nIDCounter;
	strcpy(name,"null");	//名字
	showID=0;				//显示图片的id
	equipID=0;				//装备编号
	state=1;				//状态
	x=y=0;					//位置
	w=h=0;					//宽度和高度
	vx=vy=5;				//x,y方向上的速度
	life=max_life=500;		//生命
	attack=20;				//攻击力
	defence=10;				//防御力
	level=1;				//等级

	picX=picY=0;			//图片上的坐标
	frameID=0;				//动态图片第几帧
	frame=0;				//显示的帧数
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