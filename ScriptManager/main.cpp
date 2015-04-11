#include <iostream>
#include "../include/ScriptManager.h"
using namespace std;

bool say(ScriptArgList *pargs)
{
	cout<<"say:"<<pargs->args[0]<<endl;
	return true;
}

int main()
{
	CScriptManager *pScript=new CScriptManager();
	pScript->registerFunction("say",say);
	pScript->loadScript("script.txt");
	while(true)
	{
		if(!pScript->run())
		{
			cout<<"�ű�������ϡ�"<<endl;
			break;
		}
	}
	delete pScript;
	return 0;
}