#pragma once

#include "..\g2dengine\gamegui.h"
#include "..\SoundEngine\audiere.h"


class CLRCLoader;

class CMp3Player : public CUIStatic  
{
public:
    CMp3Player(void);
    ~CMp3Player(void);

    bool loadMp3(char *szMp3);
    //显示
    void render(HDC hDC);
    void update(int nElapse);

    //播放
    void play(void) ;
    //暂停
    void pause(void);
    //停止
    void stop(void) ;

    //是否正在播放
    inline bool isPlaying(void){ return m_pStream->isPlaying(); }

    //重置。将播放位置置于开头。
    inline void reset(void){ m_pStream->reset(); } ;
    
    //设置重复播放
    inline void setRepeat(bool repeat){ m_pStream->setRepeat(repeat); }
    inline bool getRepeat(void){ return m_pStream->getRepeat(); }

    //设置音量
    inline void setVolume(float volume);
    inline float getVolume(void){ return m_pStream->getVolume(); }

    //设置音效平衡
    inline void setPan(float pan){ m_pStream->setPan(pan); }
    inline float getPan(void){ return m_pStream->getPan(); }

    inline void setPitchShift(float shift){ m_pStream->setPitchShift(shift); }
    inline float getPitchShift(void){ return m_pStream->getPitchShift(); }

    inline bool isSeekable(void){ return m_pStream->isSeekable(); }
    //获得音频长度。单位为帧。
    inline int getLength(void){ return m_pStream->getLength(); }
    //设置播放位置。单位为帧。
    inline void setPosition(int position){ m_pStream->setPosition(position); }
    inline int getPosition(void){ return m_pStream->getPosition(); }

    inline void setFade(bool fade){ m_bFade = fade; }

    inline void setFadeTime(int time){ m_nFadeTime = time; }

private:
    RefPtr<CLRCLoader>    m_pLrcLoader;
    bool                  m_bLRCEnable;
    audiere::RefPtr<audiere::AudioDevice> m_pDevice;
    audiere::RefPtr<audiere::OutputStream> m_pStream;

    int     m_nCommand;     //控制命令。1-播放，2-暂停，3-停止。
    bool    m_bFade;        //是否淡入淡出
    int     m_nFadeTime;    //淡化时间
    int     m_nElapseTime;  //已经淡化时间
    float   m_fOldVolum;    //音量
};

//设置音量
inline void CMp3Player::setVolume(float volume)
{ 
    m_fOldVolum = volume;
    if (m_fOldVolum<0.0f)
    {
        m_fOldVolum = 0.0f;
    }
    else if(m_fOldVolum>1.0f)
    {
        m_fOldVolum = 1.0f;
    }
    m_pStream->setVolume(m_fOldVolum);
}