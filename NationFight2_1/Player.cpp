//Player.cpp
#include "../G2DEngine/Commen.h"
#include "Object.h"
#include "Enemy.h"
#include "Player.h"
#include "game.h"

extern CGame g_game;

Player::Player()
{
	score=0;
}

Player::Player(char *name,int showID,int level,int equipID,
			   int x,int y,int vx,int vy,int life,
			   int attack,int defence)
{
	score=0;
	Init(name,showID,level,equipID,x,y,vx,vy,life,attack,defence);
}

void Player::Init(char *name,int showID,int level,int equipID,
		  int x,int y,int vx,int vy,int life,
		int attack,int defence)
{
	if(name!=NULL)
	{
		strcpy(this->name,name);
	}
	this->showID=showID;
	this->x=x;
	this->y=y;
	this->vx=vx;
	this->vy=vy;
	this->life=life;
	this->attack=attack;
	this->defence=defence;
	this->level=level;
	this->equipID=equipID;

	LBitmapInfo bmi=g_game.m_heroBmp.GetAt(showID);
	w=bmi->nWidth/bmi->nXFrame;
	h=bmi->nHeight/bmi->nYFrame;

	bomb=3;
}
void Player::show(HDC hDC)
{
	if(state==0)
	{
		return;
	}
	HDC memDC=CreateCompatibleDC(hDC);
	SelectObject(memDC,g_game.m_heroBmp.GetAt(showID)->hBitmap);
	TransparentBlt(hDC,x,y,w,h,memDC,picX,0,w,h,GetPixel(memDC,0,0));
	DeleteDC(memDC);
}
void Player::moveLook(bool positive)
{
	if (positive)
	{
		y -= vy;
	}
	else
	{
		y += vy;
	}
}
void Player::moveRight(bool positive)
{
	if(positive)
	{
		x += vx;
	}
	else
	{
		x -= vx;
	}
}
void Player::AI(Object *obj,RECT* rc)
{
	frame++;
	if(state == 0)
	{
		return;
	}
	
	if (obj != this)//辅助英雄机
	{

		if (frame>1000)
		{
			state=0;
			return ;
		}
		if (strcmp(name,"left")==0)
		{
			x=obj->x - w;
			y=obj->y + (obj->h - h);
		}
		else if (strcmp(name,"right")==0)
		{
			x=obj->x + obj->w;
			y=obj->y + (obj->h - h);
		}
		else
		{
			if(x < rc->left-w/2)
			{
				x=rc->left-w/2;
				vx=-vx;
			}
			if(x > rc->right-w/2)
			{
				x=rc->right-w/2;
				vx=-vx;
			}
			if(y < rc->top)
			{
				y=rc->top;
				vy=-vy;
			}
			if(y > rc->bottom-h)
			{
				y=rc->bottom-h;
				vy=-vy;
			}
			x += vx;
			y += vy;
		}
		if (frame%4==0 && frame%20!=0)
		{
			LBitmapInfo bmi=g_game.m_bulletBmp.GetAt(equipID);
			g_game.m_bulletManager.add(new Bullet(equipID,
				x+(w-(bmi->nWidth/bmi->nXFrame))/2,y,0,-(abs(vy)+2)));
		}
	}
	else//英雄机
	{
		if(x < rc->left-w/2)
		{
			x=rc->left-w/2;
		}
		if(x > rc->right-w/2)
		{
			x=rc->right-w/2;
		}
		if(y < rc->top)
		{
			y=rc->top;
		}
		if(y > rc->bottom-h)
		{
			y=rc->bottom-h;
		}
		/////////////更新分数//////////////////////////
		int nextLevelScore=(level*level*level+4)*100;
		if (score >= nextLevelScore)//升级啦！！！
		{
            g_game.m_pSound->play(17);

			++level;
			++bomb;
			attack += level*50;
			defence += level*20;
			max_life += level*10;//生命值只能通过吃奖子得到
		}
	}

	/////////////更新显示图片///////////////////////
	LBitmapInfo bmi=g_game.m_heroBmp.GetAt(showID);
	if(++frameID >= bmi->nXFrame)
	{
		frameID=0;
	}
	picX=frameID*w;
}
