#pragma warning(disable:4996)
#include "Commen.h"
#include "ScriptManager.h"
#include <cctype>
#include <iostream>
using namespace std;


#ifndef SCRIPTMANAGERTEST
ofstream g_out("scriptLog.txt");
#else
#define g_out cout
#endif

char outputBuffer[256];
void OutputInfo(char *str)
{
	g_out<<str<<endl;
}


CScriptManager::CScriptManager(void)
{
	m_bRunning=false;
	m_bExecuteFinished=true;
	m_bUsefull=false;
	m_nCurRow=0;
	m_pExParam=NULL;
	memset(m_szBuffer,0,sizeof(m_szBuffer));
	registerFunction("sampleFunction",sampleFunction);
}

CScriptManager::~CScriptManager(void)
{
}
bool CScriptManager::loadScript(char *szFileName)
{
	unloadScript();
	sprintf(outputBuffer,"正在加载脚本%s...",szFileName);
	OutputInfo(outputBuffer);
	m_fstream.open(szFileName);
	if (m_fstream.fail())
	{
		OutputInfo("加载脚本失败!");
		return false;
	}
	m_bRunning=true;
	m_nCurRow=0;
	return true;
}
bool CScriptManager::unloadScript()
{
	if (m_bRunning)
	{
		m_bRunning=false;
		m_fstream.close();
		m_argList.clear();
		OutputInfo("脚本已卸载。");
	}
	memset(m_szBuffer,0,sizeof(m_szBuffer));
	m_bExecuteFinished=true;
	m_strFunction="";
	m_strObject="";
	m_bUsefull=false;
	m_nCurRow=0;
	m_pExParam=NULL;
	return true;
}
bool CScriptManager::registerFunction(std::string strName,PScriptFunc pFunc)
{
	if (m_mapFunction[strName] != NULL)//已经存在
	{
		return false;
	}
	else
	{
		m_mapFunction[strName] = pFunc;
		return true;
	}
}
bool CScriptManager::unregisterFunction(std::string strName)
{
	map<string,PScriptFunc>::iterator it=m_mapFunction.find(m_strFunction);
	if (it == m_mapFunction.end())
	{
		return false;
	}
	else
	{
		m_mapFunction.erase(it);
		return true;
	}
}
bool CScriptManager::run()
{
	if(!m_bRunning)
	{
		return false;
	}
	if (m_bExecuteFinished)
	{
		read();
		anylise();
	}
	execute();
	if (isEnd() && m_bExecuteFinished)
	{
		unloadScript();
	}
	return true;
}
bool CScriptManager::isEnd()
{
	if (m_fstream.bad() ||m_fstream.fail() || m_fstream.eof())
	{
		return true;
	}
	else return false;
}
bool CScriptManager::read()
{
	if (m_bRunning)
	{
		m_argList.clear();
		m_strFunction.clear();
		m_strObject.clear();
		m_bUsefull=true;
		while(m_bUsefull)
		{
			m_fstream.getline(m_szBuffer,256);
			++m_nCurRow;
			if (m_fstream.bad() || m_fstream.fail())
			{
				m_bUsefull=false;
				sprintf(outputBuffer,"读取脚本失败：第%d行。", m_nCurRow);
				OutputInfo(outputBuffer);
			}
			else
			{
				if(strlen(m_szBuffer)>0 && m_szBuffer[0]!='#')//忽略空行和注释行
				{
					break;
				}
			}
		}
	}
	else m_bUsefull=false;
	return m_bUsefull;
}
bool CScriptManager::anylise()
{
	if (m_bRunning && m_bUsefull)
	{
		int strType=0;//1数字,2字符串,3变量
		bool bDot=false;//是否有'.'
		int kuo=0;//-1有左括号,0无括号,1有右括号
		int yin=0;//-1有左单引号
		int syin=0;//-1有左双引号
		bool bFunc=false;//有函数
		string strTemp;
		int i=0;
		char ch=m_szBuffer[0];
		while (ch!='\n' && ch!='\0' && i<256)
		{
			if( ' '==ch && 2!=strType)
			{
				if (!bFunc && !strTemp.empty())//不是参数,没有函数，则添加函数
				{
					bFunc=true;
					m_strFunction=strTemp;
					strTemp.clear();
				}
				else if (strTemp.empty())//有连续的空格符
				{
					continue;
				}
				else//出错。出现了两个函数.
				{
					m_bUsefull=false;
					sprintf(outputBuffer,"脚本错误：第%d行，%s，非法空格。", m_nCurRow,m_szBuffer);
					break;
				}
			}
			else if(','==ch && 2!=strType)
			{
				if (-1 == kuo)//有左括号。是参数
				{
					++m_argList.n;
					m_argList.args.push_back(strTemp);
					strTemp.clear();
				}
				else//无左括号。不是参数，出错。
				{
					m_bUsefull=false;
					sprintf(outputBuffer,"脚本错误：第%d行，%s，非法','。", m_nCurRow,m_szBuffer);
					break;
				}
			}
			else if( '('==ch  && 2!=strType)
			{
				if(!bFunc && !strTemp.empty())
				{
					kuo=-1;
					bFunc=true;
					m_strFunction=strTemp;
					strTemp.clear();
				}
				else if(!bFunc)//此语句无函数
				{
					m_bUsefull=false;
					sprintf(outputBuffer,"脚本错误：第%d行，%s，非法'('。", m_nCurRow,m_szBuffer);
					break;
				}
			}
			else if (')'==ch && 2!=strType)
			{
				if(!bFunc)//本语句无函数
				{
					m_bUsefull=false;
					sprintf(outputBuffer,"脚本错误：第%d行，%s，非法')'。", m_nCurRow,m_szBuffer);
					break;
				}
				if(!strTemp.empty())
				{
					++m_argList.n;
					m_argList.args.push_back(strTemp);
					strTemp.clear();
				}
				kuo=1;//有右括号
				break;//语句结束
			}
			else if ('\"' == ch)
			{
				if(2 == strType)
				{
					strType=0;
				}
				else
				{
					if (!strTemp.empty())
					{
						m_bUsefull=false;
						sprintf(outputBuffer,"脚本错误：第%d行，%s，非法'\"'。", m_nCurRow,m_szBuffer);
						break;
					}
					strType=2;
				}
			}
			else
			{
				strTemp+=ch;
			}
			ch=m_szBuffer[++i];
		}
		if(m_bUsefull && kuo!=1)
		{
			m_bUsefull=false;
			sprintf(outputBuffer,"脚本错误：第%d行，%s，无右括号。", m_nCurRow,m_szBuffer);
		}
		if (!m_bUsefull)
		{
			OutputInfo(outputBuffer);
		}
	}
	else
	{
		m_bUsefull=false;
	}
	return m_bUsefull;
}

bool CScriptManager::execute()
{
	if(m_bRunning && m_bUsefull)
	{
		map<string,PScriptFunc>::iterator it=m_mapFunction.find(m_strFunction);
		if (it != m_mapFunction.end())
		{
			PScriptFunc pFunc=it->second;
			if (pFunc != NULL)
			{
				m_argList.pExParam=m_pExParam;
				m_bExecuteFinished=(*pFunc)(&m_argList);//执行函数
				return true;
			}
		}
		else//不存在此函数
		{
			sprintf(outputBuffer,"脚本错误：第%d行，%s，非法函数%s", m_nCurRow,m_szBuffer,m_strFunction.c_str());
			OutputInfo(outputBuffer);
		}
	}
	return false;//运行失败
}

bool CScriptManager::sampleFunction(ScriptArgList *pArgs)
{
	g_out<<"\n这是样例函数。正在运行...\n"<<endl;
	return true;//如果此函数需要循环执行，请返回false。否则请总是返回ture。
}