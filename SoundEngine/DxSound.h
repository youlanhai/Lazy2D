#pragma once

#include "dsound.h"
#include <vector>

/*��Ч�ࡣ*/
class CDxSound
{
protected:
    struct CopyDSBuffer
    {
        int     copyIndex;  //������Դ������
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

    /*��ʼ����Ч��*/
	bool init(HWND hWnd,int max_file=50);

    /*����wav�ļ���*/
	bool loadWave(int index,char* filename);

    /*�Ӹ����ļ��м���wav�ļ���*/
    bool loadFromFile(char *szFileName);

    /*���š�*/
	bool play(int index,bool loop=false,bool bfromhead=false,bool bCopy=true);

    /*�Ƿ����ڲ��š�*/
	bool isPlaying(int index);

    /*����Ƶ�ʡ�*/
	bool setFrequncy(int index,long fre);

    /*����ƽ�⡣*/
	bool setPan(int index,long lpan);

    /*����������*/
	bool setVolum(int index,long volume);

    /*ֹͣ���š�*/
	bool stop(int index);

    /*ֹͣ���в��š�*/
	void stopAll();

    /*�ͷ���Դ��*/
	void deleteSound(int index);

protected:
    void deleteAllSound();
    void destroy();
    int copyBuffer(int index);
    void releaseUnuseCopyBuffer();

private:
	IDirectSound8        *m_lpDS;

    std::vector<IDirectSoundBuffer8*>   m_vecDSBuffer;      //����������
    std::vector<CopyDSBuffer>           m_vecDSCopyBuffer;  //����������
};