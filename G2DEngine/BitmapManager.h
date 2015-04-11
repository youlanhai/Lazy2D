//BitmapManager.h
#pragma once
#include <string>
#include <vector>

//位图信息
struct BitmapInfo
{
	HBITMAP hBitmap;//位图句柄
	int nWidth;		//位图宽度
	int nHeight;	//位图高度
	int nXFrame;	//位图x方向帧数
	int nYFrame;	//位图y方向帧数

	BitmapInfo()
	{
		hBitmap=NULL;
		nWidth=nHeight=0;
		nXFrame=nYFrame=0;
	}
};

typedef  BitmapInfo *LBitmapInfo;
typedef  BitmapInfo CBitmapInformation;//与旧版本兼容

/***************************************
位图管理器：管理位图的加载，分配和释放。
用法：构造位图配置文件，load从文件中读取信息，并加载位图。
***************************************/
class CBitmapManager
{
private:
	std::vector<BitmapInfo>	m_vecBitmaps;//位图信息数组
    
public://v2.0
	CBitmapManager();
	~CBitmapManager();

    bool load(HINSTANCE hInst,const char *szFileName);
    void clear(void);

    LBitmapInfo at(int nIndex);
    HBITMAP atBitmap(int nIndex);

    inline int size(void) const { return m_vecBitmaps.size(); }

    inline std::vector<BitmapInfo>& getBitmapArray(void)
    { return m_vecBitmaps; }

    //以下是v1.0版本
public:
	bool CreateBitmapFromFileList//从文件中读取位图文件名创建位图信息
	    (HINSTANCE hInstance,
        std::string strListName)
    { 
        return load(hInstance,strListName.c_str()); 
    }

    void DestroyHBitmapArray(){ clear(); }		//销毁位图结构数组

    LBitmapInfo GetAt(int nIndex){ return at(nIndex); }	//返回位图结构的引用

	int  GetCount(){return size();}
};