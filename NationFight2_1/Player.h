//Player.h
#pragma once
#include "Object.h"

class Player:public Object
{
public:
	Player();
	Player(char *name,int showID=0,int level=0,int equipID=0,
		int x=0,int y=0,int vx=5,int vy=5,int life=100,
		int attack=20,int defence=10);
	void Init(char *name,int showID=0,int level=0,int equipID=0,
		int x=0,int y=0,int vx=5,int vy=5,int life=100,
		int attack=20,int defence=10);
	void show(HDC hDC);
	void AI(Object *obj,RECT* rc);
	inline void AddBomb(int n){ bomb+=n; }
	inline void AddScore(int n){ score+=n; }
	inline void AddLife(int n){ if((life+=n)>max_life)life=max_life; }
	void moveLook(bool positive);
	void moveRight(bool positive);

	int score;
	int bomb;
};