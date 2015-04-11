//Object.h
#pragma once

class Object
{
public:
	Object();
	virtual void AI(Object *obj,RECT* rc){};
	virtual void show(HDC hDC){}
	inline int getID(void){ return nID; }
    inline CRect getRect(void);
public:
	char name[20];//名字
	int showID;	//显示图片的id
	int equipID;//装备编号
	int state;	//状态
	int x;
	int y;		//位置
	int w;
	int h;		//宽度和高度
	int vx;
	int vy;		//x,y方向上的速度
	int life;	//生命
	int max_life;
	int attack;	//攻击力
	int defence;//防御力
	int level;	//等级

protected:
	int picX;	//图片上的坐标
	int picY;
	int frameID;//动态图片第几帧
	int frame;	//显示的帧数
	int oldTime;

	int nID;	//物体ID
	static int nIDCounter;//ID计数器
};


inline CRect Object::getRect(void)
{
    return CRect(x,y,x+w,y+h);
}