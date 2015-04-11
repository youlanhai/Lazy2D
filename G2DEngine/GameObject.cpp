//GameObject.cpp
#include "Commen.h"
#include "GameObject.h"
#include "BitmapManager.h"

int	CGameObject::nIDCounter=1;
CBitmapManager CGameObject::bmpManager=CBitmapManager();//位图管理器

void CGameObject::InitializeCommen(HINSTANCE hInstance,std::string strBmpListFileName)
{
	bmpManager.CreateBitmapFromFileList(hInstance,strBmpListFileName);
}

CGameObject::CGameObject()
{
	memset(this,0,sizeof(CGameObject));
	nID=++nIDCounter;
}
CGameObject::~CGameObject()
{

}
void CGameObject::Initialize
(
	int kind,std::string name,
	int state,int speed,
	int life,int magic,
	int attack,int defence,
	int level
)
{
	nShowID=kind;
	strName=name;
	nState=state;
	nSpeed=speed;
	nLife=nMaxLife=life;
	nMagic=nMaxMagic=magic;
	nAttack=attack;
	nDefence=defence;
	nLevel=level;
	LBitmapInfo bmi=bmpManager.GetAt(kind);
	if(bmi)
	{
		nWidth=bmi->nWidth/bmi->nXFrame;
		nHeight=bmi->nHeight/bmi->nYFrame;
	}
	actionRect.w=actionRect.h=8;
}
void CGameObject::Show(HDC hBackDC)//显示
{
	//绘制人物
	HDC memDC=CreateCompatibleDC(hBackDC);
	SelectObject(memDC,bmpManager.GetAt(nShowID)->hBitmap);
	TransparentBlt(
		hBackDC,ptPosition.x-nWidth/2,ptPosition.y-nHeight,
		nWidth,nHeight,
		memDC,
		ptPositionInPixture.x*nWidth,ptPositionInPixture.y*nHeight,
		nWidth,nHeight,
		GetPixel(memDC,0,0));
	DeleteDC(memDC);
}