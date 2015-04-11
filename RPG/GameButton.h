//GameButton.h
#pragma once
#include "BitmapManager.h"

typedef enum
{
	BUTTON_NORMAL=0,	//һ��״̬
	BUTTON_ACTIVE,		//�״̬
	BUTTON_DOWN			//����״̬
}ButtonState;

typedef enum
{
	BUTTON_PUSH=0,	//��ѹʽ��ť������Ϊ����3̬λͼ
	BUTTON_DRAW,	//��ѹʽ��ť������Ϊ�ֶ���ӵ�3̬λͼ
	BUTTON_STATIC,	//����������������Ϣ
	BUTTON_TEXT,	//��ʾ�ı����ޱ���
}ButtonStyle;

typedef void (*GameButtonCall)(void* pParam);
/***********************************************
��ť�ࣺ
ʹ�÷�����
1.��ʼ���������InitializeCommen
2.��ʼ����ťInitButton
3.������Ϣ
a.������ָ��pButtonCall��ֵ������ָ��ԭ�ͣ� void GameButtonCall(void* pParam);
���캯��ָ�뷽����
1.����ȫ�ֺ�������ȫ�ֺ�����ԭ�Ͷ���Ϊvoid GameButtonCall(void* pParam);
2.�������Ա��������Ҫ������Ϣ����������Ӧ��Ա������
�ٶ���һ��ȫ�ֺ�����̬��Ա����������Ϊvoid GameButtonCall(void* pParam);
�ڸú����У���pParamǿ��ת��ΪҪ����ĳ�Ա���������ָ�룬ʹ��ת�����ָ�����
��Ա������
b.���������ϢProcessMouseMessage��
ע�������pParam����ʱ��һ��Ϊӵ�а�ť����Ķ����thisָ�롣
***********************************************/
class CGameButton
{
public:
	CGameButton();
	~CGameButton();
	CGameButton(std::string caption,ButtonStyle style,GameButtonCall pFuction=NULL);
	void InitButton											//��ʼ����ť
	(
		std::string caption,ButtonStyle style,
		COLORREF foreColor=0,COLORREF backColor=0,
		GameButtonCall pFuction=NULL//�ص�����ָ��
	);
	void SetCaption(std::string caption){strCaption=caption;}//���ñ���
	void SetColor(COLORREF foreColor,COLORREF backColor);	//������ɫ
	void SetRect(int x,int y,int w=0,int h=0);				//������ʾ����
	void SetRect(CGameRect* rc);							//������ʾ����
	void SetButtonCall(GameButtonCall pFuction);			//���ûص�����
	void SetStyle(ButtonStyle style){bsStyle=style;}		//���ð�ť���
	void SetBitmap											//���ð�ť��ʾλͼ
	(
		CBitmapManager* bmpManager,
		int nStartPosition=0
	);
	void Show(HDC hDC);										//��ʾ��ť
	ButtonStyle GetStyle(){return bsStyle;}					//��ð�ť���
	ButtonState GetState(){return bsState;}					//��ð�ť״̬
	CGameRect*  GetRect(){return &rect;}					//�����ʾ����
	ButtonState ProcessMouseMessage							//���������Ϣ
	(
		POINT* ptMouse,	//�������
		bool bDown,		//����Ƿ���
		void* pParam	//���Ӳ��������ݸ��ص������Ĳ���
	);
	static bool InitializeCommen							//��ʼ��ͨ��λͼ
	(
		HINSTANCE hInstance,
		std::string strBmpListName
	);
private:
	ButtonStyle	bsStyle;					//��ť���
	ButtonState	bsState;					//��ť״̬
	std::string strCaption;					//��ť����
	COLORREF	crForeColor;				//������ɫ
	COLORREF	crBackColor;				//��ťΪstaticʱ���������λͼ����Ӧ���ô���
	CGameRect	rect;						//��ť����
	CBitmapInformation		bmpButton[3];	//��ť����̬λͼ
	GameButtonCall			pButtonCall;	//��ťҪ�������Ϣ����ָ��
	static CBitmapManager	bmpCommenButton;//������ť��̬λͼ
};

inline void CGameButton::SetColor(COLORREF foreColor,COLORREF backColor)
{
	crForeColor=foreColor;
	crBackColor=backColor;
	if(crForeColor==crBackColor)
	{
		crForeColor=RGB(255,0,0);
		crBackColor=RGB(20,20,20);
	}
}
inline void CGameButton::SetButtonCall(GameButtonCall pFuction)
{
	pButtonCall=pFuction;
}