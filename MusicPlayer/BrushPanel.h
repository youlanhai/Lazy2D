#pragma once

#include "../G2DEngine/Utilities.h"
#include <vector>

class CBrushPanel : public IBaseObject
{
public:
    CBrushPanel(void);
    ~CBrushPanel(void);
    bool loadPanel(char *szFilename);
    void createRandom(int n);
    void clear();

    inline HBRUSH atBrush(int index)
    {
        if (index>=0 && (UINT)index<m_vecBrush.size())
        {
            return m_vecBrush.at(index);
        } 
        else return NULL;
    }
    inline HPEN atPen(int index)
    {
        if (index>=0 && (UINT)index<m_vecPen.size())
        {
            return m_vecPen.at(index);
        } 
        else return NULL;
    }

    inline int size(void){ return m_vecBrush.size(); }

    inline bool getPenEnable(void){ return m_bEanblePen; }
    inline void setPenEnable(bool s){ m_bEanblePen = s; }
public:
    std::vector<HBRUSH> m_vecBrush;
    std::vector<HPEN>   m_vecPen;
private:
    bool    m_bEanblePen;
};
