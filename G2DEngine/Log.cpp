#include <windows.h>
#include "Log.h"

CLog g_log;

CLog::CLog(void)
{
	m_bUsefull=false;
}

CLog::~CLog(void)
{
	if(m_bUsefull)
	{
		GetLocalTime(&m_systm);
		char buffer[50];
		sprintf_s(buffer,50,"---[%d-%d-%d]日志系统停止---\n",m_systm.wYear,m_systm.wMonth,m_systm.wDay);
		write(buffer);
		m_fLog.close();
	}
}
bool CLog::init(const char *szLogName)
{
	if(m_bUsefull)
	{
		return false;
	}
	m_fLog.open(szLogName);
	if(m_fLog.bad() || m_fLog.fail())
	{
		return false;
	}
	m_bUsefull=true;
	GetLocalTime(&m_systm);
	char buffer[50];
	sprintf_s(buffer,50,"---[%d-%d-%d]日志系统启动---\n",m_systm.wYear,m_systm.wMonth,m_systm.wDay);
	write(buffer);
	return true;
}
void CLog::write(const char *szBuffer,bool bApendTime)
{
	GetLocalTime(&m_systm);
	char buffer[50];
	sprintf_s(buffer,50,"[%d:%d:%d]:",m_systm.wHour,m_systm.wMinute,m_systm.wSecond);
	if (bApendTime)
	{
		m_fLog<<buffer<<szBuffer<<std::endl;
	}
	else
	{
		m_fLog<<szBuffer<<std::endl;
	}
}
void CLog::writex(char *format, ...  )
{
    const int MAX_SIZE = 10000;
    static char szBuffer[MAX_SIZE];
    va_list vl;
    va_start (vl, format);

    vsnprintf_s (szBuffer, MAX_SIZE,MAX_SIZE, format, vl);
    write(szBuffer);

    va_end (vl);     
}