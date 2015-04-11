#include "GameScript.h"
#include <iostream>
using namespace std;

CGameScript::CGameScript(void)
{
	m_pGame=NULL;
}

CGameScript::~CGameScript(void)
{
}

bool CGameScript::init(CGame *pGame)
{
	m_pGame=pGame;
	m_mapFunction["sampleFunction"]=&CGameScript::sampleFunction;
	m_mapFunction["say"]=&CGameScript::say;
	m_mapFunction["moveTo"]=&CGameScript::moveTo;
	return true;
}
bool CGameScript::execute()
{
	if(m_bRunning && m_bUsefull)
	{
		map<string,pScriptFunc>::iterator it=m_mapFunction.find(m_strFunction);
		if (it != m_mapFunction.end())
		{
			pScriptFunc pFunc=it->second;
			if (pFunc != NULL)
			{
				m_bExecuteFinished=false;
				(this->*pFunc)();
				return true;
			}
		}
		else
		{
// 			sprintf(outputBuffer,"�ű����󣺵�%d�У�%s���Ƿ�����%s", m_nCurRow,m_szBuffer,m_strFunction.c_str());
// 			OutputInfo(outputBuffer);
		}
	}
	m_bExecuteFinished=true;
	return false;
}
void CGameScript::sampleFunction(void)
{
	cout<<"����������������..."<<endl;
	m_bExecuteFinished=true;
}
void CGameScript::say(void)
{
	if (m_argList.n>=1)
	{
		cout<<"˵:"<<m_argList.args[0]<<endl;
	}
	else
	{
// 		sprintf(outputBuffer,"�ű����󣺵�%d�У���������", m_nCurRow,m_strFunction.c_str());
// 		OutputInfo(outputBuffer);
	}
	m_bExecuteFinished=true;
}
void CGameScript::moveTo(void)
{
	if (m_argList.n>=2)
	{
		float x=atof(m_argList.args[0].c_str());
		float y=atof(m_argList.args[1].c_str());
		cout<<"���ƶ�������:("<<x<<","<<y<<")��"<<endl;
	}
	else
	{
// 		sprintf(outputBuffer,"�ű����󣺵�%d�У���������", m_nCurRow,m_strFunction.c_str());
// 		OutputInfo(outputBuffer);
	}
	m_bExecuteFinished=true;
}
