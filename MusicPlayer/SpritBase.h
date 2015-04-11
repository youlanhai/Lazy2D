#pragma once
#include "..\g2dengine\utilities.h"
#include "..\g2dengine\BitmapManager.h"

//动画类型
enum AnimType
{
    AT_XNYN = 1,    //水平方向N帧，垂直方向N帧。水平绘制优先
    AT_YNXN,        //水平方向N帧，垂直方向N帧。垂直绘制优先
    AT_XN,          //单水平帧。
    AT_YN,          //单垂直帧。
    FORCE_DWORD = 0xffffffff
};

struct IAnimation : public IBaseObject
{
    IAnimation(void);
    void update(int nElapse,int param);
    void setBmpInfo(LBitmapInfo pInfo);

    AnimType m_nType;
    CPoint  m_ptPos;	//图片上的索引坐标,！！非物理坐标。
    int m_nElapseTime;  //已经流逝时间
    int m_nInterval;    //更新时间间隔
private:
    LBitmapInfo m_pBmpInfo;
};

struct IBaseSprit : public IBaseObject
{
    IBaseSprit(void);
    virtual ~IBaseSprit(void);
    virtual void update(int nElapse,RECT * rc) { }
    virtual void render(HDC hDC) { }
    virtual void clear(void) { }    //释放资源
    virtual void moveX(bool positive) { }//水平方向移动
    virtual void moveY(bool positive) { }//垂直方向移动

    int m_nID;					//唯一编号
    int m_nShowID;				//显示图片
    int m_nState;				//精灵状态
    char m_szName[128]; 		//名称
    int m_nDirection;           //当前运动方向。0下、1左、2右、3上。
    int m_nSpeed;				//移动速度
    int m_nWidth;               //精灵大小
    int m_nHeight;
    CPoint m_ptPos;			    //地图上的坐标
    int m_nLife;
    int m_nMaxLife;				//生命值
    int m_nMagic;
    int m_nMaxMagic;			//魔法值
    int m_nAttack;				//攻击
    int m_nDefence;				//防御
    int m_nExperence;			//经验
    int m_nLevel;				//级别
    IAnimation m_animation;     //图片动画

    static int	m_nIDCounter;		//ID计数器
    static CBitmapManager* m_pBmpManager;//位图管理器
    static void setBmpManager(CBitmapManager * pManager);
};
