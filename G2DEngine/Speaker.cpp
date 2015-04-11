//CSpeaker.cpp
#include "Commen.h"
#include "Speaker.h"

CSpeaker::CSpeaker()
{
	size=0;
	currentPtr=0;	//����ǰָ��
}
CSpeaker::~CSpeaker()
{
	Clear();
}
void CSpeaker::Clear()
{
	bmpManager.DestroyHBitmapArray();
	wordArry.clear();
	size=0;
	currentPtr=0;
}
bool CSpeaker::ReadFromFile(HINSTANCE hInst,std::string bmpFile,std::string speakerFile)
{
	Clear();
	if(!bmpManager.CreateBitmapFromFileList(hInst,bmpFile))
		return false;
	std::ifstream fin(speakerFile.c_str());
	if(fin.bad())
		return false;
	fin>>size;
	wordArry.resize(size);
	for(int i=0;i<size;++i)
	{
		fin>>wordArry[i].id;
		fin.getline(wordArry[i].words,256,'\n');
		char* ptr=wordArry[i].words;
		ptr=strchr(ptr,'@');//��@�ַ��滻�ɻ��з�
		*ptr='\n';
		*(ptr+1)='\n';
	}
	fin.close();
	return true;
}
int CSpeaker::Show(HDC hDC,RECT* rc)
{
	if(currentPtr>=size)
		return 1;
	//����Ի�����
	RECT textRC;
	textRC.left=rc->left;
	textRC.top=rc->bottom-100;
	textRC.right=rc->right;
	textRC.bottom=rc->bottom;
	//��͸����ʾ�Ի�����
	BLENDFUNCTION bf;
	bf.AlphaFormat=0;
	bf.BlendFlags=0;
	bf.BlendOp=AC_SRC_OVER;
	bf.SourceConstantAlpha=180;//͸����0-255
	HDC memDC=CreateCompatibleDC(NULL);
	HBITMAP bmp=CreateCompatibleBitmap(hDC,textRC.right,100);
	SelectObject(memDC,bmp);
	FillRect(memDC,&textRC,Graphic::BlackBrush);
	AlphaBlend(hDC,textRC.left,textRC.top,textRC.right,100,
		memDC,0,0,textRC.right,100,bf);
	DeleteObject(bmp);
	//��ʾ�Ի�����
	LBitmapInfo bmi=bmpManager.GetAt(wordArry[currentPtr].id);
	SelectObject(memDC,bmi->hBitmap);
	int x=textRC.left;
	if(bmi->nYFrame==0)
		x=textRC.right-bmi->nWidth;
	TransparentBlt(hDC,x,textRC.bottom-bmi->nHeight,bmi->nWidth,bmi->nHeight,
		memDC,0,0,bmi->nWidth,bmi->nHeight,GetPixel(memDC,0,0));//RGB(255,255,255)
	//���ƶԻ��ı�
	int style=DT_WORDBREAK;
	if(bmi->nYFrame==1)
	{
		textRC.left+=bmi->nWidth+20;
		style|=DT_LEFT;
	}
	else
	{
		textRC.right-=(bmi->nWidth+20);
		style|=DT_RIGHT;
	}
	COLORREF oldTextCr=SetTextColor(hDC,RGB(255,255,255));
	DrawText(hDC,wordArry[currentPtr].words,
		-1,&textRC,style);//����룬�Զ�����
	SetTextColor(hDC,oldTextCr);
	DeleteDC(memDC);
	return 0;
}
