//BitmapManager.cpp
#include "../G2DEngine/Commen.h"
#include <fstream>
#include "BitmapManager.h"

////////////////////////////////class CBitmapManager////////////////////
CBitmapManager::CBitmapManager()
{
}
CBitmapManager::~CBitmapManager()
{
	clear();
}

//从文件中读取位图文件名创建位图信息
bool CBitmapManager::load(HINSTANCE hInst,const char *szFileName)
{
    std::ifstream fin(szFileName);
    if(fin.fail() || fin.bad())//读取文件失败
    {
        return false;
    }

    clear();//将原有数据清空

    int nBitmapCount;
    fin>>nBitmapCount;//读入位图个数
    m_vecBitmaps.resize(nBitmapCount);
    char szBuffer[128];
    HBITMAP hBitmap=NULL;
    BITMAP bm;
    for(int i=0;i<nBitmapCount;++i)
    {
        fin>>szBuffer;//读取文件名
        hBitmap=(HBITMAP)LoadImage(hInst,szBuffer,
            IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
        if(NULL == hBitmap)//创建失败
        {
            g_log.writex("加载位图【%s】失败",szBuffer);
        }
        else
        {//获得位图的宽度和高度
            memset(&bm,0,sizeof(bm));
            GetObject(hBitmap,sizeof(bm),&bm);
            m_vecBitmaps[i].nWidth=bm.bmWidth;
            m_vecBitmaps[i].nHeight=bm.bmHeight;
        }
        m_vecBitmaps[i].hBitmap=hBitmap;
        fin>>m_vecBitmaps[i].nXFrame;//读取水平方向帧数
        fin>>m_vecBitmaps[i].nYFrame;//读取垂直方向帧数
    }
    fin.close();//关闭文件
    return true;
}

void CBitmapManager::clear(void)
{
    std::vector<BitmapInfo>::iterator it;
    for(it=m_vecBitmaps.begin(); it != m_vecBitmaps.end(); ++it)
    {
        if((*it).hBitmap != NULL)
        {
            DeleteObject((*it).hBitmap);
        }
    }
    m_vecBitmaps.clear();
}

LBitmapInfo CBitmapManager::at(int nIndex)
{
    if(nIndex>=0 && (UINT)nIndex<m_vecBitmaps.size())
    {
        return &(m_vecBitmaps[nIndex]);
    }
    else return NULL;
}

HBITMAP CBitmapManager::atBitmap(int nIndex)
{
    if(nIndex>=0 && (UINT)nIndex<m_vecBitmaps.size())
    {
        return m_vecBitmaps[nIndex].hBitmap;
    }
    else return NULL;
}
///////////////////////////class CBitmapManager///////////////////////