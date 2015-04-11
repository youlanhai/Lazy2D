/************************************************************************/
/*D3DTimer.h                                                            */
/*定时器：获得游戏的帧刷新率fps。                                       */
/************************************************************************/
#pragma once

#include "Utilities.h"

class CFPS : public IBaseObject
{
public:
	CFPS(void);
	void init( void );
	void updateFPS(void);	//每帧都必须执行！
	float getCurTime(void);	//获得当前时间
	inline int getStartTime(void){ return m_nStartTime; }
	inline int getFrameCount(void){ return m_nFrameCount; }
	inline float getFPS(void){ return m_fFPS; }
    inline float getElapse(){ return m_fElapse; }
private:
	int m_nStartTime;		//启动时间
	int m_nFrameCount;		//从游戏启动到现在经过的帧数
	int m_nLastUpdateTime;	//上次更新时的时间
	int m_nLastUpdateFrame;	//上次更新时的帧数
	float m_fFPS;			//帧刷新率
    float m_fElapse;
};