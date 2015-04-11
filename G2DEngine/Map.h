//CGameMap.h
#pragma once
#include "BitmapManager.h"
#include "Scene.h"

class CGameObject;

class CMapNode
{
public:
	int nObstacleKind;		//障碍类型
	int nObjectIndex;		//地面物体索引
	CGameObject* pSprit;	//精灵
	CMapNode(int obstacle=0,int index=0,CGameObject* pObject=NULL)
	{
		nObstacleKind=obstacle;
		nObjectIndex=index;
		pSprit=pObject;
	}
};

/***************************
地图类：
构造地图文件列表：
地图方格的宽、高
地图方格的行、列
****************************/
class CGameMap
{
public:
	CGameMap();
	~CGameMap();
	bool Initialize//初始化地图
	(
		HINSTANCE hInstance,
		std::string strObjectBmpFileName,
		std::string strGroundBmpFileName
	);
	bool ReadMapFromFile(std::string strMapName);//读地图
	void Destroy();
	void Show2(HDC hDC,CGameObject* player,RECT* clientRC);//显示地图，包括精灵的显示
	int  GetAt(int nXIndex,int nYIndex);
	CGameObject* GetGameObject(int nXIndex,int nYIndex);//获得方格中的精灵
	bool IsSquareCanPass(int nXIndex,int nYIndex);//判断此处方格是否可通过
	bool IsThereCanPass(POINT* ptPosition);//判断此处坐标位置是否可通过
	void AddGameObject(CGameObject* pSprit);//向地图中添加一个精灵的信息
	void RemoveGameObject(CGameObject* pSprit);//从地图中删除一个精灵的信息
	void SetShowObstacle(bool bShow){bShowObstacle=bShow;}

	inline bool GetEnable(void){ return bEnable; }
public:
	int nMapWidth;				//地图宽度
	int nMapHeight;				//地图高度
	int nSquareWidth;			//地图方格宽度
	int nSquareHeight;			//地图方格高度
	int nXCount;				//地图方格列数
	int nYCount;				//地图方格行数
	int	nGroundType;			//地面类型
	CBitmapManager bmpGround;	//地面位图管理器
	CBitmapManager bmpObject;	//物体位图管理器
private:
	CMapNode**	ppObjectArray;	//地面物体
	bool		bShowObstacle;	//显示障碍
	bool		bEnable;//是否可用
public:
	/***************************以下为地图编辑器用***********************/
	void CreateMap												//创建地图
	(
		int mapWidth,
		int mapHeight,
		int squareWidth=32,
		int squareHeight=16
	);
	bool WriteMapToFile(std::string strMapName);				//写地图
	void AddObstacle(int xIndex,int yIndex);					//添加障碍
	void AddObject(int xIndex,int yIndex,int nObjectIndex);		//添加地面物体
	void SetGroundType(int nIndex);								//设置地面类型
	void Show(HDC hDC,int mapX,int mapY,RECT* clientRC);		//显示地图
	/***************************以上为地图编辑器用***********************/
};
//typedef CGameMap CMap;