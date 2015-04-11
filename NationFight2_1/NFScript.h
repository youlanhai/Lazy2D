#pragma once

#include "../G2DEngine/ScriptManager.h"

#include <vector>

class Enemy;

class CNFScript :
	public CScriptManager
{
public:
	CNFScript(void);
	~CNFScript(void);

    //敌机模板
	static std::vector<Enemy*> enemyTemplate;

    //发送消息
    static bool sendMessage(ScriptArgList *pArgs);

    //新建敌人
	static bool newEnemy(ScriptArgList *pArgs);

    //添加敌人
	static bool addEnemy(ScriptArgList *pArgs);

    //等待，以毫秒为单位
	static bool waite(ScriptArgList *pArgs);
};
