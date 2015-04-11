//Map.cpp
#include "Commen.h"
#include "GameObject.h"
#include "Map.h"

CGameMap::CGameMap()
{
	nMapWidth=0;	//地图宽度
	nMapHeight=0;	//地图高度
	nSquareWidth=0;	//地图方格宽度
	nSquareHeight=0;//地图方格高度
	nXCount=0;		//地图方格列数
	nYCount=0;		//地图方格行数
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
	//读取障碍数组
	std::ifstream fin(strMapName.c_str());
	if(fin.fail())
		return false;
	Destroy();
	//读取方格的宽、高、列数、行数
	fin>>nSquareWidth>>nSquareHeight>>nXCount>>nYCount;
	nMapWidth=nSquareWidth*nXCount;
	nMapHeight=nSquareHeight*nYCount;
	//分配二维空间
	int i,j;
	ppObjectArray=new CMapNode*[nYCount];
	for(i=0;i<nYCount;++i)
	{
		ppObjectArray[i]=new CMapNode[nXCount];
		memset(ppObjectArray[i],0,sizeof(CMapNode)*nXCount);
	}
	//读取地面类型
	fin>>nGroundType;
	//读取物体数组
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
	if(ppObjectArray)//释放物体描述数组
	{
		for(int i=0;i<nYCount;++i)
			delete [] ppObjectArray[i];
		delete [] ppObjectArray;
		ppObjectArray=NULL;
	}
	nMapWidth=0;	//地图宽度
	nMapHeight=0;	//地图高度
	nSquareWidth=0;	//地图方格宽度
	nSquareHeight=0;//地图方格高度
	nXCount=0;		//地图方格列数
	nYCount=0;		//地图方格行数
	nGroundType=0;	//地面类型
	bShowObstacle=false;
	bEnable=false;
}
//在使用此函数前必须调用RemoveGameObject将自己先前的位置释放
void CGameMap::AddGameObject(CGameObject* pSprit)
{
	int i=pSprit->ptPosition.y/nSquareHeight;//计算行
	int j=pSprit->ptPosition.x/nSquareWidth;//计算列
	if(i>=0 && i<nYCount && j>=0 && j<nXCount)
	{
		ppObjectArray[i][j].pSprit=pSprit;
		ppObjectArray[i][j].nObstacleKind=1;//不能通行
	}
}
void CGameMap::RemoveGameObject(CGameObject* pSprit)
{
	if(this)
	{
		int i=pSprit->ptPosition.y/nSquareHeight;//计算行
		int j=pSprit->ptPosition.x/nSquareWidth;//计算列
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
	int xIndex=ptPosition->x/nSquareWidth;//计算列
	int yIndex=ptPosition->y/nSquareHeight;//计算行
	return IsSquareCanPass(xIndex,yIndex);
}

void CGameMap::Show2(HDC hDC,CGameObject* player,RECT* rcClient)
{
	//计算区域的宽度和高度
	int screenWidth=rcClient->right - rcClient->left;
	int screenHeight=rcClient->bottom - rcClient->top;

	POINT screenPos;//屏幕的左上角在相对于地图的坐标
	screenPos.x=player->ptPosition.x - screenWidth/2;//将玩家始终显示在屏幕中央
	screenPos.y=player->ptPosition.y - screenHeight/2;
	
	POINT startPos;//地图要显示到屏幕上的区域左上角坐标。
	startPos.x=screenPos.x;
	startPos.y=screenPos.y;
	POINT endPos;//要显示到屏幕上的区域的右下角坐标.
	endPos.x=startPos.x+screenWidth+100;//+100为偏移量，放置物体被提前裁剪
	endPos.y=startPos.y+screenHeight+100;

	if(startPos.x<0)//超出了左边界
	{
		startPos.x=0;
	}
	if(startPos.y<0)//超出上边界
	{
		startPos.y=0;
	}
	if(endPos.x>nMapWidth)//超出右边界
	{
		endPos.x=nMapWidth;
	}
	if(endPos.y>nMapHeight)//超出下边界
	{
		endPos.y=nMapHeight;
	}

	LBitmapInfo bmi=NULL;
	HDC memDC=CreateCompatibleDC(0);
	//绘制地面
	bmi=bmpGround.GetAt(nGroundType);
	SelectObject(memDC,bmi->hBitmap);
	//计算开始绘制的位置的索引
	int startI=startPos.y/bmi->nHeight;
	int startJ=startPos.x/bmi->nWidth;
	//计算结束绘制位置的索引
	int endI=endPos.y/bmi->nHeight+1;
	int endJ=endPos.x/bmi->nWidth+1;
	int i,j,x,y,xx,yy;
	for(i=startI;i<endI;++i)
	{
		for(j=startJ;j<endJ;++j)
		{
			//要显示到屏幕的位置
			x=j*bmi->nWidth-screenPos.x;
			y=i*bmi->nHeight-screenPos.y;
			//要显示的宽度和高度
			xx=bmi->nWidth;
			yy=bmi->nHeight;
			//将多余的部分裁剪掉
			if(x+xx > endPos.x-screenPos.x)
				xx=endPos.x-screenPos.x-x;
			if(y+yy > endPos.y-screenPos.y)
				yy=endPos.y-screenPos.y-y;
			BitBlt(hDC,x,y,xx,yy,memDC,0,0,SRCCOPY);
		}
	}
	//计算开始绘制的位置的索引
	startI=startPos.y/nSquareWidth;
	startJ=startPos.x/nSquareHeight;
	//计算结束绘制位置的索引
	endI=endPos.y/nSquareHeight;
	endJ=endPos.x/nSquareWidth;
	//绘制物体
	for(i=startI;i<endI;++i)
	{
		for(j=startJ;j<endJ;++j)
		{
			if(bShowObstacle)
			{
				if(ppObjectArray[i][j].nObstacleKind!=0)//绘制障碍
				{
					x=j*nSquareWidth-screenPos.x;
					y=i*nSquareHeight-screenPos.y;
					Rectangle(hDC,x,y,x+nSquareWidth,y+nSquareHeight);
				}
			}	//绘制精灵
			CGameObject* pSprit=ppObjectArray[i][j].pSprit;
			if(pSprit!=NULL && pSprit->nState!=0)
			{
				bmi=CGameObject::bmpManager.GetAt(pSprit->nShowID);
				if(bmi)
				{
					SelectObject(memDC,bmi->hBitmap);
					x=pSprit->ptPosition.x - screenPos.x;
					y=pSprit->ptPosition.y - screenPos.y;
					//绘制
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
				//当物体过宽时，将物体向左移动
				if(bmi->nWidth > nSquareWidth)
					x=x - bmi->nWidth + nSquareWidth;
				//当物体过高时，将物体向上移动
				if(bmi->nHeight > nSquareHeight)
					y=y - bmi->nHeight + nSquareHeight;
				TransparentBlt(hDC,x,y,bmi->nWidth,bmi->nHeight,
					memDC,0,0,bmi->nWidth,bmi->nHeight,GetPixel(memDC,0,0));
			}
		}
	}
	DeleteDC(memDC);
}

/***************************以下为地图编辑器用***********************/
void CGameMap::CreateMap												//创建地图
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
	nMapWidth=nXCount*nSquareWidth;		//重新计算地图宽度和高度，取整
	nMapHeight=nYCount*nSquareHeight;
	//分配二维空间
	int i;
	ppObjectArray=new CMapNode*[nYCount];
	for(i=0;i<nYCount;++i)
	{
		ppObjectArray[i]=new CMapNode[nXCount];
		memset(ppObjectArray[i],0,sizeof(CMapNode)*nXCount);
	}
	bEnable=true;
}

bool CGameMap::WriteMapToFile(std::string strMapName)//写地图
{
	std::ofstream fout(strMapName.c_str());
	if(fout.bad())return false;
	//写入方格的宽、高、列数、行数
	fout<<nSquareWidth<<" "<<nSquareHeight<<" "<<nXCount<<" "<<nYCount<<std::endl;
	//写入地面类型
	fout<<nGroundType<<std::endl;
	int i,j;
	//写入物体数组
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
void CGameMap::AddObstacle(int xIndex,int yIndex)					//添加障碍
{
	if(yIndex<nYCount && yIndex>=0 && xIndex<nXCount && xIndex>=0)
		ppObjectArray[yIndex][xIndex].nObstacleKind=!ppObjectArray[yIndex][xIndex].nObstacleKind;
}
void CGameMap::AddObject(int xIndex,int yIndex,int objectIndex)	//添加地面物体
{
	if(yIndex<nYCount && yIndex>=0 && xIndex<nXCount && xIndex>=0)
		ppObjectArray[yIndex][xIndex].nObjectIndex=objectIndex;
}
void CGameMap::SetGroundType(int nIndex)							//设置地面类型
{
	nGroundType=nIndex;
}
//mapX,mapY为屏幕在地图上的坐标
void CGameMap::Show(HDC hDC,int mapX,int mapY,RECT* rcClient)
{
	if (!bEnable)
	{
		return ;
	}
	LBitmapInfo bmi=NULL;
	HDC memDC=CreateCompatibleDC(0);
	//绘制地面
	bmi=bmpGround.GetAt(nGroundType);
	SelectObject(memDC,bmi->hBitmap);
	BitBlt(hDC,0,0,bmi->nWidth,bmi->nHeight,
		memDC,0,0,SRCCOPY);
	int x=nMapWidth/bmi->nWidth;//计算所需地面图片的个数
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
	int startX=mapX/nSquareWidth;//计算开始绘制的位置的索引
	int startY=mapY/nSquareHeight;
	int endX=startX+rcClient->right/nSquareWidth+1;
	if(endX>nXCount)
		endX=nXCount;
	int endY=startY+rcClient->bottom/nSquareHeight+1;
	if(endY>nYCount)
		endY=nYCount;
	//绘制物体
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
				//当物体过高时，将物体向上移动
				if(bmi->nHeight > nSquareHeight)
					yy=yy-bmi->nHeight+nSquareHeight;
				xx=j*nSquareWidth-mapX;
				if(bmi->nWidth > nSquareWidth)//当物体过宽时，将物体向左移动
					xx=xx-bmi->nWidth+nSquareWidth;
				TransparentBlt(hDC,xx,yy,bmi->nWidth,bmi->nHeight,
					memDC,0,0,bmi->nWidth,bmi->nHeight,GetPixel(memDC,0,0));
			}
		}
	}
	DeleteDC(memDC);
}
/***************************以上为地图编辑器用***********************/