//CSpeaker.h
#pragma once
#include "BitmapManager.h"

//�Ի���㣬����һ�ζԻ���Ϣ
class  CSpeakNode
{
public:
	int id;				//˵������id
	char words[256];	//˵������
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
�Ի�ϵͳ����������Ի���ʾ
�÷�:ÿ�μ�������Ի�ʱ�������ReadFromFile��
��ʼ���Ի�����λͼ,�ͶԻ��ı���
**********************************************/
class CSpeaker
{
public:
	CSpeaker();
	~CSpeaker();
	void Clear();//���ϵͳ����
	bool ReadFromFile(HINSTANCE hInst,std::string bmpFile,std::string speakerFile);
	int  Show(HDC hDC,RECT* rc);//��ʾ�ı�
	void AddPtr(){++currentPtr;}//�����α�

    bool IsFinished() const { return currentPtr >= size;}

private:
	int size;							//�Ի�����
	int currentPtr;						//�αꡣָʾ��ǰ�Ի���λ��
	std::vector<CSpeakNode> wordArry;	//�Ի�����
	CBitmapManager bmpManager;			//�Ի�����ͷ��
};