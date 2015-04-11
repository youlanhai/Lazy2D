#include "Cursor.h"

CCursor::CCursor(void) : m_size(32,32)
{
    m_hBitmap = NULL;
    m_bTransparent = true;
}

CCursor::~CCursor(void)
{
}

void CCursor::update(POINT * pt)
{
    m_ptPos = *pt;
}
void CCursor::render(HDC hDC)
{
    if (hDC==NULL || m_hBitmap==NULL)
    {
        return ;
    }

    HDC memDC = CreateCompatibleDC(hDC);
    SelectObject(memDC,m_hBitmap);
    BITMAP bm;
    GetObject(m_hBitmap,sizeof(bm),&bm);

    if (m_bTransparent)
    {
        TransparentBlt(hDC,m_ptPos.x,m_ptPos.y,m_size.cx,m_size.cy,
            memDC,0,0,bm.bmWidth,bm.bmHeight,GetPixel(memDC,0,0));
    }
    else
    {
        BitBlt(hDC,m_ptPos.x,m_ptPos.y,m_size.cx,m_size.cy,
            memDC,0,0,SRCCOPY);
    }
    DeleteDC(memDC);
}