#pragma once

#define DIRECTSOUND_VERSION 0x0800

#include "dmksctrl.h"
#include "dmusicc.h"
#include "dmusici.h"
#include "dmusicf.h"

// #ifndef ISOUND_INTERFACE
// #define ISOUND_INTERFACE
// 
// struct ISound
// {
//     ~ISound(){}
//     virtual bool play()=0;
//     virtual bool stop()=0;
//     virtual float setVolum(float v)=0;
//     virtual float getVolum( void )=0;
// };
// 
// #endif 

/* CDxMusic
** ²¥·ÅMIDÎÄ¼þ
*/
class CDxMusic
{
public:
    struct DMUSIC_MID
    {
        IDirectMusicSegment *pSegment;
        IDirectMusicSegmentState *pSegState;
        DMUSIC_MID(void) : pSegment(0),pSegState(0) { }
    };
public:
    CDxMusic(void);
    ~CDxMusic(void);
    ///////////////////////////////////
    bool init(HWND hWnd,int max_file=10);
    bool load(int index,char *filename);
    bool play(int index,bool loop=false,bool bfromhead=false);
    bool isPlaying(int index);
//    bool setFrequncy(int index,long fre);
//    bool setPan(int index,long lpan);
    float setVolum(int index,float volume);
    bool stop(int index);
    void stopAll();
    void deleteOne(int index);
    void deleteAll();
    void destroy();
private:

    DMUSIC_MID              *m_pMid;
    IDirectMusicPerformance *m_pDmperf;
    IDirectMusicLoader      *m_pDmloader;
    int                     m_fileAmount;
};
