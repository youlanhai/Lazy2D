#pragma once

#include "../G2DEngine/Utilities.h"

#include <vector>

typedef void (*TIMERCALLBACK)(void *pParam);

struct TimerInfo
{
    int nID;
    int nTimeInterval;//定时时间间隔
    int nStart;     //开始时间
    int nElapse;    //已流逝时间
    bool bRepeat;   //是否循环计时
    bool bOver;     //是否完毕
    bool bEnable;   //是否可用
    TIMERCALLBACK pCallBack;
    void *pParam;

    TimerInfo(void)
    {
        nID=0;
        nTimeInterval=1000;
        nStart=0;
        nElapse=0;
        bRepeat=true;
        bOver=false;
        bEnable=true;
        pCallBack=NULL;
        pParam=NULL;
    }
    void reset(int curTime)
    {
        nStart = curTime;
        nElapse = 0;
        bOver = false;
    }
    void update(int curTime)
    {
        if(!bEnable)
        {
            return;
        }
        if (bOver)
        {
            if (bRepeat)
            {
                reset(curTime);
            }
            return ;
        }
        nElapse = curTime - nStart;
        
        if (nElapse >= nTimeInterval)
        {
            bOver = true;
            if (pCallBack != NULL)
            {
                pCallBack(pParam);
            }
        }
    }
};

class CTimer : public IBaseObject
{
public:
    CTimer(void);
    ~CTimer(void);

    void update(void);
    int set(int id,int interval,TIMERCALLBACK pCallBack=NULL,void *pParam=NULL);
    void setRepeat(int id,bool r);
    void reset(int id);
    void kill(int id);
    bool isOver(int id);
    void enable(int id,bool en);
    TimerInfo * at(int id);
private:
    std::vector<TimerInfo> m_vecTimer;
};
