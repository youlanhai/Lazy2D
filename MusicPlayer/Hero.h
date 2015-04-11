#pragma once
#include "spritbase.h"

class CHero : public IBaseSprit
{
public:
    CHero(void);
    ~CHero(void);

    void init(int nShow);
    void update(int nElapse,RECT * rc) ;
    void render(HDC hDC) ;
    void moveX(bool positive);//水平方向移动
    void moveY(bool positive) ;//垂直方向移动
public://属性

    void setPos(int x,int y){ m_ptPos = CPoint(x,y); }
    POINT * getPos(void){ return &m_ptPos; }

    void setSpeed(int v){ m_nSpeed = v; }

    int getID(void){ return m_nID; }
private:
    bool m_bMoving;//是否正在运动
};
