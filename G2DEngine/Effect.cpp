//CEffect.cpp
#include "Commen.h"
#include "BitmapManager.h"
#include "Effect.h"

CBitmapManager CEffect::bmpManager=CBitmapManager();//��ЧͼƬ������

CEffect::CEffect()
{
	nID=0;
	nFrame=nXCount=nIndexCounter=0;
	hMemDC=CreateCompatibleDC(0);
}
CEffect::CEffect(int id,int x,int y)
{
	Initialize(id,x,y);
}
CEffect::~CEffect()
{
	if(hMemDC)
		DeleteObject(hMemDC);
}
void CEffect::Initialize(int id,int x,int y)
{
	nID=id;
	rect.x=x;
	rect.y=y;
	LBitmapInfo bmi=bmpManager.GetAt(id);
	nFrame=bmi->nXFrame;	//��֡��
	nXCount=bmi->nYFrame;	//һ�е�֡��
	rect.w=bmi->nWidth/nXCount;
	int t=nFrame%nXCount;
	if(t>0)
		t=1;
	rect.h=bmi->nHeight/(nFrame/nXCount+t);//nFrame/nXCount+t�����������
	dwOldTime=0;
	nIndexCounter=0;
}
bool CEffect::InitializeCommen(HINSTANCE hInstance,std::string strBitmapListName)//������Чλͼ
{
	if(!bmpManager.CreateBitmapFromFileList(hInstance,strBitmapListName))
		return false;
	return true;
}

bool CEffect::Show(HDC hBackDC)//��ʾ��Ч
{
	if(nXCount==0 || nFrame==0)
		return true;
	DWORD dwNewTime=timeGetTime();
	if(dwNewTime-dwOldTime>80)
	{
		dwOldTime=dwNewTime;
		++nIndexCounter;
	}
	int nCurrentIndex=nIndexCounter%nFrame;
	LBitmapInfo bmi=bmpManager.GetAt(nID);
	SelectObject(hMemDC,bmi->hBitmap);
	TransparentBlt(hBackDC,rect.x - rect.w/2,rect.y - rect.h/2,
		rect.w,rect.h,hMemDC,
		nCurrentIndex%nXCount * rect.w,nCurrentIndex/nXCount * rect.h,
		rect.w,rect.h,
		GetPixel(hMemDC,0,0));

	if(nIndexCounter>=nFrame)
		return true;
	else
		return false;
}
