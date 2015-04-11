//App.cpp
#include "Commen.h"
#include "App.h"
#include "Log.h"
#include "FPS.h"
#include "Keyboard.h"

CApp * g_pApp = NULL;
const char * g_pClassName = "G2DEngine";

//窗口过程
static LRESULT CALLBACK windowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (g_pApp != NULL)
    {
        if (g_pApp->onWindowMessage(hWnd, msg, wParam, lParam))
        {
            return 0;
        }
    }

    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            DestroyWindow(hWnd);
            break;
        }
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

///////////////////class CApp start/////////////////////////
CApp::CApp()
{
    assert(g_pApp == NULL);
    g_pApp = this;

    m_hWnd = NULL;
    m_hInstance = NULL;

    m_pFPS = new CFPS();
    m_pKeyboard = new CKeyboard();
}

CApp::~CApp()
{
    UnregisterClass(g_pClassName, m_hInstance);

    SafeDelete(m_pFPS);
    SafeDelete(m_pKeyboard);

    if (g_pApp == this)
    {
        g_pApp = NULL;
    }
}

/* 游戏类初始化
** hInstance: 应用程序实例对象
** caption: 窗口标题
** width: 窗口宽度
** height: 窗口高度
** bFullScreen: 是否全屏
*/
bool CApp::create(HINSTANCE hInstance,
    const std::string & caption,
    int width,
    int height,
    bool bFullScreen)
{
    m_hInstance = hInstance;
    m_nWidth = width;
    m_nHeight = height;
    m_bFullScreen = bFullScreen;
    m_caption = caption;

    registerClass();//注册窗口类
    if (!createWindow())//创建窗口
    {
        g_log.write("创建窗口失败!");
        return false;
    }

    m_dcDevice.createDC(NULL);
    m_dcDevice.init(m_hWnd);
    m_dcDevice.setTextColor(RGB(255, 0, 0));
    m_dcDevice.setBkMode(TRANSPARENT);

    m_pFPS->init();

    if (!onInit())//初始化其他
    {
        onClose();

        DestroyWindow(m_hWnd);
        m_hWnd = 0;

        g_log.write("初始化游戏失败!");
        return false;
    }

    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);
    return true;
}
//创建窗口
bool CApp::createWindow()
{
    int x = (GetSystemMetrics(SM_CXSCREEN) - m_nWidth) / 2;
    int y = (GetSystemMetrics(SM_CYSCREEN) - m_nHeight) / 2;
    m_hWnd = CreateWindow(
        g_pClassName,
        m_caption.c_str(),
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        x,
        y,
        m_nWidth,
        m_nHeight,
        NULL,
        NULL,
        m_hInstance,
        NULL);

    return m_hWnd != NULL;
}

//注册窗口类
void CApp::registerClass()
{
    WNDCLASSEX wc;
    memset(&wc, 0, sizeof(wc));

    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = (WNDPROC) windowProc;
    wc.hInstance = m_hInstance;
    wc.lpszClassName = g_pClassName;
    wc.style = CS_CLASSDC;

    onRegisterClass(&wc);

    if (wc.hbrBackground == 0)
    {
        wc.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    }

    if (wc.hIcon == 0)
    {
        wc.hIcon = LoadIcon(NULL, IDI_INFORMATION);
    }

    if (wc.hIconSm == 0)
    {
        wc.hIconSm = wc.hIcon;
    }

    if (wc.hCursor == 0)
    {
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    }

    RegisterClassEx(&wc);
}

//消息循环
void CApp::mainLoop()
{
    MSG msg = { 0 };
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            if (GetActiveWindow() == m_hWnd)
            {
                Sleep(1);
            }

            onTick();
            onDraw();
        }
    }

    onClose();
}

void CApp::getClientRect(RECT *pRC) const
{
    ::GetClientRect(m_hWnd, pRC);
}

///////////////////class CApp end/////////////////////////