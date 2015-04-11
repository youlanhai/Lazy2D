//Enemy.cpp
#include "../G2DEngine/Commen.h"
#include "../SoundEngine/DxSound.h"
#include "Enemy.h"
#include "Game.h"

#include <cmath>

extern CGame g_game;
const int bulletSound[14]={1,1,1,1,2,3,3,3,4,4,4,5,5,5};
const int equip[]={0,2,3,5,6,7,8,9,10,13};
//////////////class Prize/////////////////
Prize::Prize()
{
	
}
Prize::Prize(int showID,int x,int y,int vx,int vy)
{
	Init(showID,x,y,vx,vy);
}
void Prize::Init(int showID,int x,int y,int vx,int vy)
{
	this->showID=showID;
	this->x=x;
	this->y=y;
	this->vx=vx;
	this->vy=vy;

	LBitmapInfo bmi=g_game.m_prizeBmp.GetAt(showID);
	w=bmi->nWidth/bmi->nXFrame;
	h=bmi->nHeight/bmi->nYFrame;
}
void Prize::AI(Object *obj,RECT* rc)
{
	frame++;
	x+=vx;
	y+=vy;
	if(state==0)
		return;
	if(y<rc->top || y>rc->bottom)
	{
		state=0;
		return;
	}
	if(x<rc->left)
	{
		x=0;
		vx=-vx;
	}
	if(x>rc->right-w)
	{
		x=rc->right-w;
		vx=-vx;
	}
	
	LBitmapInfo bmi=g_game.m_prizeBmp.GetAt(showID);
	if(showID==0 || showID==3 ||showID==4)
	{		
		if(++frameID >= bmi->nXFrame*bmi->nYFrame)
		{
			frameID=0;
		}
		picX=(frameID%bmi->nXFrame)*w;
		picY=(frameID/bmi->nXFrame)*h;
	}
	else
	{
		if (frame%40 ==0)
		{
			if(++frameID >= bmi->nXFrame)
			{
				frameID=0;
			}
			picX=frameID*w;
			state=-state;
		}
	}
}
void Prize::show(HDC hDC)
{
	if(state == 0)
	{
		return;
	}
	LBitmapInfo bmi=g_game.m_prizeBmp.GetAt(showID);
	HDC memDC=CreateCompatibleDC(hDC);
	SelectObject(memDC,bmi->hBitmap);
	TransparentBlt(hDC,x,y,w,h,memDC,picX,picY,w,h,GetPixel(memDC,0,0));
	DeleteDC(memDC);
}

//////////////class Explose///////////////
Explose::Explose()
{
}
Explose::Explose(int showID,int x,int y)
{
	Init(showID,x,y);
}
void Explose::Init(int showID,int x,int y)
{
	this->showID=showID;
	this->level=level; 
	this->x=x;
	this->y=y;

	LBitmapInfo bmi=g_game.m_explodeBmp.GetAt(showID);
	w=bmi->nWidth/bmi->nXFrame;
	h=bmi->nHeight/bmi->nYFrame;

	if(showID==0)		
	{
		g_game.m_pSound->play(7);
	}
	else if(showID==1)
	{
		g_game.m_pSound->play(8);
	}
}
void Explose::AI(Object *obj,RECT* rc)
{
	frame++;
	LBitmapInfo bmi=g_game.m_explodeBmp.GetAt(showID);
	
	if(++frameID >= bmi->nXFrame)
	{
		state=0;
		return;
	}
	picX=frameID*w;
}
void Explose::show(HDC hDC)
{
	LBitmapInfo bmi=g_game.m_explodeBmp.GetAt(showID);
	HDC memDC=CreateCompatibleDC(hDC);
	SelectObject(memDC,bmi->hBitmap);
	TransparentBlt(hDC,x,y,w,h,memDC,picX,0,w,h,GetPixel(memDC,0,0));
	DeleteDC(memDC);
}
///////////////class Bullet/////////////
Bullet::Bullet()
{
}
Bullet::Bullet(int showID,int x,int y,int vx,int vy,int attack)
{
	Init(showID,x,y,vx,vy,attack);
}
void Bullet::Init(int showID,int x,int y,int vx,int vy,int attack)
{
	this->showID=showID;
	this->x=x;
	this->y=y;
	this->vx=vx;
	this->vy=vy;
	this->attack=attack;
	
	LBitmapInfo bmi=g_game.m_bulletBmp.GetAt(showID);
	w=bmi->nWidth/bmi->nXFrame;
	h=bmi->nHeight/bmi->nYFrame;
}
void Bullet::AI(Object *obj,RECT* rc)
{
	if(state == 0)
	{
		return;
	}
	frame++;
	if (obj != NULL )
	{
		if ( y <= obj->y)
		{
			if (x < obj->x)
			{
				vx=abs(vx);
			}
			else
			{
				vx=-abs(vx);
			}
		}
	}
	x+=vx;
	y+=vy;
	if(x<rc->left || x>rc->right || y<rc->top || y>rc->bottom)
	{
		state=0;
		return;
	}
	
	LBitmapInfo bmi=g_game.m_bulletBmp.GetAt(showID);
	if(++frameID >= bmi->nXFrame)
	{
		frameID=0;
	}
	picX=frameID*w;	
}
void Bullet::show(HDC hDC)
{
	if(state == 0)
	{
		return;
	}
	HDC memDC=CreateCompatibleDC(hDC);
	SelectObject(memDC,g_game.m_bulletBmp.GetAt(showID)->hBitmap);
	TransparentBlt(hDC,x,y,w,h,memDC,picX,0,w,h,GetPixel(memDC,0,0));
	DeleteDC(memDC);
}
//////////////class Enemy///////////////
Enemy::Enemy()
{

}
Enemy::Enemy(int showID,int level,
			 int x,int y,int vx,int vy,
			 int life,int attack,int defence)
{
	Init(showID,level,x,y,vx,vy,life,attack,defence);
}


void Enemy::Init(int showID,int level,
				 int x,int y,int vx,int vy,
				 int life,int attack,int defence)
{
	this->showID=showID;
	this->level=level;
	this->x=x;
	this->y=y;
	this->vx=vx;
	this->vy=vy;
	this->life=life;
	this->attack=attack;
	this->defence=defence;

    if (showID == 9)//boss
    {
        strcpy_s(name,20,"boss");
    }
	
	LBitmapInfo bmi=g_game.m_enemyBmp.GetAt(showID);
	w=bmi->nWidth/bmi->nXFrame;
	h=bmi->nHeight/bmi->nYFrame;
}

void Enemy::AI(Object *obj,RECT* rc)
{
	frame++;
	if(state==0)
	{
		return;
	}
	if (obj != NULL )
	{
		if ( y <= obj->y)
		{
			if (x < obj->x)
			{
				vx=abs(vx);
			}
			else
			{
				vx=-abs(vx);
			}
		}
	}

	x+=vx;
	y+=vy;
	if(y > rc->bottom-h)
	{
		if (showID == 9)//BOSS
		{
			y = rc->bottom-h;
			vy =- vy;
		}
		else
		{
			state=0;
			return;
		}
	}
	if (showID == 9)//BOSS)
	{
		if (y < -h)
		{
			y=-h;
			vy = -vy;
		}
	}
	if(x<0)
	{
		x=0;
		vx=-vx;
	}
	else if(x>rc->right-w)
	{
		x=rc->right-w;
		vx=-vx;
	}
	
	LBitmapInfo bmi=g_game.m_enemyBmp.GetAt(showID);
	if(++frameID >= bmi->nXFrame)
	{
		frameID=0;
	}
	picX=frameID*w;
	
	if(frame%15==0 && rand()%100<70)
	{
		if (showID>=5 && showID<=8)
		{
			g_game.AddBullet(x+w/2,y+h/2,showID-4);
			g_game.m_pSound->play(5);
		}
		else
		{
			int id=equip[showID];
			Bullet* bu=new Bullet;
			bmi=g_game.m_bulletBmp.GetAt(id);
			bu->Init(id,x+(w-bmi->nWidth/bmi->nXFrame)/2,y+h,rand()%10-5,vy+rand()%7+2);
			g_game.m_enBulletManager.add(bu);
		}
	}

}
void Enemy::show(HDC hDC)
{
	if(state==0)
	{
		return;
	}
	HDC memDC=CreateCompatibleDC(hDC);
	SelectObject(memDC,g_game.m_enemyBmp.GetAt(showID)->hBitmap);
	TransparentBlt(hDC,x,y,w,h,memDC,picX,0,w,h,GetPixel(memDC,0,0));
	DeleteDC(memDC);
}