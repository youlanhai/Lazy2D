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
//���ݳ�Ա
	int		n;	//��������
	std::vector<std::string> args;//�����б�
	void	*pExParam;	//���Ӳ���
};

//�ű�������
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
	bool			m_bRunning;				//�������Ƿ���������
	bool			m_bUsefull;				//��ǰ����Ƿ����
	bool			m_bExecuteFinished;		//��ǰ����Ƿ�ִ�����
	std::ifstream	m_fstream;				//��ǰ�ű��ļ���
	char			m_szBuffer[256];		//��ǰ��仺����
	int				m_nCurRow;				//��ǰ����
	std::string		m_strObject;			//��ǰ����漰�Ķ�����
	std::string		m_strFunction;			//��ǰ����漰�ĺ�����
	ScriptArgList	m_argList;				//��ǰ����漰�Ĳ����б�
	void			*m_pExParam;			//���Ӳ���
	std::map<std::string,PScriptFunc>	m_mapFunction;//����ӳ���
public:
	static bool sampleFunction(ScriptArgList *pArgs);
};
