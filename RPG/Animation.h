//Animation.h
#pragma once
class CBitmapManager;

//动画类
class CAnimation
{
public:
	CAnimation();
	~CAnimation();

	bool	Initialize(HINSTANCE hInstance,std::string strBitmapListName);//初始化动画资源
	int		DrawHeader(HDC hBackDC,RECT* rcClient);//开头动画,播放结束返回1
	int		DrawAnimation(HDC hBackDC,RECT* rcClient);//绘制特效
	void	ClearAnimation();				//清除动画资源

    bool    IsFinished() const { return m_index >= 256 * 2; }
private:
	CBitmapManager  bmpAnimManager;//动画位图管理器
    int             m_index;
};