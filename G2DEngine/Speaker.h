//CSpeaker.h
#pragma once
#include "BitmapManager.h"

//对话结点，存贮一次对话信息
class  CSpeakNode
{
public:
	int id;				//说话人物id
	char words[256];	//说话内容
	CSpeakNode()
	{
		id=0;
		memset(words,0,sizeof(words));
	}
	CSpeakNode(const CSpeakNode& speak)
	{
		id=speak.id;
		strcpy(words,speak.words);
	}
};
/**********************************************
对话系统：控制人物对话显示
用法:每次加载人物对话时，需调用ReadFromFile，
初始化对话人物位图,和对话文本。
**********************************************/
class CSpeaker
{
public:
	CSpeaker();
	~CSpeaker();
	void Clear();//清空系统内容
	bool ReadFromFile(HINSTANCE hInst,std::string bmpFile,std::string speakerFile);
	int  Show(HDC hDC,RECT* rc);//显示文本
	void AddPtr(){++currentPtr;}//增加游标

    bool IsFinished() const { return currentPtr >= size;}

private:
	int size;							//对话数量
	int currentPtr;						//游标。指示当前对话的位置
	std::vector<CSpeakNode> wordArry;	//对话内容
	CBitmapManager bmpManager;			//对话人物头像
};