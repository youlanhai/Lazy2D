#pragma once

#include "Utilities.h"

class CKeyboard : public IBaseObject
{
public:
	CKeyboard(void);
	~CKeyboard(void);
	
    void update();

    bool isKeyDown(DWORD vk) const
    {
        return isKeyPress(vk) && !isOldKeyPress(vk);
    }

    bool isKeyUp(DWORD vk) const
    {
        return !isKeyPress(vk) && isOldKeyPress(vk);
    }
    
    bool isKeyPress(DWORD vk) const
    {
        return (m_keyboard[vk] & 0x80) != 0;
    }

    bool isOldKeyPress(DWORD vk) const
    {
        return (m_oldKeyboard[vk] & 0x80) != 0;
    }

	inline BYTE* getKeyboard(void){ return m_keyboard; }

private:
	BYTE	m_keyboard[256];
    BYTE	m_oldKeyboard[256];
};
