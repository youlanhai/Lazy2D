// MySound.cpp : Defines the entry point for the DLL application.
//
#include "MySound.h"

#include <direct.h>
#include <mmsystem.h>

#include "dsound.h"
#include "dmksctrl.h"
#include "dmusicc.h"
#include "dmusici.h"
#include "dmusicf.h"
//#pragma comment(lib,"dxguid.lib")
//#pragma comment(lib,"dsound.lib")
//#pragma comment(lib,"winmm.lib")
//////////////////////////////////////
typedef struct{
    IDirectMusicSegment *pSegment;
    IDirectMusicSegmentState *pSegState;
    int index;
}DMUSIC_MID;

const int max_copy_count = 100;

#ifdef SOUND_ENGINE_DLL
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}
#endif

/////////////////dmusic/////////////////
MYSOUND_API int max_midi_file=10;
MYSOUND_API int max_wave_file=50;

MYSOUND_API DMUSIC_MID *g_mid=NULL;
MYSOUND_API IDirectMusicPerformance *dmperf=NULL;
MYSOUND_API IDirectMusicLoader *dmloader=NULL;
MYSOUND_API LPDIRECTSOUND lpds=NULL;
MYSOUND_API LPDIRECTSOUNDBUFFER *g_dsbuffer=NULL;
MYSOUND_API int CopyBufferCount[100];
//////////////////////////////////////
MYSOUND_API int InitMusic(HWND hWnd,int max_file)
{
	//初始化COM
	if(FAILED(CoInitialize(NULL)))
		return 0;
	//创建一演奏对象
	if(FAILED(CoCreateInstance(CLSID_DirectMusicPerformance,
								NULL,
								CLSCTX_INPROC,
								IID_IDirectMusicPerformance,
								(void**)&dmperf)))
		return 0;
	//初始化演奏对象
	if(FAILED(dmperf->Init(NULL,NULL,hWnd)))
		return 0;
	//给演奏对象增加端口
	if(FAILED(dmperf->AddPort(NULL)))//NULL为使用默认的软件合成器
		return 0;
	//创建一加载程序
	if(FAILED(CoCreateInstance(CLSID_DirectMusicLoader,
								NULL,
								CLSCTX_INPROC,
								IID_IDirectMusicLoader,
								(void**)&dmloader)))
		return 0;
	max_midi_file=max_file;
	g_mid=(DMUSIC_MID*)malloc(sizeof(DMUSIC_MID)*max_midi_file);
	for(int i=0;i<max_midi_file;i++)
	{
		g_mid[i].index=-1;
		g_mid[i].pSegment=NULL;
		g_mid[i].pSegState=NULL;
	}
	return 1;
}
MYSOUND_API int LoadMid(int index,char *filename)
{
	DMUS_OBJECTDESC ObjDesc;
	HRESULT hr;
	IDirectMusicSegment *pSegment;
	char szDir[100];
	WCHAR wszDir[100];
	//获得当前目录
	if(_getcwd(szDir,100)==NULL)
		return 0;
	//ACII转换为宽字
	MultiByteToWideChar( CP_ACP,MB_PRECOMPOSED, szDir,-1,wszDir,100);
	//告诉dmloader要去哪个目录查找文件.
	hr=dmloader->SetSearchDirectory(GUID_DirectMusicAllTypes,wszDir,FALSE);
	if(FAILED(hr))
		return 0;
	WCHAR wszfilename[100];
	MultiByteToWideChar( CP_ACP,MB_PRECOMPOSED, filename,-1,wszfilename,100);

	memset(&ObjDesc,0,sizeof(ObjDesc));
	ObjDesc.dwSize=sizeof(ObjDesc);
	ObjDesc.guidClass=CLSID_DirectMusicSegment;
	wcscpy(ObjDesc.wszFileName,wszfilename);
	ObjDesc.dwValidData=DMUS_OBJ_CLASS|DMUS_OBJ_FILENAME;

	hr=dmloader->GetObject(&ObjDesc,IID_IDirectMusicSegment,(void**)&pSegment);
	if(FAILED(hr))
		return 0;
	hr=pSegment->SetParam(GUID_StandardMIDIFile,-1,0,0,(void*)dmperf);
	if(FAILED(hr))
		return 0;
	hr=pSegment->SetParam(GUID_Download,-1,0,0,(void*)dmperf);
	if(FAILED(hr))
		return 0;

	g_mid[index].pSegment=pSegment;
	g_mid[index].index=index;
	pSegment=NULL;
	return 1;
}
MYSOUND_API int PlayMid(int index,bool loop)
{
	if(FAILED(dmperf->PlaySegment(g_mid[index].pSegment,0,0,&g_mid[index].pSegState)))
		return 0;
	if(loop)
	{
		if(FAILED(g_mid[index].pSegment->SetRepeats(DMUS_SEG_REPEAT_INFINITE)))
			return 0;
	}
	return 1;
}
MYSOUND_API int IsPlaying(int index)
{
	if(dmperf->IsPlaying(g_mid[index].pSegment,NULL)==S_OK)
		return 1;
	else
		return 0;
}
MYSOUND_API int StopMid(int index)
{
	dmperf->Stop(g_mid[index].pSegment,NULL,0,0);
	return 1;
}
MYSOUND_API int StopAllMid()
{
	dmperf->Stop(NULL,NULL,0,0);
	return 1;
}
MYSOUND_API int DeleteMid(int index)
{
	g_mid[index].pSegment->SetParam(GUID_Unload,-1,0,0,(void*)dmperf);
	g_mid[index].pSegment->Release();
	g_mid[index].pSegment=NULL;
	g_mid[index].pSegState=NULL;
	g_mid[index].index=-1;
	return 1;
}
MYSOUND_API int DeleteAllMid()
{
	if(dmperf)
		dmperf->Stop(NULL,NULL,0,0);
	for(int i=0;i<max_midi_file;i++)
	{
		if(g_mid[i].index!=-1)
		{
			DeleteMid(i);
		}
	}
	return 1;
}
MYSOUND_API int DestroyMusic()
{
	free(g_mid);
	g_mid=NULL;
	if(dmperf)
	{
		dmperf->CloseDown();
		dmperf->Release();
		dmperf=NULL;
	}if(dmloader)
	{
		dmloader->Release();
	}
	CoUninitialize();
	return 1;
}
//////////////////////////////////
MYSOUND_API int InitSound(HWND hWnd,int max_file)
{
	if(FAILED(DirectSoundCreate(NULL,&lpds,NULL)))
		return 0;
	if(FAILED(lpds->SetCooperativeLevel(hWnd,DSSCL_NORMAL)))
		return 0;
	max_wave_file=max_file;
	g_dsbuffer=(LPDIRECTSOUNDBUFFER*)malloc(sizeof(LPDIRECTSOUNDBUFFER)*max_wave_file);
	for(int i=0;i<max_wave_file;i++)
	{
		g_dsbuffer[i]=NULL;
	}
	memset(CopyBufferCount,-1,sizeof(int)*max_copy_count);
	return 1;
}

MYSOUND_API int LoadWave(int index,char *filename)
{
	HMMIO hwave;
	MMCKINFO parent,child;
	WAVEFORMATEX wmfx;
	
	parent.ckid=(FOURCC)0;
	parent.cksize=0;
	parent.dwDataOffset=0;
	parent.fccType=(FOURCC)0;
	parent.dwFlags=0;
	child=parent;
	if((hwave=mmioOpen(filename,NULL,MMIO_READ|MMIO_ALLOCBUF))==NULL)
		return 0;
	parent.fccType=mmioFOURCC('W','A','V','E');
	if(mmioDescend(hwave,&parent,NULL,MMIO_FINDRIFF))
	{
		mmioClose(hwave,0);
		return 0;
	}
	child.ckid=mmioFOURCC('f','m','m',' ');
	if(mmioDescend(hwave,&child,&parent,0))
	{
		mmioClose(hwave,0);
		return 0;
	}
	if(mmioRead(hwave,(char*)&wmfx,sizeof(wmfx))!=sizeof(wmfx))
	{
		mmioClose(hwave,0);
		return 0;
	}
	if(wmfx.wFormatTag!=WAVE_FORMAT_PCM)
	{
		mmioClose(hwave,0);
		return 0;
	}
	if(mmioAscend(hwave,&child,0))
	{
		mmioClose(hwave,0);
		return 0;
	}
	child.ckid=mmioFOURCC('d','a','t','a');
	if(mmioDescend(hwave,&child,&parent,MMIO_FINDCHUNK))
	{
		mmioClose(hwave,0);
		return 0;
	}
	UCHAR *snd_buffer=(UCHAR*)malloc(sizeof(UCHAR)*child.cksize);
	if(mmioRead(hwave,(char*)snd_buffer,child.cksize)==-1)
	{
		mmioClose(hwave,0);
		return 0;
	}
	mmioClose(hwave,0);

	//创建缓冲区	
	DWORD dwBufferSize=1024000*5;
	DSBUFFERDESC dsbd;
	memset(&dsbd,0,sizeof(dsbd));
	dsbd.dwSize=sizeof(dsbd);
	dsbd.dwFlags=DSBCAPS_CTRLFREQUENCY|DSBCAPS_CTRLPAN|DSBCAPS_CTRLVOLUME
		|DSBCAPS_STATIC|DSBCAPS_LOCSOFTWARE;
	if(dwBufferSize>child.cksize)
		dwBufferSize=child.cksize;
	dsbd.dwBufferBytes=dwBufferSize;
	dsbd.lpwfxFormat=&wmfx;
	
	if(g_dsbuffer[index])
	{
		DeleteSound(index);
	}
	if(FAILED(lpds->CreateSoundBuffer(&dsbd,&g_dsbuffer[index],NULL)))
		return 0;
	//锁定缓冲区
	UCHAR *audio_ptr1,*audio_ptr2;
	DWORD audio_len1,audio_len2;
	if(FAILED(g_dsbuffer[index]->Lock(0,child.cksize,(void**)&audio_ptr1,&audio_len1,
		(void**)&audio_ptr2,&audio_len2,DSBLOCK_ENTIREBUFFER)))
		return 0;
	//向缓冲区写入数据
	memcpy(audio_ptr1,snd_buffer,audio_len1);
	memcpy(audio_ptr2,snd_buffer+audio_len1,audio_len2);
	//解除锁定
	if(FAILED(g_dsbuffer[index]->Unlock(audio_ptr1,audio_len1,audio_ptr2,audio_len2)))
		return 0;
	return 1;
}
MYSOUND_API int CopySoundBuffer(int index)
{
	if(!g_dsbuffer[index])
		return -1;
    int i = 0;
	for(;i<max_wave_file;i++)
	{
		if(g_dsbuffer[i]==NULL)
			break;
	}
	if(i>=max_wave_file)
		return -1;
	if(FAILED(lpds->DuplicateSoundBuffer(g_dsbuffer[index], &g_dsbuffer[i])))
		return -1;
	return i;
}
MYSOUND_API int IsSounPlaying(int index)
{
	DWORD status;
	if(FAILED(g_dsbuffer[index]->GetStatus(&status)))
		return 0;
	if(status==DSBSTATUS_LOOPING || status==DSBSTATUS_PLAYING)
		return 1;
	return 0;
}
MYSOUND_API int PlaySound(int index,bool loop,bool bfromhead)
{	
	if(!g_dsbuffer[index])
		return 0;
	int i;
	for(i=0;i<max_copy_count;i++)
	{
		if(CopyBufferCount[i]!=-1)
			if(!IsSounPlaying(CopyBufferCount[i]))
			{
				DeleteSound(CopyBufferCount[i]);
				CopyBufferCount[i]=-1;
			}
	}
	DWORD dwFlags=0;
	if(loop)
		dwFlags=DSBPLAY_LOOPING;
	if(bfromhead)
	{
		StopSound(index);
		if(FAILED(g_dsbuffer[index]->Play(0,0,dwFlags)))
			return 0;
	}
	else
	{
		int r=CopySoundBuffer(index);
		if(-1==r)
			return 0;
		for(i=0;i<max_copy_count;i++)
		{
			if(CopyBufferCount[i]==-1)
			{
				CopyBufferCount[i]=r;
				if(FAILED(g_dsbuffer[r]->Play(0,0,dwFlags)))
					return 0;
				break;
			}
		}
	}
	return 1;
}
MYSOUND_API int SetVolum(int index,long volume)
{
	if(!g_dsbuffer[index])
		return 0;
	if(volume<0)
		volume=0;
	if(volume>100)
		volume=100;
	if(FAILED(g_dsbuffer[index]->SetVolume(-10*(100-volume))))
		return 0;
	return 1;

}
MYSOUND_API int SetFrequncy(int index,long fre)
{
	if(FAILED(g_dsbuffer[index]->SetFrequency(fre)))
		return 0;
	return 1;
}
MYSOUND_API int SetPan(int index,long lpan)
{
	if(!g_dsbuffer[index])
		return 0;
	if(FAILED(g_dsbuffer[index]->SetPan(lpan)))
		return 0;
	return 1;
}
MYSOUND_API int StopSound(int index)
{
	if(!g_dsbuffer[index])
		return 0;
	if(FAILED(g_dsbuffer[index]->Stop()))
		return 0;
	return 1;
}
MYSOUND_API int StopAllSound()
{
	for(int i=0;i<max_wave_file;i++)
		StopSound(i);
	return 1;
}
MYSOUND_API int DeleteSound(int index)
{
	if(g_dsbuffer[index])
		g_dsbuffer[index]->Release();
	g_dsbuffer[index]=NULL;
	for(int i=0;i<max_copy_count;i++)
		if(CopyBufferCount[i]==index)
			CopyBufferCount[i]=-1;
	return 1;
}
MYSOUND_API int DeleteAllSound()
{
	for(int i=0;i<max_wave_file;i++)
	{
		if(g_dsbuffer)
			DeleteSound(i);
	}
	return 1;
}
MYSOUND_API int DestroySound()
{
	free(g_dsbuffer);
	g_dsbuffer=NULL;
	if(lpds)
		lpds->Release();
	lpds=NULL;
	return 1;
}
//////////////////////////////////////////
