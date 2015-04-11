//Map.cpp
#include "Commen.h"
#include "GameObject.h"
#include "Map.h"

CGameMap::CGameMap()
{
	nMapWidth=0;	//��ͼ���
	nMapHeight=0;	//��ͼ�߶�
	nSquareWidth=0;	//��ͼ������
	nSquareHeight=0;//��ͼ����߶�
	nXCount=0;		//��ͼ��������
	nYCount=0;		//��ͼ��������
	ppObjectArray=NULL;
	nGroundType=0;
	bShowObstacle=false;
	bEnable=false;
}
CGameMap::~CGameMap()
{
	Destroy();
}
bool CGameMap::Initialize
(
	HINSTANCE hInstance,
	std::string strObjectBmpFileName,
	std::string strGroundBmpFileName
)
{
	if(!bmpObject.CreateBitmapFromFileList(hInstance,strObjectBmpFileName))
		return false;
	if(!bmpGround.CreateBitmapFromFileList(hInstance,strGroundBmpFileName))
		return false;
	return true;
}
bool CGameMap::ReadMapFromFile(std::string strMapName)
{
	//��ȡ�ϰ�����
	std::ifstream fin(strMapName.c_str());
	if(fin.fail())
		return false;
	Destroy();
	//��ȡ����Ŀ��ߡ�����������
	fin>>nSquareWidth>>nSquareHeight>>nXCount>>nYCount;
	nMapWidth=nSquareWidth*nXCount;
	nMapHeight=nSquareHeight*nYCount;
	//�����ά�ռ�
	int i,j;
	ppObjectArray=new CMapNode*[nYCount];
	for(i=0;i<nYCount;++i)
	{
		ppObjectArray[i]=new CMapNode[nXCount];
		memset(ppObjectArray[i],0,sizeof(CMapNode)*nXCount);
	}
	//��ȡ��������
	fin>>nGroundType;
	//��ȡ��������
	for(i=0;i<nYCount;++i)
	{
		for(j=0;j<nXCount;++j)
		{
			fin>>ppObjectArray[i][j].nObstacleKind>>ppObjectArray[i][j].nObjectIndex;
		}
	}
	fin.close();
	bEnable=true;
	return true;
}
void CGameMap::Destroy()
{
	if(ppObjectArray)//�ͷ�������������
	{
		for(int i=0;i<nYCount;++i)
			delete [] ppObjectArray[i];
		delete [] ppObjectArray;
		ppObjectArray=NULL;
	}
	nMapWidth=0;	//��ͼ���
	nMapHeight=0;	//��ͼ�߶�
	nSquareWidth=0;	//��ͼ������
	nSquareHeight=0;//��ͼ����߶�
	nXCount=0;		//��ͼ��������
	nYCount=0;		//��ͼ��������
	nGroundType=0;	//��������
	bShowObstacle=false;
	bEnable=false;
}
//��ʹ�ô˺���ǰ�������RemoveGameObject���Լ���ǰ��λ���ͷ�
void CGameMap::AddGameObject(CGameObject* pSprit)
{
	int i=pSprit->ptPosition.y/nSquareHeight;//������
	int j=pSprit->ptPosition.x/nSquareWidth;//������
	if(i>=0 && i<nYCount && j>=0 && j<nXCount)
	{
		ppObjectArray[i][j].pSprit=pSprit;
		ppObjectArray[i][j].nObstacleKind=1;//����ͨ��
	}
}
void CGameMap::RemoveGameObject(CGameObject* pSprit)
{
	if(this)
	{
		int i=pSprit->ptPosition.y/nSquareHeight;//������
		int j=pSprit->ptPosition.x/nSquareWidth;//������
		if(i>=0 && i<nYCount && j>=0 && j<nXCount)
		{
			if(ppObjectArray[i][j].pSprit==pSprit)
			{
				ppObjectArray[i][j].pSprit=NULL;
				ppObjectArray[i][j].nObstacleKind=0;
			}
		}
	}
}
int CGameMap::GetAt(int xIndex,int yIndex)
{
	if(yIndex<nYCount && yIndex>=0 && xIndex<nXCount && xIndex>=0)
		return ppObjectArray[yIndex][xIndex].nObstacleKind;//ppMapArray[yIndex][xIndex];
	else return -1;
}
CGameObject* CGameMap::GetGameObject(int xIndex,int yIndex)
{
	if(yIndex<nYCount && yIndex>=0 && xIndex<nXCount && xIndex>=0)
		return ppObjectArray[yIndex][xIndex].pSprit;
	else return NULL;
}
bool CGameMap::IsSquareCanPass(int xIndex,int yIndex)
{
	if(yIndex<nYCount && yIndex>=0 && xIndex<nXCount && xIndex>=0)
	{
		if(0==ppObjectArray[yIndex][xIndex].nObstacleKind)//if(0==ppMapArray[yIndex][xIndex])
			return true;
	}
	return false;
}
bool CGameMap::IsThereCanPass(POINT* ptPosition)
{
	int xIndex=ptPosition->x/nSquareWidth;//������
	int yIndex=ptPosition->y/nSquareHeight;//������
	return IsSquareCanPass(xIndex,yIndex);
}

void CGameMap::Show2(HDC hDC,CGameObject* player,RECT* rcClient)
{
	//��������Ŀ�Ⱥ͸߶�
	int screenWidth=rcClient->right - rcClient->left;
	int screenHeight=rcClient->bottom - rcClient->top;

	POINT screenPos;//��Ļ�����Ͻ�������ڵ�ͼ������
	screenPos.x=player->ptPosition.x - screenWidth/2;//�����ʼ����ʾ����Ļ����
	screenPos.y=player->ptPosition.y - screenHeight/2;
	
	POINT startPos;//��ͼҪ��ʾ����Ļ�ϵ��������Ͻ����ꡣ
	startPos.x=screenPos.x;
	startPos.y=screenPos.y;
	POINT endPos;//Ҫ��ʾ����Ļ�ϵ���������½�����.
	endPos.x=startPos.x+screenWidth+100;//+100Ϊƫ�������������屻��ǰ�ü�
	endPos.y=startPos.y+screenHeight+100;

	if(startPos.x<0)//��������߽�
	{
		startPos.x=0;
	}
	if(startPos.y<0)//�����ϱ߽�
	{
		startPos.y=0;
	}
	if(endPos.x>nMapWidth)//�����ұ߽�
	{
		endPos.x=nMapWidth;
	}
	if(endPos.y>nMapHeight)//�����±߽�
	{
		endPos.y=nMapHeight;
	}

	LBitmapInfo bmi=NULL;
	HDC memDC=CreateCompatibleDC(0);
	//���Ƶ���
	bmi=bmpGround.GetAt(nGroundType);
	SelectObject(memDC,bmi->hBitmap);
	//���㿪ʼ���Ƶ�λ�õ�����
	int startI=startPos.y/bmi->nHeight;
	int startJ=startPos.x/bmi->nWidth;
	//�����������λ�õ�����
	int endI=endPos.y/bmi->nHeight+1;
	int endJ=endPos.x/bmi->nWidth+1;
	int i,j,x,y,xx,yy;
	for(i=startI;i<endI;++i)
	{
		for(j=startJ;j<endJ;++j)
		{
			//Ҫ��ʾ����Ļ��λ��
			x=j*bmi->nWidth-screenPos.x;
			y=i*bmi->nHeight-screenPos.y;
			//Ҫ��ʾ�Ŀ�Ⱥ͸߶�
			xx=bmi->nWidth;
			yy=bmi->nHeight;
			//������Ĳ��ֲü���
			if(x+xx > endPos.x-screenPos.x)
				xx=endPos.x-screenPos.x-x;
			if(y+yy > endPos.y-screenPos.y)
				yy=endPos.y-screenPos.y-y;
			BitBlt(hDC,x,y,xx,yy,memDC,0,0,SRCCOPY);
		}
	}
	//���㿪ʼ���Ƶ�λ�õ�����
	startI=startPos.y/nSquareWidth;
	startJ=startPos.x/nSquareHeight;
	//�����������λ�õ�����
	endI=endPos.y/nSquareHeight;
	endJ=endPos.x/nSquareWidth;
	//��������
	for(i=startI;i<endI;++i)
	{
		for(j=startJ;j<endJ;++j)
		{
			if(bShowObstacle)
			{
				if(ppObjectArray[i][j].nObstacleKind!=0)//�����ϰ�
				{
					x=j*nSquareWidth-screenPos.x;
					y=i*nSquareHeight-screenPos.y;
					Rectangle(hDC,x,y,x+nSquareWidth,y+nSquareHeight);
				}
			}	//���ƾ���
			CGameObject* pSprit=ppObjectArray[i][j].pSprit;
			if(pSprit!=NULL && pSprit->nState!=0)
			{
				bmi=CGameObject::bmpManager.GetAt(pSprit->nShowID);
				if(bmi)
				{
					SelectObject(memDC,bmi->hBitmap);
					x=pSprit->ptPosition.x - screenPos.x;
					y=pSprit->ptPosition.y - screenPos.y;
					//����
					TransparentBlt(
						hDC,
						x - pSprit->nWidth/2,y - pSprit->nHeight,
						pSprit->nWidth,pSprit->nHeight,
						memDC,
						pSprit->ptPositionInPixture.x * pSprit->nWidth,
						pSprit->ptPositionInPixture.y * pSprit->nHeight,
						pSprit->nWidth,pSprit->nHeight,
						GetPixel(memDC,0,0)
						);
				}
			}
			if(ppObjectArray[i][j].nObjectIndex!=0)
			{
				bmi=bmpObject.GetAt(ppObjectArray[i][j].nObjectIndex-1);
				SelectObject(memDC,bmi->hBitmap);
				x=j*nSquareWidth-screenPos.x;
				y=i*nSquareHeight-screenPos.y;
				//���������ʱ�������������ƶ�
				if(bmi->nWidth > nSquareWidth)
					x=x - bmi->nWidth + nSquareWidth;
				//���������ʱ�������������ƶ�
				if(bmi->nHeight > nSquareHeight)
					y=y - bmi->nHeight + nSquareHeight;
				TransparentBlt(hDC,x,y,bmi->nWidth,bmi->nHeight,
					memDC,0,0,bmi->nWidth,bmi->nHeight,GetPixel(memDC,0,0));
			}
		}
	}
	DeleteDC(memDC);
}

/***************************����Ϊ��ͼ�༭����***********************/
void CGameMap::CreateMap												//������ͼ
(
	int mapWidth,
	int mapHeight,
	int squareWidth,
	int squareHeight
)
{
	Destroy();
	nSquareWidth=squareWidth;
	nSquareHeight=squareHeight;
	nXCount=mapWidth/nSquareWidth;
	nYCount=mapHeight/nSquareHeight;
	nMapWidth=nXCount*nSquareWidth;		//���¼����ͼ��Ⱥ͸߶ȣ�ȡ��
	nMapHeight=nYCount*nSquareHeight;
	//�����ά�ռ�
	int i;
	ppObjectArray=new CMapNode*[nYCount];
	for(i=0;i<nYCount;++i)
	{
		ppObjectArray[i]=new CMapNode[nXCount];
		memset(ppObjectArray[i],0,sizeof(CMapNode)*nXCount);
	}
	bEnable=true;
}

bool CGameMap::WriteMapToFile(std::string strMapName)//д��ͼ
{
	std::ofstream fout(strMapName.c_str());
	if(fout.bad())return false;
	//д�뷽��Ŀ��ߡ�����������
	fout<<nSquareWidth<<" "<<nSquareHeight<<" "<<nXCount<<" "<<nYCount<<std::endl;
	//д���������
	fout<<nGroundType<<std::endl;
	int i,j;
	//д����������
	for(i=0;i<nYCount;++i)
	{
		for(j=0;j<nXCount;++j)
		{
			fout<<ppObjectArray[i][j].nObstacleKind<<" "
				<<ppObjectArray[i][j].nObjectIndex<<" ";
		}
		fout<<std::endl;
	}
	fout.close();
	return true;
}
void CGameMap::AddObstacle(int xIndex,int yIndex)					//����ϰ�
{
	if(yIndex<nYCount && yIndex>=0 && xIndex<nXCount && xIndex>=0)
		ppObjectArray[yIndex][xIndex].nObstacleKind=!ppObjectArray[yIndex][xIndex].nObstacleKind;
}
void CGameMap::AddObject(int xIndex,int yIndex,int objectIndex)	//��ӵ�������
{
	if(yIndex<nYCount && yIndex>=0 && xIndex<nXCount && xIndex>=0)
		ppObjectArray[yIndex][xIndex].nObjectIndex=objectIndex;
}
void CGameMap::SetGroundType(int nIndex)							//���õ�������
{
	nGroundType=nIndex;
}
//mapX,mapYΪ��Ļ�ڵ�ͼ�ϵ�����
void CGameMap::Show(HDC hDC,int mapX,int mapY,RECT* rcClient)
{
	if (!bEnable)
	{
		return ;
	}
	LBitmapInfo bmi=NULL;
	HDC memDC=CreateCompatibleDC(0);
	//���Ƶ���
	bmi=bmpGround.GetAt(nGroundType);
	SelectObject(memDC,bmi->hBitmap);
	BitBlt(hDC,0,0,bmi->nWidth,bmi->nHeight,
		memDC,0,0,SRCCOPY);
	int x=nMapWidth/bmi->nWidth;//�����������ͼƬ�ĸ���
	int y=nMapHeight/bmi->nHeight;
	int i,j,xx,yy;
	for(i=0;i<y;++i)
	{
		for(j=0;j<x;++j)
		{
			xx=j*bmi->nWidth-mapX;
			yy=i*bmi->nHeight-mapY;
			BitBlt(hDC,xx,yy,bmi->nWidth,bmi->nHeight,
				memDC,0,0,SRCCOPY);
		}
	}
	int startX=mapX/nSquareWidth;//���㿪ʼ���Ƶ�λ�õ�����
	int startY=mapY/nSquareHeight;
	int endX=startX+rcClient->right/nSquareWidth+1;
	if(endX>nXCount)
		endX=nXCount;
	int endY=startY+rcClient->bottom/nSquareHeight+1;
	if(endY>nYCount)
		endY=nYCount;
	//��������
	for(i=startY;i<endY;++i)
	{
		for(j=startX;j<endX;++j)
		{
			if(bShowObstacle)
			{
				if(ppObjectArray[i][j].nObstacleKind!=0)//ppMapArray[i][j])
				{
					Rectangle(hDC,j*nSquareWidth,i*nSquareHeight,(j+1)*nSquareWidth,(i+1)*nSquareHeight);
				}
			}
			if(ppObjectArray[i][j].nObjectIndex!=0)
			{
				bmi=bmpObject.GetAt(ppObjectArray[i][j].nObjectIndex-1);
				SelectObject(memDC,bmi->hBitmap);
				yy=i*nSquareHeight-mapY;
				//���������ʱ�������������ƶ�
				if(bmi->nHeight > nSquareHeight)
					yy=yy-bmi->nHeight+nSquareHeight;
				xx=j*nSquareWidth-mapX;
				if(bmi->nWidth > nSquareWidth)//���������ʱ�������������ƶ�
					xx=xx-bmi->nWidth+nSquareWidth;
				TransparentBlt(hDC,xx,yy,bmi->nWidth,bmi->nHeight,
					memDC,0,0,bmi->nWidth,bmi->nHeight,GetPixel(memDC,0,0));
			}
		}
	}
	DeleteDC(memDC);
}
/***************************����Ϊ��ͼ�༭����***********************/