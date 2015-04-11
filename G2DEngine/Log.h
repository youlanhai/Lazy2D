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
	std::ofstream	m_fLog;		//����ļ���
	bool			m_bUsefull;	//��־ϵͳ�Ƿ����
	SYSTEMTIME		m_systm;
};

extern CLog g_log;
