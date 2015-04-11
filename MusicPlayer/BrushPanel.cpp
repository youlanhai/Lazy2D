#include <Windows.h>
#include "BrushPanel.h"

#include <fstream>

CBrushPanel::CBrushPanel(void)
{
    m_bEanblePen = true;
}

CBrushPanel::~CBrushPanel(void)
{
}
bool CBrushPanel::loadPanel(char *szFilename)
{
    clear();
    std::ifstream fin(szFilename);
    if (fin.bad() || fin.fail())
    {
        return false;
    }
    int n;
    fin>>n;
    m_vecBrush.reserve(n);
    m_vecPen.reserve(n);
    int r,g,b;
    for(int i=0;i<n;++i)
    {
        fin>>r>>g>>b;
        if (fin.bad() || fin.fail() )
        {
            break;
        }
        m_vecBrush.push_back(CreateSolidBrush(RGB(r,g,b)));
        if (m_bEanblePen)
        {
            m_vecPen.push_back(CreatePen(PS_SOLID,1,RGB(r,g,b)));
        }
    }

    fin.close();
    return true;
}

void CBrushPanel::createRandom(int n)
{
    clear();
    m_vecBrush.reserve(n);
    m_vecPen.reserve(n);
    for (int i=0;i<n;++i)
    {
        COLORREF cr=RGB(rand()%255,rand()%255,rand()%255);
        m_vecBrush.push_back(CreateSolidBrush(cr));
        if (m_bEanblePen)
        {
            m_vecPen.push_back(CreatePen(PS_SOLID,1,cr));
        }
    }
}

void CBrushPanel::clear()
{
    for (UINT i=0; i<m_vecBrush.size(); ++ i)
    {
        DeleteObject(m_vecBrush.at(i));
    }
    m_vecBrush.clear();

    for (UINT i=0; i<m_vecPen.size(); ++ i)
    {
        DeleteObject(m_vecPen.at(i));
    }
    m_vecPen.clear();
}