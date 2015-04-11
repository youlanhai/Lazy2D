#pragma once
#include "..\g2dengine\utilities.h"
#include "..\g2dengine\BitmapManager.h"

//��������
enum AnimType
{
    AT_XNYN = 1,    //ˮƽ����N֡����ֱ����N֡��ˮƽ��������
    AT_YNXN,        //ˮƽ����N֡����ֱ����N֡����ֱ��������
    AT_XN,          //��ˮƽ֡��
    AT_YN,          //����ֱ֡��
    FORCE_DWORD = 0xffffffff
};

struct IAnimation : public IBaseObject
{
    IAnimation(void);
    void update(int nElapse,int param);
    void setBmpInfo(LBitmapInfo pInfo);

    AnimType m_nType;
    CPoint  m_ptPos;	//ͼƬ�ϵ���������,�������������ꡣ
    int m_nElapseTime;  //�Ѿ�����ʱ��
    int m_nInterval;    //����ʱ����
private:
    LBitmapInfo m_pBmpInfo;
};

struct IBaseSprit : public IBaseObject
{
    IBaseSprit(void);
    virtual ~IBaseSprit(void);
    virtual void update(int nElapse,RECT * rc) { }
    virtual void render(HDC hDC) { }
    virtual void clear(void) { }    //�ͷ���Դ
    virtual void moveX(bool positive) { }//ˮƽ�����ƶ�
    virtual void moveY(bool positive) { }//��ֱ�����ƶ�

    int m_nID;					//Ψһ���
    int m_nShowID;				//��ʾͼƬ
    int m_nState;				//����״̬
    char m_szName[128]; 		//����
    int m_nDirection;           //��ǰ�˶�����0�¡�1��2�ҡ�3�ϡ�
    int m_nSpeed;				//�ƶ��ٶ�
    int m_nWidth;               //�����С
    int m_nHeight;
    CPoint m_ptPos;			    //��ͼ�ϵ�����
    int m_nLife;
    int m_nMaxLife;				//����ֵ
    int m_nMagic;
    int m_nMaxMagic;			//ħ��ֵ
    int m_nAttack;				//����
    int m_nDefence;				//����
    int m_nExperence;			//����
    int m_nLevel;				//����
    IAnimation m_animation;     //ͼƬ����

    static int	m_nIDCounter;		//ID������
    static CBitmapManager* m_pBmpManager;//λͼ������
    static void setBmpManager(CBitmapManager * pManager);
};
