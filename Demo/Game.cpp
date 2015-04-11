//Game.cpp
#include "../G2DEngine/Commen.h"
#include "../G2DEngine/FPS.h"
#include "../G2DEngine/Keyboard.h"

#include "Game.h"
#include "resource.h"

///////////////////������///////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    changeWorkDirToModulePath();

    CGame *pGame = new CGame();
    if (pGame->create(hInstance,
        "G2DEngine(http://blog.csdn.net/you_lan_hai)",
        640, 480))//��ʼ����Ϸ��
    {
        pGame->mainLoop();//��Ϸѭ��
    }
    delete pGame;
    return 0;
}
////////////////class CGame start//////////////////////////////

//ע�ᴰ����
void CGame::onRegisterClass(WNDCLASSEX *pwc)
{
    pwc->hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_ICON1));
    pwc->hCursor = LoadCursor(NULL, IDC_ARROW);
}

CGame::CGame()
{
}
CGame::~CGame()
{
}

void CGame::onClose()
{
}

bool CGame::onInit()
{
    return true;
}

void CGame::onTick()
{
    m_pFPS->updateFPS();
    m_pKeyboard->update();
}

void CGame::onDraw()
{
    CRect clientRect;
    getClientRect(&clientRect);
    m_dcDevice.fillSolidRect(&clientRect, 0xffa0a0a0);
    ////////////////////////////��ʼ����///////////////////////

    char *text = "ESC to quit game.";
    m_dcDevice.textOut(100, 100, text, strlen(text));

    renderText();
    ////////////////////////////�ύ��ǰ̨/////////////////////
    m_dcDevice.present();
}

void CGame::renderText()
{
    char buffer[64];
    sprintf(buffer, "fps:%.2f", m_pFPS->getFPS());
    m_dcDevice.textOut(0, 0, buffer, strlen(buffer));
}

//////////////////class Game end//////////////////////////////