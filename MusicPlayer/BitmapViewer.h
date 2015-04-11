#pragma once

#include "../G2DEngine/Utilities.h"

//位图查看器
class CBitmapViewer : public IBaseObject
{
public:
    CBitmapViewer(void);
    ~CBitmapViewer(void);

    bool load(HINSTANCE hInst,char *szFilename);

    //一轮效果完毕，返回true
    bool update(int elapse);
    void render(HDC hDC);

    void clear();

    //设置图片。 
    inline void setBitmap(HBITMAP bmp,bool bManaged=false)
    {
        m_hBitmap=bmp; 
        m_bAutoManageRes = bManaged; 
    }
    inline HBITMAP getBitmap( void ){ return m_hBitmap; }

    //是否透明处理
    inline void setTransparent(bool t){ m_bTransparent=t; }

    //是否拉伸处理
    inline void setStretch( bool s){ m_bStretch=s; }

    //设置内存管理
    inline void setManaged(bool m){ m_bAutoManageRes=m; }
    inline bool getManaged( void ){ return m_bAutoManageRes; }

    //设置控件区域
    void setRect(CGameRect * rc);
    inline CGameRect & getRect(void){return m_rect; }

    //设置淡入风格
    inline void setFadeStyle(DWORD style){ m_dwFadeStyle = style; }

    //设置淡入时间
    inline void setFadeTime(int time){ m_nFadeTime = time; }
    inline void setShowTime(int time){ m_nShowTime = time; }

    inline bool getFadeOver(void){ return m_bFadeOver; }
    inline void setFadeOver(bool o){ m_bFadeOver = o; }
private:
    HBITMAP     m_hBitmap;
    bool        m_bAutoManageRes;   //自动管理资源
    bool        m_bTransparent; //是否透明处理
    bool        m_bStretch;     //是否拉伸
    HDC         m_dcBack;       
    CGameRect   m_rect;
    DWORD       m_dwShowStyle;//显示风格

    /*淡入淡出风格
    0-无效果
    1-从白色淡入
    2-从白色淡出
    4-从黑色淡入
    8-从黑色淡出
    */
    DWORD       m_dwFadeStyle;  //淡入淡出风格
    int         m_nFadeTime;    //淡入的时长(ms)
    int         m_nShowTime;    //正常显示的时长(ms)
    int         m_nBlendColor;  //混合颜色
    int         m_nBlendAlpa;   //透明混合系数
    int         m_nElapse;      //已经流逝的时间
    bool        m_bFadeOver;    //是否淡出完毕
};
