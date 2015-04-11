//Comment.h
//公共文件
#ifndef H_G2DENGINE_COMMON_H
#define H_G2DENGINE_COMMON_H

#include <windows.h>
#include <mmsystem.h>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
#include <cstring>
#include <string>
#include <list>
#include <vector>

#define KEYDOWN(vk_code)	(GetAsyncKeyState(vk_code)&0x8000)
#define KEYUP(vk_code)		(!GetAsyncKeyState(vk_code)&0x8000)
#define SafeDelete(pObj)	{if (pObj!=NULL){ delete pObj; pObj=NULL; }}
#define SafeDeleteArray(pObj) {if (pObj!=NULL){ delete [] pObj; pObj=NULL; }}

#include "Utilities.h"
#include "Log.h"

namespace Graphic
{
	static HBRUSH	BlackBrush=(HBRUSH)GetStockObject(BLACK_BRUSH);
	static HBRUSH	WhiteBrush=(HBRUSH)GetStockObject(WHITE_BRUSH);
	static HPEN		RedPen=CreatePen(PS_SOLID,1,RGB(255,0,0));
};

enum UI_MESSAGE	//ui消息
{
	WM_UI_REORDER=WM_USER+1	//重排序，使得某一Dialog位于最前端
};

#endif //H_G2DENGINE_COMMON_H
