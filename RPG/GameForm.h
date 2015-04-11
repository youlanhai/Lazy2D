//GameForm.h
#pragma once
#include "BitmapManager.h"
class CGameButton;

typedef enum
{
	FORM_CAPTION=0,	//�б���͹رհ�ť�Ĵ���
	FORM_POPUP,		//û�б������Ĵ���
	FORM_NONE		//����ʾ����
}FormStyle;//������

typedef enum
{
	FORM_HIDE=0,
	FORM_SHOW,
	FORM_DESTROY
}FormState;//����״̬

class CGameForm
{
public:
	CGameForm();
	~CGameForm();
	void InitForm												//��ʼ������
	(
		std::string caption,FormStyle style,
		int x=0,int y=0,
		int width=0,int height=0,
		COLORREF foreColor=0,COLORREF backColor=0
	);
	void SetCaption(std::string caption);						//���ñ���
	void SetColor(COLORREF foreColor=0,COLORREF backColor=0);	//������ɫ
	void SetRect(int x,int y,int w=0,int h=0);					//������ʾ����
	void SetRect(CGameRect* rc);								//������ʾ����
	void SetState(FormState state){fsState=state;}				//���ô���״̬
	void SetStyle(FormStyle style){fsStyle=style;}				//���ô�����
	void SetBitmap												//������ʾλͼ
	(
		CBitmapManager* bmpManager,
		int nStartPosition=0
	);
	void Show(HDC hDC);											//��ʾ
	FormState GetState(){return fsState;}						//���״̬
	CGameRect* GetRect(){return &rect;}							//�����ʾ����
	void ProcessMessage(HWND hWnd,void* pParam);				//������Ϣ
	void ClearButton();											//��հ�ť
	void AddButton(CGameButton* button);						//��Ӱ�ť
	void ClientToForm(POINT* pt);
	void ClientToForm(RECT* rc);
	void ClientToForm(CGameRect* rc);
public:
	std::vector<CGameButton*> vecButtons;//��ť��
private:
	std::string	strCaption;				//��ť����
	COLORREF	crBackColor;			//ǰ��ɫ
	COLORREF	crForeColor;			//����ɫ
	FormState	fsState;				//����״̬
	FormStyle	fsStyle;				//������
	CGameRect	rect;					//��ť����
	CGameRect	rcClose;				//�رհ�ť
	CBitmapInformation	bmpBackGround;	//����ͼƬ
};