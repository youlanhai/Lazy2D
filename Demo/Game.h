//Game.h
#pragma once
#include "../G2DEngine/App.h"

class CFPS;
class CKeyboard;

/****************************************************
游戏类
游戏初始化，游戏主进程等。
******************************************************/
class CGame : public CApp
{
public:
    CGame();						//构造函数
    ~CGame();						//析构函数

private:
    void onRegisterClass(WNDCLASSEX *pwc);	//注册窗口类
    bool onInit();		//初始化游戏
    void onDraw();			//渲染
    void onTick();			//更新
    void onClose();			//销毁资源

    void renderText();
};