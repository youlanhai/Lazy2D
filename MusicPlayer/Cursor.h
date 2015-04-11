#pragma once
#include "..\g2dengine\utilities.h"

class CCursor :
    public IBaseObject
{
public:
    CCursor(void);
    ~CCursor(void);

    void update(POINT * pt);
    void render(HDC hDC);

public://属性
    //设置坐标
    inline void setPosition(const CPoint & pt){ m_ptPos = pt; };
    inline CPoint & getPosition(void){ return m_ptPos; }
    //设置尺寸
    inline void setSize(const CSize & sz){ m_size = sz; }
    inline CSize & getSize(void){ return m_size; }
    //设置鼠标图像
    inline void setBitmap(HBITMAP bmp){ m_hBitmap = bmp; }
    inline HBITMAP getBitmap(void){ return m_hBitmap ; }
    //设置透明
    void setTransparent(bool t){ m_bTransparent = t; }
    bool getTransparent(void){ return m_bTransparent; }
private:
    CPoint      m_ptPos;    //鼠标显示位置
    CSize       m_size;     //鼠标显示尺寸
    HBITMAP     m_hBitmap;  //鼠标图像
    bool        m_bTransparent;//透明处理
};
