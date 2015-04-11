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
	bool	bFlow;			//窗口滚动
	bool	bShake;			//窗口震颤
	int		nShakeCounter;	//震颤次数
	int		nBackBmpIndex;	//背景图片索引
	int		nX;				//背景往客户区上绘制的坐标
	int		nY;
	int		nWindowWidth;
	int		nWindowHeight;
};
