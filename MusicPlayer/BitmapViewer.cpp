#include "../G2DEngine/Commen.h"
#include "BitmapViewer.h"

#define SafeDeleteObject(pObj) {if(pObj!=NULL){DeleteObject(pObj);pObj=NULL;}}

CBitmapViewer::CBitmapViewer(void)
{ 
    m_hBitmap = 0;
    m_bAutoManageRes = false;   //自动管理资源
    m_bTransparent = false; //是否透明处理
    m_bStretch = false;     //是否拉伸
    m_dcBack = CreateCompatibleDC(0);
    m_dwShowStyle = 0;//显示风格
    
    m_dwFadeStyle = 0;
    m_nFadeTime = 1000;//淡入的时长(ms)
    m_nShowTime = 2000;//正常显示的时长(ms)
    m_nBlendAlpa = 0;    //透明混合系数
    m_nBlendColor = 255;//混合颜色
    m_nElapse = 0;  //已经流逝的时间
    m_bFadeOver = false;
}

CBitmapViewer::~CBitmapViewer(void)
{
    clear();
    DeleteDC(m_dcBack);
}

bool CBitmapViewer::load(HINSTANCE hInst,char *szFilename)
{
    clear();
    m_hBitmap=(HBITMAP)LoadImage(hInst,szFilename,
        IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
    if(NULL == m_hBitmap)//创建失败
    {
        g_log.writex("加载位图【%s】失败",szFilename);
        return false;
    }
    return true;
}

void CBitmapViewer::setRect(CGameRect * rc)
{
    if (rc != NULL)
    {
        m_rect = *rc;
    }
    else if(m_hBitmap != NULL)
    {
        BITMAP bm;
        GetObject(m_hBitmap,sizeof(bm),&bm);
        m_rect = CGameRect(0,0,bm.bmWidth,bm.bmHeight);
    }
}

bool CBitmapViewer::update(int elapse)
{
    /*淡入淡出风格
    0-无效果
    1-从白色淡入
    2-从白色淡出
    4-从黑色淡入
    8-从黑色淡出
    */
    m_nElapse += elapse;
    if (m_nElapse <= m_nFadeTime)
    {
        if (m_dwFadeStyle & 1)//从白色淡入
        {
            m_nBlendAlpa = (255*m_nElapse)/m_nFadeTime;
            m_nBlendColor = RGB(255,255,255);
        }
        else if(m_dwFadeStyle & 4)//从黑色淡入
        {
            m_nBlendAlpa = (255*m_nElapse)/m_nFadeTime;
            m_nBlendColor = 0;
        }
    }
    else if(m_nElapse <= m_nFadeTime+m_nShowTime)
    {
        m_nBlendAlpa = 255;
    }
    else if(m_nElapse <= m_nFadeTime*2+m_nShowTime)
    {
        int dt = m_nElapse - (m_nFadeTime+m_nShowTime);
        if ( m_dwFadeStyle & 2)//淡出白色
        {
            m_nBlendAlpa = 255-(255*dt)/m_nFadeTime;
            m_nBlendColor = RGB(255,255,255);
        }
        else if(m_dwFadeStyle & 8)//淡出黑色
        {
            m_nBlendAlpa = 255-(255*dt)/m_nFadeTime;
            m_nBlendColor = 0;
        }
    }
    else
    {
        m_nElapse = 0;
        m_bFadeOver = true;
        return true;
    }
    return false;
}
void CBitmapViewer::render(HDC hDC)
{
    if(NULL == m_hBitmap)
    {
        return ;
    }
    SelectObject(m_dcBack,m_hBitmap);

    BITMAP bm;
    GetObject(m_hBitmap,sizeof(bm),&bm);

    HDC memDC = CreateCompatibleDC(0);
    HBITMAP memBmp = CreateCompatibleBitmap(hDC,bm.bmWidth,bm.bmHeight);
    SelectObject(memDC,memBmp);

    if (m_dwFadeStyle != 0)
    {
        RECT rc ;
        rc.left = rc.top = 0;
        rc.right = bm.bmWidth;
        rc.bottom = bm.bmHeight;

        HBRUSH brush = CreateSolidBrush(m_nBlendColor);
        FillRect(memDC,&rc,brush);
        //半透明显示对话区域
        BLENDFUNCTION bf;
        bf.AlphaFormat=0;
        bf.BlendFlags=0;
        bf.BlendOp=AC_SRC_OVER;
        bf.SourceConstantAlpha=m_nBlendAlpa;//透明度0-255
        AlphaBlend(memDC,0,0,rc.right,rc.bottom,
            m_dcBack,0,0,rc.right,rc.bottom,bf);
        DeleteObject(brush);
    }
    else
    {
        BitBlt(memDC,m_rect.x,m_rect.y,m_rect.w,m_rect.h,
            m_dcBack,0,0,SRCCOPY);
    }


    if (m_bTransparent)
    {
        int dw=min(m_rect.w,bm.bmWidth);
        int dh=min(m_rect.h,bm.bmHeight);
        if (m_bStretch)
        {
            dw=m_rect.w;
            dh=m_rect.h;
        }
        int sw=min(m_rect.w,bm.bmWidth);
        int sh=min(m_rect.h,bm.bmHeight);
        if (m_bStretch)
        {
            sw=bm.bmWidth;
            sh=bm.bmHeight;
        }
        TransparentBlt(hDC,m_rect.x,m_rect.y,dw,dh,
            memDC,0,0,sw,sh,GetPixel(memDC,0,0));
    }
    else if(m_bStretch)
    {
        StretchBlt(hDC,m_rect.x,m_rect.y,m_rect.w,m_rect.h,
            memDC,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
    }
    else
    {
        BitBlt(hDC,m_rect.x,m_rect.y,m_rect.w,m_rect.h,
            memDC,0,0,SRCCOPY);
    }


    DeleteObject(memBmp);
    DeleteDC(memDC);

}

void CBitmapViewer::clear()
{
    if (m_bAutoManageRes)
    {
        SafeDeleteObject(m_hBitmap);
    }
}
