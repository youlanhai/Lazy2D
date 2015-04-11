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
    //��ʾ
    void render(HDC hDC);
    void update(int nElapse);

    //����
    void play(void) ;
    //��ͣ
    void pause(void);
    //ֹͣ
    void stop(void) ;

    //�Ƿ����ڲ���
    inline bool isPlaying(void){ return m_pStream->isPlaying(); }

    //���á�������λ�����ڿ�ͷ��
    inline void reset(void){ m_pStream->reset(); } ;
    
    //�����ظ�����
    inline void setRepeat(bool repeat){ m_pStream->setRepeat(repeat); }
    inline bool getRepeat(void){ return m_pStream->getRepeat(); }

    //��������
    inline void setVolume(float volume);
    inline float getVolume(void){ return m_pStream->getVolume(); }

    //������Чƽ��
    inline void setPan(float pan){ m_pStream->setPan(pan); }
    inline float getPan(void){ return m_pStream->getPan(); }

    inline void setPitchShift(float shift){ m_pStream->setPitchShift(shift); }
    inline float getPitchShift(void){ return m_pStream->getPitchShift(); }

    inline bool isSeekable(void){ return m_pStream->isSeekable(); }
    //�����Ƶ���ȡ���λΪ֡��
    inline int getLength(void){ return m_pStream->getLength(); }
    //���ò���λ�á���λΪ֡��
    inline void setPosition(int position){ m_pStream->setPosition(position); }
    inline int getPosition(void){ return m_pStream->getPosition(); }

    inline void setFade(bool fade){ m_bFade = fade; }

    inline void setFadeTime(int time){ m_nFadeTime = time; }

private:
    RefPtr<CLRCLoader>    m_pLrcLoader;
    bool                  m_bLRCEnable;
    audiere::RefPtr<audiere::AudioDevice> m_pDevice;
    audiere::RefPtr<audiere::OutputStream> m_pStream;

    int     m_nCommand;     //�������1-���ţ�2-��ͣ��3-ֹͣ��
    bool    m_bFade;        //�Ƿ��뵭��
    int     m_nFadeTime;    //����ʱ��
    int     m_nElapseTime;  //�Ѿ�����ʱ��
    float   m_fOldVolum;    //����
};

//��������
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