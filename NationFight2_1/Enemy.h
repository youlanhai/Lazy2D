//Enemy.h
#pragma once

#include "Object.h"

class Prize:public Object
{
public:
	Prize();
	Prize(int showID,int x=0,int y=0,int vx=3,int vy=2);
	void Init(int showID=0,int x=0,int y=0,int vx=3,int vy=2);
	void AI(Object *obj,RECT* rc);
	void show(HDC hDC);
};

class Explose:public Object
{
public:
	Explose();
	Explose(int showID,int x=0,int y=0);
	void Init(int showID=0,int x=0,int y=0);
	void AI(Object *obj,RECT* rc);
	void show(HDC hDC);
};

class Bullet:public Object
{
public:
	Bullet();
	Bullet(int showID, int x=0,int y=0,int vx=5,int vy=5, int attack=20);
	void Init(int showID=0, int x=0,int y=0,int vx=5,int vy=5, int attack=20);
	void AI(Object *obj,RECT* rc);
	void show(HDC hDC);
};


class Enemy:public Object
{
public:
	Enemy();
	Enemy(int showID,int level=0,
		int x=0,int y=0,int vx=5,int vy=5,
		int life=100, int attack=20,int defence=10);
	void Init(int showID=0,int level=0,
		int x=0,int y=0,int vx=5,int vy=5,
		int life=100, int attack=20,int defence=10);
	void AI(Object *obj,RECT* rc);
	void show(HDC hDC);
};
