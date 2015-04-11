#pragma once

#include <string.h>
#include "../G2DEngine/mlist.h"
#include "../G2DEngine/Utilities.h"
#include "../G2DEngine/DIBBitmap.h"

#ifndef VECTOR2D
#define VECTOR2D
//2ά����
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
    Vector2D    vPos;           //��ʼλ��
    Vector2D    vSpeed;		    //�ٶ�
    Vector2D    vAcc;	        //���ٶ�
    int         nLifeTime;		    //��������(ms)
    int         nAge;				//��ǰ����(ms)
    COLORREF    crColor;			//������ɫ
    COLORREF    crColorFade;		//��ʧʱ����ɫ
    bool        bLive;				//���״̬
};

struct IParticle  : public IBaseObject
{
public:
    IParticle(void) : m_nMaxNum(0) {};
    virtual ~IParticle(void) { clear(); };
    virtual void update(int elipse) = 0;					//����ϵͳ�е���������
    virtual void render(HDC hDC) = 0;				        //��Ⱦ�����е�����
    virtual void resetParticle(ParticleAttr *attr) = 0;		//����һ�����ӵ�����
    virtual void reset();									//�������е���������
    virtual void addParticle();								//���һ������
    virtual void removeDeadParticle();						//�Ƴ��Ѿ�����������
    virtual void clear() ;                                  //��յ�ǰ����
    bool isEmpty();											//��ǰϵͳ�������򷵻�true,����false;
    bool isDead();											//ϵͳ�е��������Ӷ������򷵻�true,���򷵻�false;
    int  getRandomData(int bound1, int bound2);

protected:
    CMList<ParticleAttr> m_lstParticles;		//��������
    int     m_nMaxNum;							//����ʱ����ϵͳ�ܴ������������
};


class CBrushPanel;

class CExplode : public IParticle
{
public:
    CExplode();
    CExplode(int numParticles, Vector2D *pos,CBrushPanel*pBrush);
    void init(int numParticles, Vector2D *pos,CBrushPanel*pBrush);
    ~CExplode(void);
    void update(int elipse) ;					//����ϵͳ�е���������
    void render(HDC hDC) ;				        //��Ⱦ�����е�����
    void resetParticle(ParticleAttr *attr) ;	//����һ�����ӵ�����

private:
    Vector2D m_vCreatePos;//��ʼλ��
    CBrushPanel *m_pBrush;
};

class CSnow : public IParticle
{
public:
    CSnow();
    ~CSnow(void);
    void init(int numParticles, RECT *rect);
    void update(int elipse) ;					//����ϵͳ�е���������
    void render(HDC hDC) ;				        //��Ⱦ�����е�����
    void resetParticle(ParticleAttr *attr) ;	//����һ�����ӵ�����

    inline void setRect(RECT *rc){ m_rect = *rc; }
private:
    CRect m_rect;
};



class CFire  : public IBaseObject
{
public:
    struct FireColor
    {
        BYTE b;//��
        BYTE g;//��
        BYTE r;//��
        BYTE n;//����
    };
public:
    CFire(void);
    ~CFire(void);
    bool init(HDC hDC,int w,int h);
    void update(int elipse);		//���»�������
    void render(HDC hDC,int x,int y);//��Ⱦ�����е�����
    void put();						//���û�������
    void reset(void);               //���û���
    void clear();                   //���ϵͳ

public://����

    inline CDIBBitmap & getDibBmp(void){ return m_dibBmp; }
    inline HBITMAP getBitmap(void){ return m_dibBmp.getBitmap(); }
    //����͸��
    void setTransparent(bool t){ m_bTransparent = t; }
    bool getTransparent(void){ return m_bTransparent; }

    void setRedution(int r){ m_nRedution = r; }
    int getRedution(void ){ return m_nRedution; }
protected:
    int         m_nWidth;
    int         m_nHeight;
    FireColor   **m_ppFireData;
    CDIBBitmap  m_dibBmp;
    int         m_nRedution;//����˥��ֵ
    bool        m_bTransparent;//�Ƿ�͸������
};