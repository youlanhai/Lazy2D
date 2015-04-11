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

    //�л�ģ��
	static std::vector<Enemy*> enemyTemplate;

    //������Ϣ
    static bool sendMessage(ScriptArgList *pArgs);

    //�½�����
	static bool newEnemy(ScriptArgList *pArgs);

    //��ӵ���
	static bool addEnemy(ScriptArgList *pArgs);

    //�ȴ����Ժ���Ϊ��λ
	static bool waite(ScriptArgList *pArgs);
};
