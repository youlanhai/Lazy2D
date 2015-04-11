//Scene.h
#pragma once

/**************************
�����ࣺ
**************************/
class CSwitchField
{
public:
	bool		bUsefull;		//�����Ƿ����
	CGameRect	rect;			//��������
	POINT		newPos;			//�����¸������������
	std::string newSceneName;	//�³���������
	CSwitchField()
	{
		rect.x=rect.y=0;
		rect.w=rect.h=0;//���������С
		newPos.x=newPos.y=0;
		bUsefull=false;
	}
	void SetSwitch(int x,int y,int w,int h,int newPosX,int newPosY,std::string sceneName)
	{
		bUsefull=true;
		rect.x=x;
		rect.y=y;
		rect.w=w;
		rect.h=h;
		newPos.x=newPosX;
		newPos.y=newPosY;
		newSceneName=sceneName;
	}
};

/************************
������:
************************/
class CScene
{
public:
	CScene();
	~CScene();
	bool ReadFromFile(std::string fileName);
	bool WriteToFile(std::string fileName);	
	std::string mapName;	//��ͼ�ļ���
	std::string	spritName;	//����������ļ���
	std::string speakName;	//�Ի��ļ���
	std::string backMusic;	//���������ļ���
	CSwitchField	swField[4];	//һ����ͼ������4����������
	POINT		ptPlayerPosition;
	bool CheckSwitchScene(POINT* ptPosition);
};