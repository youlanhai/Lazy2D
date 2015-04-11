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

//���ļ��ж�ȡλͼ�ļ�������λͼ��Ϣ
bool CBitmapManager::load(HINSTANCE hInst,const char *szFileName)
{
    std::ifstream fin(szFileName);
    if(fin.fail() || fin.bad())//��ȡ�ļ�ʧ��
    {
        return false;
    }

    clear();//��ԭ���������

    int nBitmapCount;
    fin>>nBitmapCount;//����λͼ����
    m_vecBitmaps.resize(nBitmapCount);
    char szBuffer[128];
    HBITMAP hBitmap=NULL;
    BITMAP bm;
    for(int i=0;i<nBitmapCount;++i)
    {
        fin>>szBuffer;//��ȡ�ļ���
        hBitmap=(HBITMAP)LoadImage(hInst,szBuffer,
            IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
        if(NULL == hBitmap)//����ʧ��
        {
            g_log.writex("����λͼ��%s��ʧ��",szBuffer);
        }
        else
        {//���λͼ�Ŀ�Ⱥ͸߶�
            memset(&bm,0,sizeof(bm));
            GetObject(hBitmap,sizeof(bm),&bm);
            m_vecBitmaps[i].nWidth=bm.bmWidth;
            m_vecBitmaps[i].nHeight=bm.bmHeight;
        }
        m_vecBitmaps[i].hBitmap=hBitmap;
        fin>>m_vecBitmaps[i].nXFrame;//��ȡˮƽ����֡��
        fin>>m_vecBitmaps[i].nYFrame;//��ȡ��ֱ����֡��
    }
    fin.close();//�ر��ļ�
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