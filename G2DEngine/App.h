#pragma once

#include "GDI.h"

class CFPS;
class CKeyboard;

class CApp
{
public:
	CApp(void);
	~CApp(void);
	
    //游戏初始化
    bool create(HINSTANCE hInstance,
        const std::string & caption, int width, int height,
        bool bFullScreen = false);

	void mainLoop();				//消息循环
    virtual bool onWindowMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){return 0;}

    void getClientRect(RECT *pRC) const;

    int getWidth() const { return m_nWidth; }
    int getHeight() const { return m_nHeight; }

protected:
    virtual void onRegisterClass(WNDCLASSEX *pwc){}

    virtual bool onInit() = 0;  //初始化游戏
	virtual void onTick() = 0;	//更新
	virtual void onDraw() = 0;	//渲染
	virtual void onClose() = 0;	//销毁资源

private:
	void registerClass();	//注册窗口类
	bool createWindow();	//创建窗口

protected:
    CDevice		    m_dcDevice;			//设备上下文
	HINSTANCE		m_hInstance;			//应用程序实例对象
	HWND			m_hWnd;					//窗口句柄
	int				m_nWidth;					//窗口宽度
	int				m_nHeight;					//窗口高度
	std::string		m_caption;			//窗口标题
	bool			m_bFullScreen;

    CFPS*           m_pFPS;
    CKeyboard*      m_pKeyboard;
};
