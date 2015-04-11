#pragma once

#include <string.h>
#include "../G2DEngine/mlist.h"
#include "../G2DEngine/Utilities.h"
#include "../G2DEngine/DIBBitmap.h"

#ifndef VECTOR2D
#define VECTOR2D
//2维向量
struct Vector2D
{
    float x;
    float y;
    Vector2D(void) : x(0.0f),y(0.0f) {}
    Vector2D(float x_ , float y_) : x(x_),y(y_) {}
    Vector2D(const POINT &pt): x((float)pt.x),y((float)pt.y){}
    bool operator==(const Vector2D & v)
    {
        if(fabs(x-v.x)<0.001f && fabs(y-v.y)<0.001f)
        {
            return true;
        }
        else return false;
    }
    void toRect(int dx,int dy,RECT *rc)
    {
        rc->left = (int)x - dx;
        rc->top = (int)y - dy;
        rc->right = (int)x + dx;
        rc->bottom = (int)y + dy;
    }
};
#endif

struct ParticleAttr
{
    ParticleAttr()
    {
       memset(this,0,sizeof(ParticleAttr));
    }
    Vector2D    vPos;           //初始位置
    Vector2D    vSpeed;		    //速度
    Vector2D    vAcc;	        //加速度
    int         nLifeTime;		    //生命周期(ms)
    int         nAge;				//当前年龄(ms)
    COLORREF    crColor;			//粒子颜色
    COLORREF    crColorFade;		//消失时的颜色
    bool        bLive;				//存活状态
};

struct IParticle  : public IBaseObject
{
public:
    IParticle(void) : m_nMaxNum(0) {};
    virtual ~IParticle(void) { clear(); };
    virtual void update(int elipse) = 0;					//更新系统中的所有粒子
    virtual void render(HDC hDC) = 0;				        //渲染定点中的粒子
    virtual void resetParticle(ParticleAttr *attr) = 0;		//重置一个粒子的属性
    virtual void reset();									//重置所有的粒子属性
    virtual void addParticle();								//添加一个粒子
    virtual void removeDeadParticle();						//移除已经死亡的粒子
    virtual void clear() ;                                  //清空当前粒子
    bool isEmpty();											//当前系统无粒子则返回true,否则false;
    bool isDead();											//系统中的所有粒子都死亡则返回true,否则返回false;
    int  getRandomData(int bound1, int bound2);

protected:
    CMList<ParticleAttr> m_lstParticles;		//属性链表
    int     m_nMaxNum;							//给定时间内系统能存活的最大粒子数
};


class CBrushPanel;

class CExplode : public IParticle
{
public:
    CExplode();
    CExplode(int numParticles, Vector2D *pos,CBrushPanel*pBrush);
    void init(int numParticles, Vector2D *pos,CBrushPanel*pBrush);
    ~CExplode(void);
    void update(int elipse) ;					//更新系统中的所有粒子
    void render(HDC hDC) ;				        //渲染定点中的粒子
    void resetParticle(ParticleAttr *attr) ;	//重置一个粒子的属性

private:
    Vector2D m_vCreatePos;//初始位置
    CBrushPanel *m_pBrush;
};

class CSnow : public IParticle
{
public:
    CSnow();
    ~CSnow(void);
    void init(int numParticles, RECT *rect);
    void update(int elipse) ;					//更新系统中的所有粒子
    void render(HDC hDC) ;				        //渲染定点中的粒子
    void resetParticle(ParticleAttr *attr) ;	//重置一个粒子的属性

    inline void setRect(RECT *rc){ m_rect = *rc; }
private:
    CRect m_rect;
};



class CFire  : public IBaseObject
{
public:
    struct FireColor
    {
        BYTE b;//蓝
        BYTE g;//绿
        BYTE r;//红
        BYTE n;//保留
    };
public:
    CFire(void);
    ~CFire(void);
    bool init(HDC hDC,int w,int h);
    void update(int elipse);		//更新火焰粒子
    void render(HDC hDC,int x,int y);//渲染定点中的粒子
    void put();						//放置火焰种子
    void reset(void);               //重置火焰
    void clear();                   //清空系统

public://属性

    inline CDIBBitmap & getDibBmp(void){ return m_dibBmp; }
    inline HBITMAP getBitmap(void){ return m_dibBmp.getBitmap(); }
    //设置透明
    void setTransparent(bool t){ m_bTransparent = t; }
    bool getTransparent(void){ return m_bTransparent; }

    void setRedution(int r){ m_nRedution = r; }
    int getRedution(void ){ return m_nRedution; }
protected:
    int         m_nWidth;
    int         m_nHeight;
    FireColor   **m_ppFireData;
    CDIBBitmap  m_dibBmp;
    int         m_nRedution;//火焰衰减值
    bool        m_bTransparent;//是否透明处理
};