//DxMusic.cpp
#define DIRECTSOUND_VERSION 0x0800
#include <windows.h>
#include <direct.h>
#include <mmsystem.h>

#include "DxMusic.h"

CDxMusic::CDxMusic(void)
{
    m_pMid = NULL;
    m_pDmperf = NULL;
    m_pDmloader = NULL;
    m_fileAmount = 0;
}

CDxMusic::~CDxMusic(void)
{
    destroy();
}

bool CDxMusic::init(HWND hWnd,int max_file)
{
    //初始化COM
    if(FAILED(CoInitialize(NULL)))
    {
        return false;
    }
    //创建一演奏对象
    if(FAILED(CoCreateInstance(CLSID_DirectMusicPerformance,
        NULL,
        CLSCTX_INPROC,
        IID_IDirectMusicPerformance,
        (void**)&m_pDmperf)))
    {
        return false;
    }
    //初始化演奏对象
    if(FAILED(m_pDmperf->Init(NULL,NULL,hWnd)))
    {
        return false;
    }
    //给演奏对象增加端口
    if(FAILED(m_pDmperf->AddPort(NULL)))//NULL为使用默认的软件合成器
    {
        return false;
    }
    //创建一加载程序
    if(FAILED(CoCreateInstance(CLSID_DirectMusicLoader,
        NULL,
        CLSCTX_INPROC,
        IID_IDirectMusicLoader,
        (void**)&m_pDmloader)))
    {
        return false;
    }
    m_fileAmount=max_file;
    m_pMid=new DMUSIC_MID[sizeof(DMUSIC_MID)*m_fileAmount];
    return true;
}
bool CDxMusic::load(int index,char *filename)
{
    if (index<0 || index>m_fileAmount)
    {
        return false;
    }
    if (m_pMid[index].pSegment != NULL)
    {
        deleteOne(index);
    }

    HRESULT hr;
    IDirectMusicSegment *pSegment=NULL;
    char szDir[256];
    WCHAR wszDir[256];
    //获得当前目录
    if(_getcwd(szDir,256) == NULL)
    {
        return false;
    }
    //ACII转换为宽字
    MultiByteToWideChar( CP_ACP,MB_PRECOMPOSED, szDir,-1,wszDir,256);
    //告诉dmloader要去哪个目录查找文件.
    hr=m_pDmloader->SetSearchDirectory(GUID_DirectMusicAllTypes,wszDir,FALSE);
    if(FAILED(hr))
    {
        return false;
    }
    WCHAR wszfilename[256];
    MultiByteToWideChar( CP_ACP,MB_PRECOMPOSED, filename,-1,wszfilename,256);

    DMUS_OBJECTDESC ObjDesc;
    memset(&ObjDesc,0,sizeof(ObjDesc));
    ObjDesc.dwSize=sizeof(ObjDesc);
    ObjDesc.guidClass=CLSID_DirectMusicSegment;
    wcscpy_s(ObjDesc.wszFileName,256,wszfilename);
    ObjDesc.dwValidData=DMUS_OBJ_CLASS|DMUS_OBJ_FILENAME;

    if(FAILED( m_pDmloader->GetObject(&ObjDesc,IID_IDirectMusicSegment,(void**)&pSegment)))
    {
        return false;
    }
    if(FAILED( pSegment->SetParam(GUID_StandardMIDIFile,-1,0,0,(void*)m_pDmperf)))
    {
        return false;
    }
    if(FAILED( pSegment->SetParam(GUID_Download,-1,0,0,(void*)m_pDmperf)))
    {
        return false;
    }

    m_pMid[index].pSegment = pSegment;

    return true;
}
bool CDxMusic::play(int index,bool loop,bool bfromhead)
{
    if(m_pDmperf == NULL)
    {
        return false;
    }
    if(FAILED(m_pDmperf->PlaySegment(m_pMid[index].pSegment,0,0,&m_pMid[index].pSegState)))
    {
        return false;
    }
    if (loop)
    {
        m_pMid[index].pSegment->SetRepeats(DMUS_SEG_REPEAT_INFINITE);
    }
    return true;
}
bool CDxMusic::isPlaying(int index)
{
    if(m_pDmperf == NULL)
    {
        return false;
    }
    if( m_pDmperf->IsPlaying(m_pMid[index].pSegment,NULL ) == S_OK)
    {
        return true;
    }
    else return false;
}
//    bool setFrequncy(int index,long fre);
//    bool setPan(int index,long lpan);
float CDxMusic::setVolum(int index,float volume)
{

    return 0;
}
bool CDxMusic::stop(int index)
{
    if(m_pDmperf == NULL)
    {
        return false;
    }
    if (FAILED( m_pDmperf->Stop(m_pMid[index].pSegment,NULL,0,0) ))
    {
        return false;
    }
    else return true;
}
void CDxMusic::stopAll()
{
    if(m_pDmperf == NULL)
    {
        return ;//false;
    }
    if (FAILED( m_pDmperf->Stop(NULL,NULL,0,0) ))
    {
        //return false;
    }
    //else return true;
}
void CDxMusic::deleteOne(int index)
{
    if(m_pDmperf == NULL)
    {
        return ;//false;
    }
    if(m_pMid[index].pSegment != NULL)
    {
        m_pMid[index].pSegment->SetParam(GUID_Unload,-1,0,0,(void*)m_pDmperf);
        m_pMid[index].pSegment->Release();
        m_pMid[index].pSegment=NULL;
        m_pMid[index].pSegState=NULL;
    }
}
void CDxMusic::deleteAll()
{
    if(m_pDmperf == NULL)
    {
        return ;//false;
    }
    stopAll();
    for(int i=0; i<m_fileAmount; i++)
    {
        if(m_pMid[i].pSegment != NULL)
        {
            deleteOne(i);
        }
    }
}
void CDxMusic::destroy()
{
	if(m_pDmperf == NULL)
    {
        return ;//false;
    }
    deleteAll();
    delete m_pMid;
    m_pMid = NULL;
    
	m_pDmperf->CloseDown();
	m_pDmperf->Release();
	m_pDmperf=NULL;
    
    m_pDmloader->Release();
	m_pDmloader=NULL;
	
    CoUninitialize();
}
