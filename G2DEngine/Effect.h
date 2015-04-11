//CEffect.h
#pragma once
class CBitmapManager;

/*********************************
��Ч�ࣺ
ÿһ����Ч���ʵ������������һ����Ч
λͼ�ļ��б�Ĺ��죺������Ч��ͼƬ������
�б�ĵ�һ��Ϊλͼ·�����ڶ���Ϊ��֡����������Ϊһ�е�֡��
*********************************/
class CEffect
{
public:
	CEffect();
	CEffect(int id,int x,int y);
	~CEffect();
	void Initialize(int id,int x,int y);
	bool Show(HDC hBackDC);//��ʾ��Ч
	static bool InitializeCommen(HINSTANCE hInstance,std::string strBitmapListName);//������Чλͼ
private:
	int nID;			//��Ч��id
	CGameRect rect;			//��ЧҪ��ʾ��λ������
	DWORD dwOldTime;	//��һ�α任����ʱ��ʱ��
	int nFrame;			//��֡��
	int nXCount;		//һ�е�֡��
	int nIndexCounter;	//��ʾ��֡��
	HDC	hMemDC;			//����DC
	static CBitmapManager bmpManager;//��ЧͼƬ������
};