#pragma once

#include <fstream>

class CLog
{
public:
	CLog(void);
	~CLog(void);
	bool init(const char *szLogName);
	void write(const char *szBuffer,bool bApendTime=true);
    void writex(char *szBuffer, ... );
private:
	std::ofstream	m_fLog;		//输出文件流
	bool			m_bUsefull;	//日志系统是否可用
	SYSTEMTIME		m_systm;
};

extern CLog g_log;
