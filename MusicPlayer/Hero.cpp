#include "..\G2DEngine\Commen.h"
#include "Hero.h"

CHero::CHero(void)
{
    m_bMoving = false;
}

CHero::~CHero(void)
{
}

void CHero::init(int nShow)
{
    m_nShowID = nShow;
    if (NULL == m_pBmpManager)
    {
        g_log.write("初始化CHero对象出错，m_pBmpManager不能为空！请先初始化位图资源。");
    }
    else
    {
        LBitmapInfo pbi = m_pBmpManager->at(m_nShowID);
        m_animation.setBmpInfo(pbi);
        m_animation.m_nInterval = 50;

        m_nWidth = pbi->nWidth/pbi->nXFrame;
        m_nHeight = pbi->nHeight/pbi->nYFrame;
    }
}

void CHero::update(int nElapse,RECT * rc)
{
    if (m_bMoving || (m_animation.m_ptPos.x%2)!=0)
    {
        m_animation.update(nElapse,m_nDirection);
        int dv = int(m_nSpeed * nElapse/1000.0f);
        switch (m_nDirection)
        {
        case 0:
            m_ptPos.y += dv;
        	break;
        case 1:
            m_ptPos.x -= dv;
            break;
        case 2:
            m_ptPos.x += dv;
            break;
        case 3:
            m_ptPos.y -= dv;
            break;
        default:break;
        }
        if (m_ptPos.x < rc->left)
        {
            m_ptPos.x = rc->left;
        }
        else if (m_ptPos.x > rc->right-m_nWidth)
        {
            m_ptPos.x = rc->right-m_nWidth;
        }
        if (m_ptPos.y < rc->top)
        {
            m_ptPos.y = rc->top;
        }
        else if (m_ptPos.y > rc->bottom-m_nHeight)
        {
            m_ptPos.y = rc->bottom-m_nHeight;
        }
    }

    m_bMoving = false;
}

void CHero::render(HDC hDC)
{
    if (NULL == m_pBmpManager )
    {
        return ;
    }
    
    int px = m_animation.m_ptPos.x * m_nWidth;
    int py = m_animation.m_ptPos.y * m_nHeight;

    HDC memDC = CreateCompatibleDC(0);
    SelectObject(memDC,m_pBmpManager->atBitmap(m_nShowID));

    TransparentBlt(hDC,m_ptPos.x,m_ptPos.y,m_nWidth,m_nHeight,
        memDC,px,py,m_nWidth,m_nHeight,GetPixel(memDC,0,0));

    DeleteDC(memDC);
}

void CHero::moveX(bool positive)//水平方向移动。0下、1左、2右、3上。
{
    m_bMoving = true;
    if (positive)
    {
        m_nDirection = 2;
    }
    else
    {
        m_nDirection = 1;
    }
}

void CHero::moveY(bool positive) //垂直方向移动。0下、1左、2右、3上。
{
    m_bMoving = true;
    if (positive)
    {
        m_nDirection = 0;
    }
    else
    {
        m_nDirection = 3;
    }
}