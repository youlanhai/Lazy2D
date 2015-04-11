//Scene.h
#pragma once

/**************************
开关类：
**************************/
class CSwitchField
{
public:
	bool		bUsefull;		//开关是否可用
	CGameRect	rect;			//开关区域
	POINT		newPos;			//进入下个场景后的坐标
	std::string newSceneName;	//新场景的名称
	CSwitchField()
	{
		rect.x=rect.y=0;
		rect.w=rect.h=0;//两个方格大小
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
场景类:
************************/
class CScene
{
public:
	CScene();
	~CScene();
	bool ReadFromFile(std::string fileName);
	bool WriteToFile(std::string fileName);	
	std::string mapName;	//地图文件名
	std::string	spritName;	//精灵管理器文件名
	std::string speakName;	//对话文件名
	std::string backMusic;	//背景音乐文件名
	CSwitchField	swField[4];	//一个地图允许有4个开关区域
	POINT		ptPlayerPosition;
	bool CheckSwitchScene(POINT* ptPosition);
};