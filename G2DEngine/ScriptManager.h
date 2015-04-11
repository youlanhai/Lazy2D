#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <map>

struct ScriptArgList 
{
	ScriptArgList(void)
	{
		n=0;
		pExParam=NULL;
	}
	void clear()
	{
		n=0;
		args.clear();
		pExParam=NULL;
	}
//数据成员
	int		n;	//参数个数
	std::vector<std::string> args;//参数列表
	void	*pExParam;	//附加参数
};

//脚本控制器
class CScriptManager
{
public:
	typedef bool (*PScriptFunc)(ScriptArgList *pArgs);
	CScriptManager(void);
	~CScriptManager(void);
	bool loadScript(char *szFileName);
	bool unloadScript();
	bool run();
	bool registerFunction(std::string strName,PScriptFunc pFunc);
	bool unregisterFunction(std::string strName);
	inline void setParam(void *pParam){ m_pExParam=pParam; }
protected:
	bool isEnd();
	bool read();
	bool anylise();
	bool execute();
protected:
	bool			m_bRunning;				//控制器是否正在运行
	bool			m_bUsefull;				//当前语句是否可用
	bool			m_bExecuteFinished;		//当前语句是否执行完成
	std::ifstream	m_fstream;				//当前脚本文件流
	char			m_szBuffer[256];		//当前语句缓冲区
	int				m_nCurRow;				//当前行数
	std::string		m_strObject;			//当前语句涉及的对象名
	std::string		m_strFunction;			//当前语句涉及的函数名
	ScriptArgList	m_argList;				//当前语句涉及的参数列表
	void			*m_pExParam;			//附加参数
	std::map<std::string,PScriptFunc>	m_mapFunction;//函数映射表
public:
	static bool sampleFunction(ScriptArgList *pArgs);
};
