//GDI.cpp 
#include "../G2DEngine/Commen.h"
#include "GDI.h"


//////////////////////////////////////////////////////////////////////////

CFont::CFont() : m_hFont(0) 
{
}

CFont::CFont(int height,char *szName)
{
    createFontEx(height,szName);
}


CFont::~CFont()
{
    deleteObject();
}

bool CFont::createFontIndirect(const LOGFONT* lpLogFont)
{
    m_hFont = CreateFontIndirect(lpLogFont);
    if (m_hFont == NULL)
    {
        return false;
    }
    else return true;
}

bool CFont::createFont(int nHeight, int nWidth, int nEscapement,
                int nOrientation, int nWeight, BYTE bItalic, BYTE bUnderline,
                BYTE cStrikeOut, BYTE nCharSet, BYTE nOutPrecision,
                BYTE nClipPrecision, BYTE nQuality, BYTE nPitchAndFamily,
                LPCTSTR lpszFacename)
{
    m_hFont = CreateFont(nHeight,nWidth,nEscapement,nOrientation,nWeight,
        bItalic,bUnderline,cStrikeOut,nCharSet,nOutPrecision,
        nClipPrecision,nQuality,nPitchAndFamily,lpszFacename);
    if (m_hFont == NULL)
    {
        return false;
    }
    else return true;
}

bool CFont::createFontEx(int height,char *szName)
{
    m_hFont = CreateFont(
        height,                        // nHeight
        0,                         // nWidth
        0,                         // nEscapement
        0,                         // nOrientation
        FW_NORMAL,                 // nWeight
        FALSE,                     // bItalic
        FALSE,                     // bUnderline
        0,                         // cStrikeOut
        ANSI_CHARSET,              // nCharSet
        OUT_DEFAULT_PRECIS,        // nOutPrecision
        CLIP_DEFAULT_PRECIS,       // nClipPrecision
        DEFAULT_QUALITY,           // nQuality
        DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
        szName);                 // lpszFacename
    if (m_hFont != NULL)
    {
        return true;
    }
    else return false;
}


void CFont::deleteObject(void)
{
    if(m_hFont != NULL)
    {
        DeleteObject(m_hFont);
        m_hFont = NULL;
    }
}


//////////////////////////////////////////////////////////////////////////
CBitmap::CBitmap() : m_hBitmap(0) { }

bool CBitmap::loadBitmap(HINSTANCE hInst,char *szFile)
{
    m_hBitmap = (HBITMAP)LoadImage(hInst,szFile,IMAGE_BITMAP,0,0,
        LR_LOADFROMFILE);
    if (m_hBitmap == NULL)
    {
        return false;
    }
    else return true;
}

bool CBitmap::loadBitmap(HINSTANCE hInst,UINT nIDResource)
{
    m_hBitmap = LoadBitmap(hInst,MAKEINTRESOURCE(nIDResource));
    if (m_hBitmap == NULL)
    {
        return false;
    }
    else return true;
}

bool CBitmap::createBitmap(HDC hDC, int nWidth, int nHeight)
{
    m_hBitmap = CreateCompatibleBitmap(hDC,nWidth,nHeight);
    if (m_hBitmap == NULL)
    {
        return false;
    }
    else return true;
}

void CBitmap::deleteObject(void)
{
    if(m_hBitmap != NULL)
    {
        DeleteObject(m_hBitmap);
        m_hBitmap = NULL;
    }
}


int CBitmap::getBitmap(BITMAP* pBitMap)
{
    return GetObject(m_hBitmap,sizeof(BITMAP),pBitMap);
}

// Operations
int CBitmap::setBitmapBits(DWORD dwCount, const void* lpBits)
{
    return SetBitmapBits(m_hBitmap,dwCount,lpBits);
}

int CBitmap::getBitmapBits(DWORD dwCount, LPVOID lpBits)
{
    return GetBitmapBits(m_hBitmap,dwCount,lpBits);
}

CBitmap::~CBitmap()
{
    deleteObject();
}

//////////////////////////////////////////////////////////////////////////


// Constructors
CDC::CDC()
{
    m_hDC = NULL;
}

CDC::CDC(HDC hDC)
{
    createDC(hDC);
}


CDC::~CDC()
{
    deleteDC();
}

bool CDC::createDC(HDC hDC)
{
    m_hDC = CreateCompatibleDC(hDC);
    if (NULL == m_hDC)
    {
        return false;
    }
    return true;
}

bool CDC::createDC(HDC hDC,CBitmap *pBmp)
{
    m_hDC = CreateCompatibleDC(hDC);
    if (NULL == m_hDC)
    {
        return false;
    }
    selectObject(pBmp->getHBitmap());
    return true;
}
void CDC::deleteDC()
{
    if (m_hDC != NULL)
    {
        DeleteDC(m_hDC);
    }
}

HGDIOBJ CDC::selectObject(HGDIOBJ hGdi)
{
    return SelectObject(m_hDC,hGdi);
}

COLORREF CDC::getBkColor()
{
    return GetBkColor(m_hDC);
}

int CDC::getBkMode()
{
    return GetBkMode(m_hDC);
}

COLORREF CDC::getTextColor()
{
    return GetTextColor(m_hDC);
}

COLORREF CDC::setBkColor(COLORREF crColor)
{
    return SetBkColor(m_hDC,crColor);
}

int CDC::setBkMode(int nBkMode)
{
    return SetBkMode(m_hDC,nBkMode);
}

COLORREF CDC::setTextColor(COLORREF crColor)
{
    return SetTextColor(m_hDC,crColor);
}


// Line-Output Functions
CPoint CDC::moveTo(int x, int y)
{
    CPoint pt;
    MoveToEx(m_hDC,x,y,&pt);
    return pt;
}

CPoint CDC::moveTo(const POINT * lpPoint)
{
    return moveTo(lpPoint->x,lpPoint->y);
}

bool CDC::lineTo(int x, int y)
{
    return !!LineTo(m_hDC,x,y);
}

bool CDC::lineTo(const POINT * lpPoint)
{
    return !!LineTo(m_hDC,lpPoint->x, lpPoint->y);
}

// Ellipse and Polygon Functions
bool CDC::ellipse(int x1, int y1, int x2, int y2)
{
    return !!Ellipse(m_hDC,x1,y1,x2,y2);
}
bool CDC::ellipse(LPCRECT lpRect)
{
    return !!Ellipse(m_hDC,lpRect->left,lpRect->top,lpRect->right,lpRect->bottom);
}
bool CDC::polygon(const POINT* lpPoints, int nCount)
{
    return !!Polygon(m_hDC,lpPoints,nCount);
}

bool CDC::rectangle(int x1, int y1, int x2, int y2)
{
    return !!Rectangle(m_hDC,x1,x2,y1,y2);
}

bool CDC::rectangle(const POINT * lpLT ,const POINT * lpRB)
{
    return !!Rectangle(m_hDC,lpLT->x, lpLT->y, lpRB->x, lpRB->y);
}

bool CDC::rectangle(LPCRECT lpRect)
{
    return !!Rectangle(m_hDC,lpRect->left,lpRect->top,lpRect->right,lpRect->bottom);
}

bool CDC::fillRect(LPCRECT lpRect, CBrush* pBrush)
{
    FillRect(m_hDC,lpRect,pBrush->getHBrush());
    return true;
}

void CDC::fillSolidRect(LPCRECT lpRect, COLORREF clr)
{
    CBrush brush(clr);
    fillRect(lpRect,&brush);
}

// Bitmap Functions
bool CDC::bitBlt(int x, int y, int nWidth, int nHeight, CBitmap *pBitmap,
    int xSrc, int ySrc, DWORD dwRop)
{
    CDC memDC(0);
    memDC.selectObject(pBitmap->getHBitmap());
    bitBlt(x,y,nWidth,nHeight,&memDC,xSrc,ySrc,dwRop);
    return true;
}

bool CDC::bitBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC,
                 int xSrc, int ySrc, DWORD dwRop)
{
    return !!BitBlt(m_hDC,x,y,nWidth,nHeight,pSrcDC->getHdc(),
        xSrc,ySrc,dwRop);
}

bool CDC::stretchBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC,
                     int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop)
{
    return !!StretchBlt(m_hDC,x,y,nWidth,nHeight,pSrcDC->getHdc(),
        xSrc,ySrc,nSrcWidth,nSrcHeight,dwRop);
}

bool CDC::transparentBlt(int xDest, int yDest, int nDestWidth, int nDestHeight,
    CDC* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, 
    UINT clrTransparent)
{
    return !!TransparentBlt(m_hDC,xDest,yDest,nDestWidth,nDestHeight,
        pSrcDC->getHdc(),xSrc,ySrc,nSrcWidth,nSrcHeight,clrTransparent);
}

bool CDC::alphaBlend(int xDest, int yDest, int nDestWidth, int nDestHeight,
    CDC* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, 
    BLENDFUNCTION blend)
{
    return !!AlphaBlend(m_hDC,xDest,yDest,nDestWidth,nDestHeight,
        pSrcDC->getHdc(),xSrc,ySrc,nSrcWidth,nSrcHeight,blend);
}

COLORREF CDC::getPixel(int x, int y) const
{
    return GetPixel(m_hDC,x,y);
}
COLORREF CDC::getPixel(const POINT * point) const
{
    return GetPixel(m_hDC,point->x,point->y);
}
COLORREF CDC::setPixel(int x, int y, COLORREF crColor)
{
    return !!SetPixel(m_hDC,x,y,crColor);
}
COLORREF CDC::setPixel(const POINT * point, COLORREF crColor)
{
    return !!SetPixel(m_hDC,point->x,point->y,crColor);
}

bool CDC::setPixelV(int x, int y, COLORREF crColor)
{
    return !!SetPixelV(m_hDC,x,y,crColor);
}
bool CDC::setPixelV(const POINT * point, COLORREF crColor)
{
    return !!SetPixelV(m_hDC, point->x, point->y, crColor);
}
// Text Functions
bool CDC::textOut(int x, int y, char* szString, int nCount)
{
    return !!TextOutA(m_hDC,x,y,szString,nCount);
}

int CDC::drawText(char* szString, int nCount, LPRECT lpRect,
    UINT nFormat)
{
    return DrawTextA(m_hDC,szString,nCount,lpRect,nFormat);
}

//////////////////////////////////////////////////////////////////////////

CDevice::CDevice(void)
{
    m_hDisplay = NULL;
    m_hWnd = NULL;
}

CDevice::~CDevice(void)
{
    if (m_hDisplay != NULL)
    {
        ReleaseDC(m_hWnd,m_hDisplay);
    }
}

void CDevice::init(HWND hWnd)
{
    m_hWnd = hWnd;
    m_hDisplay = GetDC(m_hWnd);

    CRect rc;
    GetClientRect(m_hWnd,&rc);
    m_bmpBack.createBitmap(m_hDisplay,rc.width(),rc.height());
    selectObject(m_bmpBack.getHBitmap());
}

void CDevice::present()
{
    CRect rc;
    GetClientRect(m_hWnd,&rc);
    BitBlt(m_hDisplay,0,0,rc.width(),rc.height(),m_hDC,0,0,SRCCOPY);
}