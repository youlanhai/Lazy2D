#pragma once

#include "GDI.h"

class CFPS;
class CKeyboard;

class CApp
{
public:
	CApp(void);
	~CApp(void);
	
    //��Ϸ��ʼ��
    bool create(HINSTANCE hInstance,
        const std::string & caption, int width, int height,
        bool bFullScreen = false);

	void mainLoop();				//��Ϣѭ��
    virtual bool onWindowMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){return 0;}

    void getClientRect(RECT *pRC) const;

    int getWidth() const { return m_nWidth; }
    int getHeight() const { return m_nHeight; }

protected:
    virtual void onRegisterClass(WNDCLASSEX *pwc){}

    virtual bool onInit() = 0;  //��ʼ����Ϸ
	virtual void onTick() = 0;	//����
	virtual void onDraw() = 0;	//��Ⱦ
	virtual void onClose() = 0;	//������Դ

private:
	void registerClass();	//ע�ᴰ����
	bool createWindow();	//��������

protected:
    CDevice		    m_dcDevice;			//�豸������
	HINSTANCE		m_hInstance;			//Ӧ�ó���ʵ������
	HWND			m_hWnd;					//���ھ��
	int				m_nWidth;					//���ڿ��
	int				m_nHeight;					//���ڸ߶�
	std::string		m_caption;			//���ڱ���
	bool			m_bFullScreen;

    CFPS*           m_pFPS;
    CKeyboard*      m_pKeyboard;
};
