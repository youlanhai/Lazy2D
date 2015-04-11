/************************************************************************/
/*D3DTimer.h                                                            */
/*��ʱ���������Ϸ��֡ˢ����fps��                                       */
/************************************************************************/
#pragma once

#include "Utilities.h"

class CFPS : public IBaseObject
{
public:
	CFPS(void);
	void init( void );
	void updateFPS(void);	//ÿ֡������ִ�У�
	float getCurTime(void);	//��õ�ǰʱ��
	inline int getStartTime(void){ return m_nStartTime; }
	inline int getFrameCount(void){ return m_nFrameCount; }
	inline float getFPS(void){ return m_fFPS; }
    inline float getElapse(){ return m_fElapse; }
private:
	int m_nStartTime;		//����ʱ��
	int m_nFrameCount;		//����Ϸ���������ھ�����֡��
	int m_nLastUpdateTime;	//�ϴθ���ʱ��ʱ��
	int m_nLastUpdateFrame;	//�ϴθ���ʱ��֡��
	float m_fFPS;			//֡ˢ����
    float m_fElapse;
};