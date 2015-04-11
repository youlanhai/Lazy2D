//CGameMap.h
#pragma once
#include "BitmapManager.h"
#include "Scene.h"

class CGameObject;

class CMapNode
{
public:
	int nObstacleKind;		//�ϰ�����
	int nObjectIndex;		//������������
	CGameObject* pSprit;	//����
	CMapNode(int obstacle=0,int index=0,CGameObject* pObject=NULL)
	{
		nObstacleKind=obstacle;
		nObjectIndex=index;
		pSprit=pObject;
	}
};

/***************************
��ͼ�ࣺ
�����ͼ�ļ��б�
��ͼ����Ŀ���
��ͼ������С���
****************************/
class CGameMap
{
public:
	CGameMap();
	~CGameMap();
	bool Initialize//��ʼ����ͼ
	(
		HINSTANCE hInstance,
		std::string strObjectBmpFileName,
		std::string strGroundBmpFileName
	);
	bool ReadMapFromFile(std::string strMapName);//����ͼ
	void Destroy();
	void Show2(HDC hDC,CGameObject* player,RECT* clientRC);//��ʾ��ͼ�������������ʾ
	int  GetAt(int nXIndex,int nYIndex);
	CGameObject* GetGameObject(int nXIndex,int nYIndex);//��÷����еľ���
	bool IsSquareCanPass(int nXIndex,int nYIndex);//�жϴ˴������Ƿ��ͨ��
	bool IsThereCanPass(POINT* ptPosition);//�жϴ˴�����λ���Ƿ��ͨ��
	void AddGameObject(CGameObject* pSprit);//���ͼ�����һ���������Ϣ
	void RemoveGameObject(CGameObject* pSprit);//�ӵ�ͼ��ɾ��һ���������Ϣ
	void SetShowObstacle(bool bShow){bShowObstacle=bShow;}

	inline bool GetEnable(void){ return bEnable; }
public:
	int nMapWidth;				//��ͼ���
	int nMapHeight;				//��ͼ�߶�
	int nSquareWidth;			//��ͼ������
	int nSquareHeight;			//��ͼ����߶�
	int nXCount;				//��ͼ��������
	int nYCount;				//��ͼ��������
	int	nGroundType;			//��������
	CBitmapManager bmpGround;	//����λͼ������
	CBitmapManager bmpObject;	//����λͼ������
private:
	CMapNode**	ppObjectArray;	//��������
	bool		bShowObstacle;	//��ʾ�ϰ�
	bool		bEnable;//�Ƿ����
public:
	/***************************����Ϊ��ͼ�༭����***********************/
	void CreateMap												//������ͼ
	(
		int mapWidth,
		int mapHeight,
		int squareWidth=32,
		int squareHeight=16
	);
	bool WriteMapToFile(std::string strMapName);				//д��ͼ
	void AddObstacle(int xIndex,int yIndex);					//����ϰ�
	void AddObject(int xIndex,int yIndex,int nObjectIndex);		//��ӵ�������
	void SetGroundType(int nIndex);								//���õ�������
	void Show(HDC hDC,int mapX,int mapY,RECT* clientRC);		//��ʾ��ͼ
	/***************************����Ϊ��ͼ�༭����***********************/
};
//typedef CGameMap CMap;