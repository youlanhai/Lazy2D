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

public://����
    //��������
    inline void setPosition(const CPoint & pt){ m_ptPos = pt; };
    inline CPoint & getPosition(void){ return m_ptPos; }
    //���óߴ�
    inline void setSize(const CSize & sz){ m_size = sz; }
    inline CSize & getSize(void){ return m_size; }
    //�������ͼ��
    inline void setBitmap(HBITMAP bmp){ m_hBitmap = bmp; }
    inline HBITMAP getBitmap(void){ return m_hBitmap ; }
    //����͸��
    void setTransparent(bool t){ m_bTransparent = t; }
    bool getTransparent(void){ return m_bTransparent; }
private:
    CPoint      m_ptPos;    //�����ʾλ��
    CSize       m_size;     //�����ʾ�ߴ�
    HBITMAP     m_hBitmap;  //���ͼ��
    bool        m_bTransparent;//͸������
};
