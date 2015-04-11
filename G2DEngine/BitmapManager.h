//BitmapManager.h
#pragma once
#include <string>
#include <vector>

//λͼ��Ϣ
struct BitmapInfo
{
	HBITMAP hBitmap;//λͼ���
	int nWidth;		//λͼ���
	int nHeight;	//λͼ�߶�
	int nXFrame;	//λͼx����֡��
	int nYFrame;	//λͼy����֡��

	BitmapInfo()
	{
		hBitmap=NULL;
		nWidth=nHeight=0;
		nXFrame=nYFrame=0;
	}
};

typedef  BitmapInfo *LBitmapInfo;
typedef  BitmapInfo CBitmapInformation;//��ɰ汾����

/***************************************
λͼ������������λͼ�ļ��أ�������ͷš�
�÷�������λͼ�����ļ���load���ļ��ж�ȡ��Ϣ��������λͼ��
***************************************/
class CBitmapManager
{
private:
	std::vector<BitmapInfo>	m_vecBitmaps;//λͼ��Ϣ����
    
public://v2.0
	CBitmapManager();
	~CBitmapManager();

    bool load(HINSTANCE hInst,const char *szFileName);
    void clear(void);

    LBitmapInfo at(int nIndex);
    HBITMAP atBitmap(int nIndex);

    inline int size(void) const { return m_vecBitmaps.size(); }

    inline std::vector<BitmapInfo>& getBitmapArray(void)
    { return m_vecBitmaps; }

    //������v1.0�汾
public:
	bool CreateBitmapFromFileList//���ļ��ж�ȡλͼ�ļ�������λͼ��Ϣ
	    (HINSTANCE hInstance,
        std::string strListName)
    { 
        return load(hInstance,strListName.c_str()); 
    }

    void DestroyHBitmapArray(){ clear(); }		//����λͼ�ṹ����

    LBitmapInfo GetAt(int nIndex){ return at(nIndex); }	//����λͼ�ṹ������

	int  GetCount(){return size();}
};