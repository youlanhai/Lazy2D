//Game.h
#pragma once
#include "../G2DEngine/App.h"

class CFPS;
class CKeyboard;

/****************************************************
��Ϸ��
��Ϸ��ʼ������Ϸ�����̵ȡ�
******************************************************/
class CGame : public CApp
{
public:
    CGame();						//���캯��
    ~CGame();						//��������

private:
    void onRegisterClass(WNDCLASSEX *pwc);	//ע�ᴰ����
    bool onInit();		//��ʼ����Ϸ
    void onDraw();			//��Ⱦ
    void onTick();			//����
    void onClose();			//������Դ

    void renderText();
};