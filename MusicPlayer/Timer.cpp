#include <windows.h>
#include "Timer.h"

CTimer::CTimer(void)
{
}

CTimer::~CTimer(void)
{
}

void CTimer::enable(int id,bool en)
{
    (at(id))->bEnable = en;
}
void CTimer::update()
{
    int curTime = timeGetTime();
    std::vector<TimerInfo>::iterator it;
    for(it=m_vecTimer.begin(); it!=m_vecTimer.end();++it)
    {
        (*it).update(curTime);
    }
}
int CTimer::set(int id,int interval,TIMERCALLBACK pCallBack,void *pParam)
{
    TimerInfo ti;
    ti.nID = id;
    ti.nTimeInterval = interval;
    ti.pCallBack = pCallBack;
    ti.pParam = pParam;
    ti.reset(timeGetTime());

    m_vecTimer.push_back(ti);

    return id;
}

bool CTimer::isOver(int id)
{
    return at(id)->bOver;
}

void CTimer::kill(int id)
{
    std::vector<TimerInfo>::iterator it;
    for(it=m_vecTimer.begin(); it!=m_vecTimer.end();++it)
    {
        if((*it).nID == id)
        {
            m_vecTimer.erase(it);
            break;
        }
    }
}

TimerInfo * CTimer::at(int id)
{
    std::vector<TimerInfo>::iterator it;
    for(it=m_vecTimer.begin(); it!=m_vecTimer.end();++it)
    {
        if((*it).nID == id)
        {
            return &(*it);
        }
    }
    return NULL;
}

void CTimer::setRepeat(int id,bool r)
{
    TimerInfo * pT = at(id);
    if(pT != NULL)
    {
        pT->bRepeat = r;
    }
}
void CTimer::reset(int id)
{
    TimerInfo * pT = at(id);
    if(pT != NULL)
    {
        pT->reset(timeGetTime());
    }
}