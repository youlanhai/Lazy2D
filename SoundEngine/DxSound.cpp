//DxSound.cpp 
#include <windows.h>
#include <tchar.h>
#include <mmsystem.h>
#include "dsound.h"

#include "DxSound.h"


#include <fstream>
#include <cassert>
 

CDxSound::CDxSound(void) : m_lpDS(0)
{
	
}

CDxSound::~CDxSound(void)
{
	destroy();
}

bool CDxSound::init(HWND hWnd,int max_file)
{
	if(FAILED(DirectSoundCreate8(NULL,&m_lpDS,NULL)))
	{
		return false;
	}
	if(FAILED(m_lpDS->SetCooperativeLevel(hWnd,DSSCL_NORMAL)))
	{
		return false;
	}

    m_vecDSBuffer.resize(max_file,NULL);
    m_vecDSCopyBuffer.reserve(max_file);
	return true;
}

void CDxSound::releaseUnuseCopyBuffer()
{
    for(size_t i=0; i<m_vecDSCopyBuffer.size(); ++i)
    {
        if(m_vecDSCopyBuffer[i].pDSBuffer != NULL)
        {
            //判断是否正在播放
            DWORD status;
            if(FAILED(m_vecDSCopyBuffer[i].pDSBuffer->GetStatus(&status)))
            {
                continue;
            }
            if(status!=DSBSTATUS_LOOPING && status!=DSBSTATUS_PLAYING)
            {
                m_vecDSCopyBuffer[i].pDSBuffer->Release();
                m_vecDSCopyBuffer[i].pDSBuffer = NULL;
            }
        }
    }
}

int CDxSound::copyBuffer(int index)
{
    if (m_vecDSBuffer.at(index) == NULL)
    {
        return -1;
    }

    releaseUnuseCopyBuffer();

    IDirectSoundBuffer8 *pBuffer8 = NULL;
	if(FAILED(m_lpDS->DuplicateSoundBuffer(m_vecDSBuffer[index],
        (IDirectSoundBuffer**)&pBuffer8)))
	{
		return -1;
    }

    size_t i;
    for(i=0; i<m_vecDSCopyBuffer.size(); ++i)
    {
        if(m_vecDSCopyBuffer[i].pDSBuffer == NULL)
        {
            break;
        }
    }
    if(i == m_vecDSCopyBuffer.size())
    {
        m_vecDSCopyBuffer.push_back(CopyDSBuffer(index,pBuffer8));
    }
    else
    {
        m_vecDSCopyBuffer[i] = CopyDSBuffer(index,pBuffer8);
    }
	return i;
}

bool CDxSound::isPlaying(int index)
{
    if (index<0 || (size_t)index>=m_vecDSBuffer.size())
    {
        return false;
    }
	DWORD status;
	if(FAILED(m_vecDSBuffer[index]->GetStatus(&status)))
	{
		return false;
	}
	if(status==DSBSTATUS_LOOPING || status==DSBSTATUS_PLAYING)
	{
		return true;
	}
	return false;
}

bool CDxSound::play(int index,bool loop,bool bfromhead,bool bCopy)
{	
    if (index<0 || (size_t)index>=m_vecDSBuffer.size())
    {
        return false;
    }
	if(NULL == m_vecDSBuffer[index])
	{
		return false;
	}
	DWORD dwFlags=0;
	if(loop)
	{
		dwFlags=DSBPLAY_LOOPING;
	}
	if(!isPlaying(index))//当前索引没有播放
	{
		if(FAILED(m_vecDSBuffer[index]->Play(0,0,dwFlags)))
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	if(bfromhead)//重头播放
	{
		stop(index);
		if(FAILED(m_vecDSBuffer[index]->Play(0,0,dwFlags)))
		{
			return false;
		}
        else return true;
	}
	else if(bCopy)//复制副本播放
	{
		int r=copyBuffer(index);
		if(-1 == r)
		{
			return false;
		}
        else
        {
            if(FAILED(m_vecDSCopyBuffer[r].pDSBuffer->Play(0,0,0)))
            {
                return false;
            }
        }
	}
	return true;
}

bool CDxSound::setVolum(int index,long volume)
{
    if (index<0 || (size_t)index>=m_vecDSBuffer.size())
    {
        return false;
    }
    if(NULL == m_vecDSBuffer[index])
    {
        return false;
    }
	if(volume<0)
	{
		volume=0;
	}
	if(volume>100)
	{
		volume=100;
	}
	if(FAILED(m_vecDSBuffer[index]->SetVolume(-10*(100-volume))))
	{
		return false;
	}
	return true;
}


bool CDxSound::setFrequncy(int index,long fre)
{
    if (index<0 || (size_t)index>=m_vecDSBuffer.size())
    {
        return false;
    }
    if(NULL == m_vecDSBuffer[index])
    {
        return false;
    }
	if(FAILED(m_vecDSBuffer[index]->SetFrequency(fre)))
	{
		return false;
	}
	return true;
}


bool CDxSound::setPan(int index,long lpan)
{
    if (index<0 || (size_t)index>=m_vecDSBuffer.size())
    {
        return false;
    }
    if(NULL == m_vecDSBuffer[index])
    {
        return false;
    }
	if(FAILED(m_vecDSBuffer[index]->SetPan(lpan)))
	{
		return false;
	}
	return true;
}


bool CDxSound::stop(int index)
{
    if (index<0 || (size_t)index>=m_vecDSBuffer.size())
    {
        return false;
    }
    if(NULL == m_vecDSBuffer[index])
    {
        return false;
    }
	if(FAILED(m_vecDSBuffer[index]->Stop()))
	{
		return false;
	}
	return true;
}


void CDxSound::stopAll()
{
	for(size_t i=0;i<m_vecDSBuffer.size();i++)
	{
		stop(i);
	}
    for(size_t i=0;i<m_vecDSCopyBuffer.size();i++)
    {
        if(m_vecDSCopyBuffer[i].pDSBuffer != NULL)
        {
            m_vecDSCopyBuffer[i].pDSBuffer->Stop();
        }
    }
}


void CDxSound::deleteSound(int index)
{
    if (index<0 || (size_t)index>=m_vecDSBuffer.size())
    {
        return ;//false;
    }
    if(NULL != m_vecDSBuffer[index])
    {
        m_vecDSBuffer[index]->Release();
        m_vecDSBuffer[index] = NULL;
	    for(size_t i=0;i<m_vecDSCopyBuffer.size();i++)
	    {
		    if(m_vecDSCopyBuffer[i].copyIndex==index &&
                m_vecDSCopyBuffer[i].pDSBuffer!=NULL)
		    {
                m_vecDSCopyBuffer[i].pDSBuffer->Release();
			    m_vecDSCopyBuffer[i].pDSBuffer=NULL;
		    }
        }
    }
}


void CDxSound::deleteAllSound()
{
	for(size_t i=0; i<m_vecDSBuffer.size(); ++i)
	{
		deleteSound(i);
	}
}


void CDxSound::destroy()
{
    stopAll();
    deleteAllSound();
	if(m_lpDS != NULL)
	{
		m_lpDS->Release();
	}
	m_lpDS=NULL;
}

/*从给定文件中加载wav文件。*/
bool CDxSound::loadFromFile(char *szFileName)
{
    assert(m_lpDS!=NULL && szFileName!=NULL);
    std::ifstream fin(szFileName);
    if (!fin.good())
    {
        return false;
    }
    int count;
    fin>>count;
    char szBuffer[256];

    for (int i=0; i<count; ++i)
    {
        fin>>szBuffer;
        if(!loadWave(i,szBuffer))
        {
            TCHAR info[256];
            _stprintf(info, _T("加载wav文件[%s]失败！"), szBuffer);
            OutputDebugString(info);
        }
    }

    fin.close();

    return true;
}

bool CDxSound::loadWave(int index,char* filename)
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
	if((hwave=mmioOpenA(filename,NULL,MMIO_READ|MMIO_ALLOCBUF))==NULL)
	{
		return false;
	}
	parent.fccType=mmioFOURCC('W','A','V','E');
	if(mmioDescend(hwave,&parent,NULL,MMIO_FINDRIFF))
	{
		mmioClose(hwave,0);
		return false;
	}
	child.ckid=mmioFOURCC('f','m','m',' ');
	if(mmioDescend(hwave,&child,&parent,0))
	{
		mmioClose(hwave,0);
		return false;
	}
	if(mmioRead(hwave,(char*)&wmfx,sizeof(wmfx))!=sizeof(wmfx))
	{
		mmioClose(hwave,0);
		return false;
	}
	if(wmfx.wFormatTag!=WAVE_FORMAT_PCM)
	{
		mmioClose(hwave,0);
		return false;
	}
	if(mmioAscend(hwave,&child,0))
	{
		mmioClose(hwave,0);
		return false;
	}
	child.ckid=mmioFOURCC('d','a','t','a');
	if(mmioDescend(hwave,&child,&parent,MMIO_FINDCHUNK))
	{
		mmioClose(hwave,0);
		return false;
	}
	UCHAR *snd_buffer=new UCHAR[sizeof(UCHAR)*child.cksize];
	if(mmioRead(hwave,(char*)snd_buffer,child.cksize)==-1)
	{
		mmioClose(hwave,0);
		return false;
	}
	mmioClose(hwave,0);

	//创建缓冲区	
	DWORD dwBufferSize=1000000;
	DSBUFFERDESC dsbd;
	memset(&dsbd,0,sizeof(dsbd));
	dsbd.dwSize=sizeof(dsbd);
	dsbd.dwFlags=DSBCAPS_CTRLFREQUENCY|DSBCAPS_CTRLPAN|DSBCAPS_CTRLVOLUME
		|DSBCAPS_STATIC|DSBCAPS_LOCSOFTWARE;
	if(dwBufferSize>child.cksize)
		dwBufferSize=child.cksize;
	dsbd.dwBufferBytes=dwBufferSize;
	dsbd.lpwfxFormat=&wmfx;

    IDirectSoundBuffer8 *pBuffer8 = NULL;
	if(FAILED(m_lpDS->CreateSoundBuffer(&dsbd,
        (LPDIRECTSOUNDBUFFER*)&pBuffer8,NULL)))
	{
		return false;
	}

	//锁定缓冲区
	UCHAR *audio_ptr1,*audio_ptr2;
	DWORD audio_len1,audio_len2;
	if(FAILED(pBuffer8->Lock(0,child.cksize,(void**)&audio_ptr1,&audio_len1,
		(void**)&audio_ptr2,&audio_len2,DSBLOCK_ENTIREBUFFER)))
	{
		return false;
	}
	//向缓冲区写入数据
	memcpy(audio_ptr1,snd_buffer,audio_len1);
	memcpy(audio_ptr2,snd_buffer+audio_len1,audio_len2);
	//解除锁定
	pBuffer8->Unlock(audio_ptr1,audio_len1,audio_ptr2,audio_len2);

	delete [] snd_buffer;

    if ((size_t)index >= m_vecDSBuffer.size())
    {
        m_vecDSBuffer.push_back(pBuffer8);
    }
    else
    {
        if (m_vecDSBuffer[index] != NULL)
        {
            deleteSound(index);
        }
        m_vecDSBuffer[index] = pBuffer8;
    }

	return true;
}
//////////////////////////////////////////