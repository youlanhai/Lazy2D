#pragma once

#define ENGINE_UTILITY

#ifdef ENGINE_UTILITY

#include "Utilities.h"
class CDIBBitmap : public IBaseObject
#else
class CDIBBitmap
#endif
{
public://方法

    CDIBBitmap(void);
    ~CDIBBitmap(void);

    bool create(HDC hdc,int width,int height,int deep);//创建位图
    bool create(HDC hDC,HBITMAP hBitmap);//通过拷贝来创建位图

    bool load(HDC hDC,char *szFileName);//从文件中价值位图
    bool save(char *szFileName);//保存位图到文件

    void clear(void);//清空数据

    void sampleRender(HDC hDC,int x,int y);//简单显示

public://属性

    //获得位图
    inline HBITMAP getBitmap(void){ return m_hDibBmp; }
    //获得数据
    inline BYTE * getBmpData(void){ return m_pBmpData; }
    //获得宽度
    inline int  getWidth(void){ return m_nWidth; }
    //获得高度
    inline int  getHeight(void){ return m_nHeight; }
    //获得位深度
    inline int  getDeep(void){ return m_nDeep; }
    //获得一行字节数
    inline int  getWidthBytes(void){ return m_nWidthBytes; }
   
protected://成员
    HBITMAP     m_hDibBmp;
    BYTE        *m_pBmpData;//指向位图数据
    int         m_nWidth;   //宽度。(单位：像素pixel)
    int         m_nHeight;  //高度。pixel
    int         m_nDeep;    //深度。bit。深度必须大于24位
    int         m_nWidthBytes;//一行所需的字节数。必须是4字节的整数倍。
};