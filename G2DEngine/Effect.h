//CEffect.h
#pragma once
class CBitmapManager;

/*********************************
特效类：
每一个特效类的实例对象可以完成一个特效
位图文件列表的构造：由于特效类图片不规则，
列表的第一项为位图路径，第二项为总帧数，第三项为一行的帧数
*********************************/
class CEffect
{
public:
	CEffect();
	CEffect(int id,int x,int y);
	~CEffect();
	void Initialize(int id,int x,int y);
	bool Show(HDC hBackDC);//显示特效
	static bool InitializeCommen(HINSTANCE hInstance,std::string strBitmapListName);//加载特效位图
private:
	int nID;			//特效的id
	CGameRect rect;			//特效要显示的位置区域
	DWORD dwOldTime;	//上一次变换动画时的时间
	int nFrame;			//总帧数
	int nXCount;		//一行的帧数
	int nIndexCounter;	//显示的帧数
	HDC	hMemDC;			//掩码DC
	static CBitmapManager bmpManager;//特效图片管理器
};