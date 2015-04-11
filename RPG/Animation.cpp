//Animation.cpp
#include "Commen.h"
#include "BitmapManager.h"
#include "Animation.h"

CAnimation::CAnimation()
: m_index(0)
{

}

CAnimation::~CAnimation()
{

}

//初始化动画资源
bool CAnimation::Initialize(HINSTANCE hInstance, std::string strBitmapListName)
{
    if (!bmpAnimManager.CreateBitmapFromFileList(hInstance, strBitmapListName))
        return false;
    return true;
}

//绘制开头动画
int CAnimation::DrawHeader(HDC hBackDC, RECT* rcClient)
{
    if (IsFinished())
    {
        return 0;
    }

    static DWORD dwOldTime = 0;
    int nDColor = 0;
    DWORD dwNewTime = timeGetTime();
    
    if (dwNewTime - dwOldTime > 50)
    {//每50ms更新一次
        dwOldTime = dwNewTime;
        if (m_index > 200 && m_index < 310)
            m_index += 1;
        else
            m_index += 4;
    }

    nDColor = m_index - 1;
    if (nDColor > 255)
    {
        nDColor = 255 - nDColor % 256;
    }

    //半透明显示对话区域
    BLENDFUNCTION bf;
    bf.AlphaFormat = 0;
    bf.BlendFlags = 0;
    bf.BlendOp = AC_SRC_OVER;
    bf.SourceConstantAlpha = nDColor;//透明度0-255
    HDC memDC = CreateCompatibleDC(hBackDC);
    LBitmapInfo bmi = bmpAnimManager.GetAt(0);
    SelectObject(memDC, bmi->hBitmap);
    int x = (rcClient->right - bmi->nWidth) / 2;
    int y = (rcClient->bottom - bmi->nHeight) / 2;
    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = bmi->nWidth;
    rect.bottom = bmi->nHeight;
    AlphaBlend(hBackDC, x, y, bmi->nWidth, bmi->nHeight,
        memDC, 0, 0, bmi->nWidth, bmi->nHeight, bf);
    DeleteDC(memDC);
    return 0;
}

int	CAnimation::DrawAnimation(HDC hBackDC, RECT* rcClient)
{
    //绘制敌人
    LBitmapInfo bmi = bmpAnimManager.GetAt(0);
    if (!bmi)
    {
        return 0;
    }

    HDC memDC = CreateCompatibleDC(0);
    SelectObject(memDC, bmi->hBitmap);
    TransparentBlt(hBackDC, rcClient->left + 100, rcClient->bottom - 100 - bmi->nHeight, bmi->nWidth, bmi->nHeight,
        memDC, 0, 0, bmi->nWidth, bmi->nHeight, GetPixel(memDC, 0, 0));

    //绘制玩家
    bmi = bmpAnimManager.GetAt(1);
    SelectObject(memDC, bmi->hBitmap);

    TransparentBlt(hBackDC, rcClient->right - 100 - bmi->nWidth, rcClient->bottom - 100 - bmi->nHeight,
        bmi->nWidth, bmi->nHeight,
        memDC, 0, 0, bmi->nWidth, bmi->nHeight, GetPixel(memDC, 0, 0));
    DeleteDC(memDC);
    return 1;
}

void CAnimation::ClearAnimation()
{
    bmpAnimManager.DestroyHBitmapArray();
}
