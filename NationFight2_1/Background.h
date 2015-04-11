#pragma once

class CBackground
{
public:
	CBackground(void);
	~CBackground(void);
	void init(int winWidth,int winHeight);
	void update();
	void render(HDC hDC);
public:
	bool	bFlow;			//���ڹ���
	bool	bShake;			//�������
	int		nShakeCounter;	//�������
	int		nBackBmpIndex;	//����ͼƬ����
	int		nX;				//�������ͻ����ϻ��Ƶ�����
	int		nY;
	int		nWindowWidth;
	int		nWindowHeight;
};
