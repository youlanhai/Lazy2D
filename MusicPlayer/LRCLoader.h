#pragma once

#include "../G2DEngine/Utilities.h"

#include <vector>
#include <string>

struct LRC
{
    int time;           //��ǰ���ʱ��
    std::string lyrics; //��ǰ��ʡ�
    LRC(void) : time(0) {}
    LRC(int t,char *s) : time(t),lyrics(s) {}
};

//��ʼ�����
class CLRCLoader : public IBaseObject
{
public:
    CLRCLoader(void);
    ~CLRCLoader(void);
    bool load(char *szFileName);
    void analysis(char *str);
    void clear(void);

    inline LRC * at(int index)
    {
        if(index>=0 && (UINT)index<m_vecLRC.size())
        {
            m_nCurIndex = index;
            return &(m_vecLRC.at(index));
        }
        else return NULL;
    }
    inline LRC * atCur(void){ return at(m_nCurIndex); }
    inline LRC * atNext( void ){ return at(++m_nCurIndex); }
    inline std::vector<LRC> & getVec( void ){ return m_vecLRC; }

    inline void setCurIndex(int index){ m_nCurIndex = index; }
    inline int getCurIndex( void ){ return m_nCurIndex; }
private:
    std::vector<LRC> m_vecLRC;
    int     m_nCurIndex;//��ǰ�ѷ��ʵ�������
};
