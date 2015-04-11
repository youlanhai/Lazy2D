#pragma once

#include "utilities.h"

class CDC;

class CPen : public IBaseObject
{
public:
    // Constructors
    CPen() : m_hPen(0) { }
    CPen(int nPenStyle, int nWidth, COLORREF crColor)
    {
        createPen(nPenStyle,nWidth,crColor);
    }

    virtual ~CPen()
    {
        deleteObject();
    }

    bool createPen(int nPenStyle, int nWidth, COLORREF crColor)
    {
        m_hPen = CreatePen(nPenStyle,nWidth,crColor);
        if (m_hPen == NULL)
        {
            return false;
        }
        else return true;
    }

    void deleteObject()
    {
        if(m_hPen != NULL)
        {
            DeleteObject(m_hPen);
            m_hPen = NULL;
        }
    }

public:
    HPEN getHPen(void){ return m_hPen; }

protected:
    HPEN    m_hPen;
};

class CBrush : public IBaseObject
{
public:
    CBrush() : m_hBrush(0) { };
    CBrush(COLORREF crColor)
    {
        createBrush(crColor);
    }

    virtual ~CBrush()
    {
        deleteObject();
    }

    bool createBrush(COLORREF crColor)
    {
        m_hBrush = CreateSolidBrush(crColor);
        if (m_hBrush == NULL)
        {
            return false;
        }
        else return true;
    }

    void deleteObject()
    {
        if(m_hBrush != NULL)
        {
            DeleteObject(m_hBrush);
            m_hBrush = NULL;
        }
    }

public:
    HBRUSH getHBrush(void){ return m_hBrush; }
   
protected:
    HBRUSH m_hBrush;
};

class CFont : public IBaseObject
{
public:
    CFont() ;
    virtual ~CFont();

    CFont(int height,char *szName);

    bool createFontIndirect(const LOGFONT* lpLogFont);

    bool createFont(int nHeight, int nWidth, int nEscapement,
        int nOrientation, int nWeight, BYTE bItalic, BYTE bUnderline,
        BYTE cStrikeOut, BYTE nCharSet, BYTE nOutPrecision,
        BYTE nClipPrecision, BYTE nQuality, BYTE nPitchAndFamily,
        LPCTSTR lpszFacename);
  
    bool createFontEx(int height,char *szName);

    void deleteObject(void);

public:
    HFONT getHFont(void){ return m_hFont; }

protected:
    HFONT m_hFont;
};

class CBitmap : public IBaseObject
{
public:
    CBitmap();
    virtual ~CBitmap();

    bool loadBitmap(HINSTANCE hInst,char *szFile);

    bool loadBitmap(HINSTANCE hInst,UINT nIDResource);
    
    bool createBitmap(HDC hDC, int nWidth, int nHeight);

    void deleteObject(void);
    
    int getBitmap(BITMAP* pBitMap);


    int setBitmapBits(DWORD dwCount, const void* lpBits);

    int getBitmapBits(DWORD dwCount, LPVOID lpBits);

public:
    HBITMAP getHBitmap(void){ return m_hBitmap; }

protected:
    HBITMAP m_hBitmap;
};


/////////////////////////////////////////////////////////////////////////////

class CDC : public IBaseObject
{
public:
    CDC();
    CDC(HDC hDC);
    virtual ~CDC();

    bool createDC(HDC hDC);
    bool createDC(HDC hDC,CBitmap *pBmp);

    void deleteDC();

public:

    HGDIOBJ selectObject(HGDIOBJ hGdi);

    COLORREF getBkColor();
    int getBkMode();
    COLORREF getTextColor();

    COLORREF setBkColor(COLORREF crColor);
    int setBkMode(int nBkMode);
    COLORREF setTextColor(COLORREF crColor);
  
    CPoint moveTo(int x, int y);
    CPoint moveTo(const POINT * lpPoint);
    bool lineTo(int x, int y);
    bool lineTo(const POINT * lpPoint);

    bool ellipse(int x1, int y1, int x2, int y2);
    bool ellipse(LPCRECT lpRect);

    bool polygon(const POINT* lpPoints, int nCount);	
   
    bool rectangle(int x1, int y1, int x2, int y2);
    bool rectangle(const POINT * lpLT ,const POINT * lpRB);
    bool rectangle(LPCRECT lpRect);

    bool fillRect(LPCRECT lpRect, CBrush* pBrush);

    void fillSolidRect(LPCRECT lpRect, COLORREF clr);

    bool bitBlt(int x, int y, int nWidth, int nHeight, CBitmap *pBitmap,
        int xSrc, int ySrc, DWORD dwRop);

    bool bitBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC,
        int xSrc, int ySrc, DWORD dwRop);

    bool stretchBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC,
        int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop);

    bool transparentBlt(int xDest, int yDest, int nDestWidth, int nDestHeight,
        CDC* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, 
        UINT clrTransparent);

    bool alphaBlend(int xDest, int yDest, int nDestWidth, int nDestHeight,
        CDC* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, 
        BLENDFUNCTION blend);

    COLORREF getPixel(int x, int y) const;
    COLORREF getPixel(const POINT * point) const;
    COLORREF setPixel(int x, int y, COLORREF crColor);
    COLORREF setPixel(const POINT * point, COLORREF crColor);

    bool setPixelV(int x, int y, COLORREF crColor);
    bool setPixelV(const POINT * point, COLORREF crColor);

    bool textOut(int x, int y, char* szString, int nCount);

    int drawText(char* szString, int nCount, LPRECT lpRect,
        UINT nFormat);

public:
    HDC getHdc() { return m_hDC; }

protected:
    HDC m_hDC; 
 
};


class CDevice : public CDC
{
public:
    CDevice(void);
    ~CDevice(void);

    void init(HWND hWnd);

    void present();//显示到屏幕。渲染完毕后，必须调用次函数。
public:
    HDC getDisplayDC(void){ return m_hDisplay; }
    CBitmap * getBackBitmap(void){ return &m_bmpBack; }
private:
    HDC     m_hDisplay;
    HWND    m_hWnd;
    CBitmap m_bmpBack;
};