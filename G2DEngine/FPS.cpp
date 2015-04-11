//D3DTimer.cpp 
#include <Windows.h>
#include "FPS.h"

CFPS::CFPS(void)
{
	m_nFrameCount=0;
	m_nLastUpdateFrame=0;
	m_nLastUpdateTime=0;
    m_fElapse = 0;
	m_nStartTime=timeGetTime();
}

void CFPS::init( void )
{
	m_nStartTime=timeGetTime();
}

void CFPS::updateFPS( void )
{
	++m_nFrameCount;
	int nCurrentTime=timeGetTime();

    static int nLastTime = nCurrentTime;
    m_fElapse = (nCurrentTime - nLastTime) / 1000.0f;
    nLastTime = nCurrentTime;

	if(nCurrentTime - m_nLastUpdateTime > 1000)//每秒更新一次
	{
		float dt=(nCurrentTime-m_nLastUpdateTime)/1000.0f;
		m_fFPS=float(m_nFrameCount-m_nLastUpdateFrame)/dt;
		m_nLastUpdateFrame=m_nFrameCount;
		m_nLastUpdateTime=nCurrentTime;
	}
}
float CFPS::getCurTime(void)
{
	return (float)timeGetTime();
}