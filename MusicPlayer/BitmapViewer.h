#pragma once

#include "../G2DEngine/Utilities.h"

//λͼ�鿴��
class CBitmapViewer : public IBaseObject
{
public:
    CBitmapViewer(void);
    ~CBitmapViewer(void);

    bool load(HINSTANCE hInst,char *szFilename);

    //һ��Ч����ϣ�����true
    bool update(int elapse);
    void render(HDC hDC);

    void clear();

    //����ͼƬ�� 
    inline void setBitmap(HBITMAP bmp,bool bManaged=false)
    {
        m_hBitmap=bmp; 
        m_bAutoManageRes = bManaged; 
    }
    inline HBITMAP getBitmap( void ){ return m_hBitmap; }

    //�Ƿ�͸������
    inline void setTransparent(bool t){ m_bTransparent=t; }

    //�Ƿ����촦��
    inline void setStretch( bool s){ m_bStretch=s; }

    //�����ڴ����
    inline void setManaged(bool m){ m_bAutoManageRes=m; }
    inline bool getManaged( void ){ return m_bAutoManageRes; }

    //���ÿؼ�����
    void setRect(CGameRect * rc);
    inline CGameRect & getRect(void){return m_rect; }

    //���õ�����
    inline void setFadeStyle(DWORD style){ m_dwFadeStyle = style; }

    //���õ���ʱ��
    inline void setFadeTime(int time){ m_nFadeTime = time; }
    inline void setShowTime(int time){ m_nShowTime = time; }

    inline bool getFadeOver(void){ return m_bFadeOver; }
    inline void setFadeOver(bool o){ m_bFadeOver = o; }
private:
    HBITMAP     m_hBitmap;
    bool        m_bAutoManageRes;   //�Զ�������Դ
    bool        m_bTransparent; //�Ƿ�͸������
    bool        m_bStretch;     //�Ƿ�����
    HDC         m_dcBack;       
    CGameRect   m_rect;
    DWORD       m_dwShowStyle;//��ʾ���

    /*���뵭�����
    0-��Ч��
    1-�Ӱ�ɫ����
    2-�Ӱ�ɫ����
    4-�Ӻ�ɫ����
    8-�Ӻ�ɫ����
    */
    DWORD       m_dwFadeStyle;  //���뵭�����
    int         m_nFadeTime;    //�����ʱ��(ms)
    int         m_nShowTime;    //������ʾ��ʱ��(ms)
    int         m_nBlendColor;  //�����ɫ
    int         m_nBlendAlpa;   //͸�����ϵ��
    int         m_nElapse;      //�Ѿ����ŵ�ʱ��
    bool        m_bFadeOver;    //�Ƿ񵭳����
};
