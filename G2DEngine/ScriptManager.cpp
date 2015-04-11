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
	sprintf(outputBuffer,"���ڼ��ؽű�%s...",szFileName);
	OutputInfo(outputBuffer);
	m_fstream.open(szFileName);
	if (m_fstream.fail())
	{
		OutputInfo("���ؽű�ʧ��!");
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
		OutputInfo("�ű���ж�ء�");
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
	if (m_mapFunction[strName] != NULL)//�Ѿ�����
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
				sprintf(outputBuffer,"��ȡ�ű�ʧ�ܣ���%d�С�", m_nCurRow);
				OutputInfo(outputBuffer);
			}
			else
			{
				if(strlen(m_szBuffer)>0 && m_szBuffer[0]!='#')//���Կ��к�ע����
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
		int strType=0;//1����,2�ַ���,3����
		bool bDot=false;//�Ƿ���'.'
		int kuo=0;//-1��������,0������,1��������
		int yin=0;//-1��������
		int syin=0;//-1����˫����
		bool bFunc=false;//�к���
		string strTemp;
		int i=0;
		char ch=m_szBuffer[0];
		while (ch!='\n' && ch!='\0' && i<256)
		{
			if( ' '==ch && 2!=strType)
			{
				if (!bFunc && !strTemp.empty())//���ǲ���,û�к���������Ӻ���
				{
					bFunc=true;
					m_strFunction=strTemp;
					strTemp.clear();
				}
				else if (strTemp.empty())//�������Ŀո��
				{
					continue;
				}
				else//������������������.
				{
					m_bUsefull=false;
					sprintf(outputBuffer,"�ű����󣺵�%d�У�%s���Ƿ��ո�", m_nCurRow,m_szBuffer);
					break;
				}
			}
			else if(','==ch && 2!=strType)
			{
				if (-1 == kuo)//�������š��ǲ���
				{
					++m_argList.n;
					m_argList.args.push_back(strTemp);
					strTemp.clear();
				}
				else//�������š����ǲ���������
				{
					m_bUsefull=false;
					sprintf(outputBuffer,"�ű����󣺵�%d�У�%s���Ƿ�','��", m_nCurRow,m_szBuffer);
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
				else if(!bFunc)//������޺���
				{
					m_bUsefull=false;
					sprintf(outputBuffer,"�ű����󣺵�%d�У�%s���Ƿ�'('��", m_nCurRow,m_szBuffer);
					break;
				}
			}
			else if (')'==ch && 2!=strType)
			{
				if(!bFunc)//������޺���
				{
					m_bUsefull=false;
					sprintf(outputBuffer,"�ű����󣺵�%d�У�%s���Ƿ�')'��", m_nCurRow,m_szBuffer);
					break;
				}
				if(!strTemp.empty())
				{
					++m_argList.n;
					m_argList.args.push_back(strTemp);
					strTemp.clear();
				}
				kuo=1;//��������
				break;//������
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
						sprintf(outputBuffer,"�ű����󣺵�%d�У�%s���Ƿ�'\"'��", m_nCurRow,m_szBuffer);
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
			sprintf(outputBuffer,"�ű����󣺵�%d�У�%s���������š�", m_nCurRow,m_szBuffer);
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
				m_bExecuteFinished=(*pFunc)(&m_argList);//ִ�к���
				return true;
			}
		}
		else//�����ڴ˺���
		{
			sprintf(outputBuffer,"�ű����󣺵�%d�У�%s���Ƿ�����%s", m_nCurRow,m_szBuffer,m_strFunction.c_str());
			OutputInfo(outputBuffer);
		}
	}
	return false;//����ʧ��
}

bool CScriptManager::sampleFunction(ScriptArgList *pArgs)
{
	g_out<<"\n����������������������...\n"<<endl;
	return true;//����˺�����Ҫѭ��ִ�У��뷵��false�����������Ƿ���ture��
}