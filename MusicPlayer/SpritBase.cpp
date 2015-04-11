#include <windows.h>
#include "SpritBase.h"


//////////////////////////////////////////////////////////////////////////

IAnimation::IAnimation(void)
{
    m_nType = AT_XNYN;
    m_nElapseTime = 0;    //上次更新时间
    m_nInterval = 100;    //更新时间间隔
    m_pBmpInfo = NULL;
}

void IAnimation::update(int nElapse,int param)
{
    m_nElapseTime += nElapse;
    if (m_nElapseTime < m_nInterval)
    {
        return ;
    }
    else
    {
        m_nElapseTime = 0;
    }

    if (NULL == m_pBmpInfo)
    {
        return ;
    }

    if (m_nType == AT_XNYN)//要求param为方向。0下、1左、2右、3上。
    {
        if(m_ptPos.y != param)//方向有更改
        {
            m_ptPos.y = param ;
            m_ptPos.x = 0;
        }
        else
        {
            ++m_ptPos.x ;
            if (m_ptPos.x >= m_pBmpInfo->nXFrame)
            {
                m_ptPos.x = 0;
            }
        }
    }
    else if (m_nType == AT_YNXN)//要求param为方向
    {
        if(m_ptPos.x != param)//方向有更改
        {
            m_ptPos.x = param ;
            m_ptPos.y = 0;
        }
        else
        {
            ++m_ptPos.y ;
            if (m_ptPos.y >= m_pBmpInfo->nYFrame)
            {
                m_ptPos.y = 0;
            }
        }
    }
    else if (m_nType == AT_XN)
    {
        ++m_ptPos.x ;
        if (m_ptPos.x >= m_pBmpInfo->nXFrame)
        {
            m_ptPos.x = 0;
        }
    }
    else if (m_nType == AT_YN)
    {
        ++m_ptPos.y ;
        if (m_ptPos.y >= m_pBmpInfo->nYFrame)
        {
            m_ptPos.y = 0;
        }
    }
}

void IAnimation::setBmpInfo(LBitmapInfo pInfo)
{
    m_pBmpInfo = pInfo;
    m_ptPos = CPoint(0,0);
    m_nElapseTime = 0;
}

//////////////////////////////////////////////////////////////////////////
int	IBaseSprit::m_nIDCounter = 0;		//ID计数器
CBitmapManager* IBaseSprit::m_pBmpManager = NULL;//位图管理器

IBaseSprit::IBaseSprit(void)
{
    m_nID = ++m_nIDCounter;
}

IBaseSprit::~IBaseSprit(void)
{
    clear();
}

void IBaseSprit::setBmpManager(CBitmapManager * pManager)
{
    m_pBmpManager = pManager;
}