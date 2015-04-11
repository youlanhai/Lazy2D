//Keyboard.cpp 
#include <Windows.h>
#include "Keyboard.h"

CKeyboard::CKeyboard(void)
{
	memset(m_keyboard,0,sizeof(m_keyboard));
    memset(m_oldKeyboard,0,sizeof(m_oldKeyboard));
}

CKeyboard::~CKeyboard(void)
{
}

void CKeyboard::update()
{
    memcpy(m_oldKeyboard,m_keyboard,sizeof(m_keyboard));
	GetKeyboardState(m_keyboard);
}
