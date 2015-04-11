#include <Windows.h>
#include <stdio.h>
#include "DIBBitmap.h"

//////////////////////////////////////////////////////////////////////////
CDIBBitmap::CDIBBitmap(void)
{
    m_hDibBmp = NULL;
    m_pBmpData = NULL;
    m_nWidth = 0;   //宽度。(单位：像素pixel)
    m_nHeight = 0;  //高度。pixel
    m_nDeep = 0;    //深度。bit。深度必须大于24位
    m_nWidthBytes = 0;
}

CDIBBitmap::~CDIBBitmap(void)
{
    clear();
}

bool CDIBBitmap::create(HDC hdc,int width,int height,int deep)
{
    clear();

    m_nWidth = width;
    m_nHeight = height;
    m_nDeep = deep;

    BITMAPINFO bmInfo;
    memset(&bmInfo,0,sizeof(bmInfo));
    bmInfo.bmiHeader.biSize = sizeof(bmInfo);
    bmInfo.bmiHeader.biWidth = m_nWidth; 
    bmInfo.bmiHeader.biHeight = m_nHeight; 
    bmInfo.bmiHeader.biPlanes = 1; 
    bmInfo.bmiHeader.biBitCount =  m_nDeep;
    bmInfo.bmiHeader.biCompression = BI_RGB; 

    m_hDibBmp = CreateDIBSection(hdc,&bmInfo,DIB_RGB_COLORS,(void**)&m_pBmpData,NULL,0);
    if (m_hDibBmp == NULL)
    {
        return false;
    }
    
    m_nWidthBytes = ((m_nWidth*m_nDeep/8 + 3)/4) * 4;

    return true;
}

bool CDIBBitmap::create(HDC hDC,HBITMAP hBitmap)
{
    if(NULL==hDC || NULL==hBitmap)
    {
        return false;
    }
    //获得位图信息
    BITMAP bm;
    GetObject(hBitmap,sizeof(bm),&bm);
    //创建位图
    if(!create(hDC,bm.bmWidth,bm.bmHeight,bm.bmBitsPixel))
    {
        return false;
    }
    //获得hBitmap数据，并拷贝给Dib位图
    GetBitmapBits(hBitmap,bm.bmWidthBytes*bm.bmHeight,getBmpData());
    return true;
}

void CDIBBitmap::clear(void)
{
    if (m_hDibBmp != NULL)
    {
        DeleteObject(m_hDibBmp);
    }
    m_hDibBmp = NULL;
    m_pBmpData = NULL;
}


void CDIBBitmap::sampleRender(HDC hDC,int x,int y)
{
    HDC memDC = CreateCompatibleDC(0);
    SelectObject(memDC,m_hDibBmp);

    BitBlt(hDC,x,y,m_nWidth,m_nHeight,memDC,0,0,SRCCOPY);

    DeleteDC(memDC);
}

bool CDIBBitmap::load(HDC hDC,char *szFileName)
{
    FILE *fp = NULL;
    fopen_s(&fp,szFileName,"rb");
    if (NULL == fp)
    {
        return false;
    }
    //读入文件头
    BITMAPFILEHEADER bmheader;
    if(fread(&bmheader,sizeof(bmheader),1,fp) !=1 )
    {
        fclose(fp);
        return false;
    }
    //读入位图信息头
    BITMAPINFOHEADER bmInfo;
    if(fread(&bmInfo,sizeof(bmInfo),1,fp) !=1 )
    {
        fclose(fp);
        return false;
    }

    /*注：目前程序不支持调色板数据位图加载。
    */
    if (bmInfo.biBitCount < 16)
    {
        return false;
    }
    //创建DIB位图
    if (!create(hDC,bmInfo.biWidth,bmInfo.biHeight,bmInfo.biBitCount))
    {
        fclose(fp);
        return false;
    }
    //读入位图数据
    if(fread(m_pBmpData,m_nWidthBytes,m_nHeight,fp) != m_nHeight)
    {
        fclose(fp);
        return false;
    }

    fclose(fp);
    return true;
}

bool CDIBBitmap::save(char *szFileName)
{
    if (NULL==m_hDibBmp || NULL==m_pBmpData) 
    {
        return false;
    }

    FILE *fp = NULL;
    fopen_s(&fp,szFileName,"wb");
    if (NULL == fp)
    {
        return false;
    }

    BITMAPFILEHEADER bmheader;
    memset(&bmheader,0,sizeof(bmheader));
    bmheader.bfType=0x4d42;     //图像格式。必须为'BM'格式。
    bmheader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER); //从文件开头到数据的偏移量
    bmheader.bfSize = m_nWidthBytes*m_nHeight + bmheader.bfOffBits;//文件大小

    BITMAPINFOHEADER bmInfo;
    memset(&bmInfo,0,sizeof(bmInfo));
    bmInfo.biSize = sizeof(bmInfo);
    bmInfo.biWidth = m_nWidth; 
    bmInfo.biHeight = m_nHeight; 
    bmInfo.biPlanes = 1; 
    bmInfo.biBitCount =  m_nDeep;
    bmInfo.biCompression = BI_RGB; 

    if(fwrite(&bmheader,sizeof(bmheader),1,fp) != 1)
    {
        fclose(fp);
        return false;
    }
    if(fwrite(&bmInfo,sizeof(bmInfo),1,fp) != 1)
    {
        fclose(fp);
        return false;
    }
    for(int i=m_nHeight-1;i>=0;--i)
    {
        int index = m_nWidthBytes*i;
        if(fwrite(m_pBmpData+index,m_nWidthBytes,1,fp) != 1)
        {
            fclose(fp);
            return false;
        }
    }
    fclose(fp);

    return true;
}

//////////////////////////////////////////////////////////////////////////