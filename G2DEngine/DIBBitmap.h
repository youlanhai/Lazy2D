#pragma once

#define ENGINE_UTILITY

#ifdef ENGINE_UTILITY

#include "Utilities.h"
class CDIBBitmap : public IBaseObject
#else
class CDIBBitmap
#endif
{
public://����

    CDIBBitmap(void);
    ~CDIBBitmap(void);

    bool create(HDC hdc,int width,int height,int deep);//����λͼ
    bool create(HDC hDC,HBITMAP hBitmap);//ͨ������������λͼ

    bool load(HDC hDC,char *szFileName);//���ļ��м�ֵλͼ
    bool save(char *szFileName);//����λͼ���ļ�

    void clear(void);//�������

    void sampleRender(HDC hDC,int x,int y);//����ʾ

public://����

    //���λͼ
    inline HBITMAP getBitmap(void){ return m_hDibBmp; }
    //�������
    inline BYTE * getBmpData(void){ return m_pBmpData; }
    //��ÿ��
    inline int  getWidth(void){ return m_nWidth; }
    //��ø߶�
    inline int  getHeight(void){ return m_nHeight; }
    //���λ���
    inline int  getDeep(void){ return m_nDeep; }
    //���һ���ֽ���
    inline int  getWidthBytes(void){ return m_nWidthBytes; }
   
protected://��Ա
    HBITMAP     m_hDibBmp;
    BYTE        *m_pBmpData;//ָ��λͼ����
    int         m_nWidth;   //��ȡ�(��λ������pixel)
    int         m_nHeight;  //�߶ȡ�pixel
    int         m_nDeep;    //��ȡ�bit����ȱ������24λ
    int         m_nWidthBytes;//һ��������ֽ�����������4�ֽڵ���������
};