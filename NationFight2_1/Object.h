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
	char name[20];//����
	int showID;	//��ʾͼƬ��id
	int equipID;//װ�����
	int state;	//״̬
	int x;
	int y;		//λ��
	int w;
	int h;		//��Ⱥ͸߶�
	int vx;
	int vy;		//x,y�����ϵ��ٶ�
	int life;	//����
	int max_life;
	int attack;	//������
	int defence;//������
	int level;	//�ȼ�

protected:
	int picX;	//ͼƬ�ϵ�����
	int picY;
	int frameID;//��̬ͼƬ�ڼ�֡
	int frame;	//��ʾ��֡��
	int oldTime;

	int nID;	//����ID
	static int nIDCounter;//ID������
};


inline CRect Object::getRect(void)
{
    return CRect(x,y,x+w,y+h);
}