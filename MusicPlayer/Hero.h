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
    void moveX(bool positive);//ˮƽ�����ƶ�
    void moveY(bool positive) ;//��ֱ�����ƶ�
public://����

    void setPos(int x,int y){ m_ptPos = CPoint(x,y); }
    POINT * getPos(void){ return &m_ptPos; }

    void setSpeed(int v){ m_nSpeed = v; }

    int getID(void){ return m_nID; }
private:
    bool m_bMoving;//�Ƿ������˶�
};
