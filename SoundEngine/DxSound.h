#pragma once

#include "dsound.h"
#include <vector>

/*音效类。*/
class CDxSound
{
protected:
    struct CopyDSBuffer
    {
        int     copyIndex;  //复制来源的索引
        IDirectSoundBuffer8 *pDSBuffer;
        CopyDSBuffer(void) : copyIndex(0),pDSBuffer(0) 
        {
        }
        CopyDSBuffer(int index,IDirectSoundBuffer8 *p) 
            : copyIndex(index),pDSBuffer(p) 
        {
        }
    };
public:
	CDxSound(void);
	~CDxSound(void);

    /*初始化音效。*/
	bool init(HWND hWnd,int max_file=50);

    /*加载wav文件。*/
	bool loadWave(int index,char* filename);

    /*从给定文件中加载wav文件。*/
    bool loadFromFile(char *szFileName);

    /*播放。*/
	bool play(int index,bool loop=false,bool bfromhead=false,bool bCopy=true);

    /*是否正在播放。*/
	bool isPlaying(int index);

    /*设置频率。*/
	bool setFrequncy(int index,long fre);

    /*设置平衡。*/
	bool setPan(int index,long lpan);

    /*设置音量。*/
	bool setVolum(int index,long volume);

    /*停止播放。*/
	bool stop(int index);

    /*停止所有播放。*/
	void stopAll();

    /*释放资源。*/
	void deleteSound(int index);

protected:
    void deleteAllSound();
    void destroy();
    int copyBuffer(int index);
    void releaseUnuseCopyBuffer();

private:
	IDirectSound8        *m_lpDS;

    std::vector<IDirectSoundBuffer8*>   m_vecDSBuffer;      //声音缓冲区
    std::vector<CopyDSBuffer>           m_vecDSCopyBuffer;  //拷贝缓冲区
};