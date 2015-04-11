#pragma once
#include "../include/scriptmanager.h"

class CGame;
class CGameScript;
typedef void (CGameScript::*pScriptFunc)(void) ;

class CGameScript :
	public CScriptManager
{
public:
	CGameScript(void);
	~CGameScript(void);
	bool init(CGame *pGame);
private:
	void sampleFunction(void);
	void say(void);
	void moveTo(void);
	bool execute();
private:
	CGame	*m_pGame;
	std::map<std::string,pScriptFunc>	m_mapFunction;//º¯ÊýÓ³Éä±í
};
