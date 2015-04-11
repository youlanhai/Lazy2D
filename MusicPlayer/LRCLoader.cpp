#include <Windows.h>
#include "LRCLoader.h"

#include <fstream>

CLRCLoader::CLRCLoader(void)
{
    m_nCurIndex = -1;
    m_vecLRC.reserve(100);
}

CLRCLoader::~CLRCLoader(void)
{
}

bool CLRCLoader::load(char *szFileName)
{
    clear();
    std::ifstream fin(szFileName);
    if(fin.bad() || fin.fail())
    {
        return false;
    }
    char szBuffer[256];
    while(!fin.eof())
    {
        fin.getline(szBuffer,256);
        analysis(szBuffer);
    }
    fin.close();
    return true;
}

void CLRCLoader::analysis(char *str)
{
    int bracket=0;//���š�-1�����ţ�0�����ţ�1������
    LRC lrc;
    char ch;
    int tFlag=0;    //ʱ���ʶ��1��,2��,3����.
    int t = 0;      //������ʱʱ��
    while((*str) != 0)
    {
        ch = *str;
        if (ch == '[')
        {
            if(bracket == 0)
            {
                bracket = -1;
            }
            else break;//�쳣�������˵ڶ���'['
        }
        else if(ch == ']')//��������
        {
            bracket = 1;

            lrc.time += t*10; //��������ʱ�䵥λ��10ms
            lrc.lyrics = (str+1);
            m_vecLRC.push_back(lrc);

            break; //��������
        }
        else if(ch >= 48 && ch <58)
        {
            t = t*10 + ch-48;
        }
        else if(ch == ':')
        {
            if (tFlag == 0)
            {
                tFlag = 1;
                lrc.time += t*60000;    //1����=60��=60000����
                t = 0;
            }
            else break; //�쳣�������˵ڶ���':'��
        }
        else if(ch == '.')
        {
            if( tFlag == 1)
            {
                lrc.time += t*1000; //1s=1000ms
                t = 0;
            }
            else break; //�쳣�������˵ڶ���'.'��
        }
        else
        {
            break;  //�쳣���Ƿ��ַ���
        }

        ++str;
    }
}

void CLRCLoader::clear(void)
{
    m_nCurIndex = -1;
    m_vecLRC.clear();
}