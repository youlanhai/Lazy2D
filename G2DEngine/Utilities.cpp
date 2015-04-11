//Utilities.cpp 
#include <windows.h>

#include "Utilities.h"
#include "Log.h"

#define MAX_SIZE 1024
static WCHAR szWideProgID[MAX_SIZE];
static char szCharProgID[MAX_SIZE];

char* WStringToChar(char *szChar,const std::wstring &str)
{
	int len=WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,
		str.c_str(),-1,
		szCharProgID,MAX_SIZE,NULL,NULL); 
	szCharProgID[len]=0;
	if(szChar != NULL)
	{
		strcpy_s(szChar,MAX_SIZE,szCharProgID);
	}
	return szCharProgID;
}

char* WCharToChar(char *szChar,const WCHAR *szWChar)
{
	int len=WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,
		szWChar,-1,
		szCharProgID,MAX_SIZE,NULL,NULL); 
	szCharProgID[len]=0;
	if(szChar != NULL)
	{
		strcpy_s(szChar,MAX_SIZE,szCharProgID);
	}
	return szCharProgID;
}

void CharToWString(std::wstring & str,const char *szChar)
{
	long lLen = MultiByteToWideChar(CP_ACP,0,
		szChar,strlen(szChar),
		szWideProgID,sizeof(szWideProgID)); 
	szWideProgID[lLen] = '\0';
	str=szWideProgID;
}

WCHAR* CharToWChar(WCHAR *szWChar,const char *szChar)
{
	long lLen = MultiByteToWideChar(CP_ACP,0,
		szChar,strlen(szChar),
		szWideProgID,sizeof(szWideProgID)); 
	szWideProgID[lLen] = '\0';
	if(szWChar != NULL)
	{
		wcscpy_s(szWChar,MAX_SIZE,szWideProgID);
	}
	return szWideProgID;
}

COLORREF getBlendColor(COLORREF cr,double x)
{
	UINT r=UINT(GetRValue(cr)*x);
	UINT g=UINT(GetGValue(cr)*x);
	UINT b=UINT(GetBValue(cr)*x);
	return RGB(r,g,b);
}

HFONT createFontEx(int height,char *szName)
{
	HFONT font=CreateFont(
		height,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		szName);                 // lpszFacename
	return font;
}

void changeWorkDirToModulePath()
{
    char path[MAX_PATH];
    if (!::GetModuleFileNameA(NULL, path, MAX_PATH))
    {
        return;
    }

    char *p = strrchr(path, '\\');
    if (NULL == p)
    {
        if (NULL == (p = strrchr(path, '/')))
        {
            return;
        }
    }

    *p = '\0';
    ::SetCurrentDirectoryA(path);
}
