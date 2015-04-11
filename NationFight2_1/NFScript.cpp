#include "../G2DEngine/Commen.h"
#include "NFScript.h"
#include "Enemy.h"
#include "Game.h"

extern CGame g_game;

std::vector<Enemy*> CNFScript::enemyTemplate=std::vector<Enemy*>(100,NULL);

CNFScript::CNFScript(void)
{
    registerFunction("SendMessage",sendMessage);
	registerFunction("NewEnemy",newEnemy);
	registerFunction("AddEnemy",addEnemy);
	registerFunction("Waite",waite);
}

CNFScript::~CNFScript(void)
{
	for (std::vector<Enemy*>::iterator it=enemyTemplate.begin();
		it != enemyTemplate.end();
		++it)
	{
		delete (*it);
	}
	enemyTemplate.clear();
}

bool CNFScript::sendMessage(ScriptArgList *pArgs)
{
    int index = atoi(pArgs->args[0].c_str());
    switch(index)
    {
    case 10 ://bossÀ´À²£¡
        g_game.m_pMusic->stopAll();
        g_game.m_nMusicID = 2;
        break;
    default: break;
    }
    return true;
}

bool CNFScript::newEnemy(ScriptArgList *pArgs)
{
	int index = atoi(pArgs->args[0].c_str());
	int showID = atoi(pArgs->args[1].c_str());
	int level= atoi(pArgs->args[2].c_str());
	int vx=atoi(pArgs->args[3].c_str());
	int vy=atoi(pArgs->args[4].c_str());
	int life=atoi(pArgs->args[5].c_str());
	int attack=atoi(pArgs->args[6].c_str());
	int defence=atoi(pArgs->args[7].c_str());

	Enemy *pEnemy=enemyTemplate.at(index);
	SafeDelete(pEnemy);
	enemyTemplate[index]=new Enemy(showID,level,0,0,vx,vy,life,attack,defence);
	return true;
}

bool CNFScript::addEnemy(ScriptArgList *pArgs)
{
	int index = atoi(pArgs->args[0].c_str());
	Enemy *pEnemy=enemyTemplate.at(index);
	if (pEnemy != NULL)
	{
		Enemy *pE = new Enemy(*pEnemy);
		pE->x = rand()%g_game.getWidth();
		pE->y = -pE->h;
		g_game.m_enemyManager.add(pE);
	}
	return true;
}

bool CNFScript::waite(ScriptArgList *pArgs)
{
	int t=atoi(pArgs->args[0].c_str());
	t -= g_game.m_updateTimeInterval;
	if (t<=0)
	{
		return true;
	}
	else
	{
		char buffer[16];
		pArgs->args[0]=itoa(t,buffer,10);
		return false;
	}
}