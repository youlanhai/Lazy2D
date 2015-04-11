#include <windows.h>
#include <math.h>

#include "Particle.h"
#include "BrushPanel.h"


void IParticle::reset()//�������е���������
{
    CMList<ParticleAttr>::LinkNode pNode;
    for (pNode=m_lstParticles.begin(); pNode!=NULL; pNode=pNode->next)
    {
        resetParticle(&(pNode->data));
    }
}
void IParticle::addParticle()//���һ������
{
    ParticleAttr attr;
    resetParticle(&attr);
    m_lstParticles.push_front(attr);
}
void IParticle::removeDeadParticle()//�Ƴ��Ѿ�����������
{
    CMList<ParticleAttr>::LinkNode pNode,pPrev;
    pPrev = m_lstParticles.head();
    pNode = m_lstParticles.begin();
    for (pNode=m_lstParticles.begin();  pNode!=m_lstParticles.end(); )
    {
        if (!(pNode->data.bLive))
        {
            pPrev->next = pNode->next;
            pNode = m_lstParticles.deleteNode(pNode);
        }
        else
        {
            pPrev = pNode;
            pNode = pNode->next;
        }
    }
}
void IParticle::clear()//��յ�ǰ����
{
    m_lstParticles.clear();
}
bool IParticle::isEmpty()//��ǰϵͳ�������򷵻�true,����false;
{
    return m_lstParticles.empty();
}
bool IParticle::isDead()//ϵͳ�е��������Ӷ������򷵻�true,���򷵻�false;
{
    CMList<ParticleAttr>::LinkNode pNode;
    for (pNode=m_lstParticles.begin(); pNode!=NULL; pNode=pNode->next)
    {
        if (pNode->data.bLive)
        {
            return false;
        }
    }
    return true;
}
int  IParticle::getRandomData(int bound1, int bound2)
{
    if (bound1 == bound2)
    {
        return 0;
    }
    if(bound1 < bound2)
    {
        int t = bound1;
        bound1 = bound2;
        bound2 = t;
    }
    int dist=bound1-bound2;
    int r = rand()%dist + bound2;
    return r;
}

//////////////////////////////////////////////////////////////////////////
//�Զ����������

#define BMP_WIDTH 800
#define BMP_HEIGHT 600

//#define YLH_BITMAP

#ifdef YLH_BITMAP

#define MAX_LEN 3000000
BYTE  ylh_buffer[MAX_LEN];

void ylh_reset(int len)
{
    memset(ylh_buffer,0,len);
}

void ylh_ellipse(BITMAP *pBm,RECT *rc,COLORREF cr)
{
    if (pBm->bmBitsPixel < 24)
    {
        return;
    }
    BYTE r = GetRValue(cr);
    BYTE g = GetGValue(cr);
    BYTE b = GetBValue(cr);
    RECT rect = *rc;
    if (rect.left<0)
    {
        rect.left = 0;
    }
    if (rect.right >= pBm->bmWidth)
    {
        rect.right = pBm->bmWidth-1;
    }
    if (rect.top<0)
    {
        rect.top = 0;
    }
    if (rect.bottom >= pBm->bmHeight)
    {
        rect.bottom = pBm->bmHeight-1;
    }
    int bits = pBm->bmBitsPixel/8;
    for(int y = rect.top; y<=rect.bottom; ++y)
    {
        int t = y*pBm->bmWidthBytes;
        for(int x = rect.left; x<rect.right; ++x)
        {
            int i = t+x*bits;
            ylh_buffer[i] = b;
            ylh_buffer[i + 1] = g;
            ylh_buffer[i + 2] = r;
        }
    }
}
void ylh_bitblt(HDC hDC,int x,int y,int w,int h,HBITMAP bmp)
{
    HDC memDC = CreateCompatibleDC(0);
    SelectObject(memDC,bmp);

    TransparentBlt(hDC,x,y,w,h,memDC,0,0,w,h,0);

    DeleteDC(memDC);
}

#endif

//////////////////////////////class CExplode////////////////////////////////////

CExplode::CExplode()
{
    m_pBrush = NULL;
}

CExplode::CExplode(int numParticles, Vector2D *pos,CBrushPanel*pBrush)
{
    init(numParticles,pos,pBrush);
}
void CExplode::init(int numParticles, Vector2D *pos,CBrushPanel*pBrush)
{
    clear();
    m_nMaxNum = numParticles;
    m_vCreatePos = *pos;
    m_pBrush = pBrush;
    for (int i=0;i<m_nMaxNum; ++i)
    {
        addParticle();
    }
}
CExplode::~CExplode(void)
{

}
void CExplode::update(int elipse)//����ϵͳ�е���������
{
    CMList<ParticleAttr>::LinkNode pNode=m_lstParticles.begin();
    ParticleAttr *pAttr=NULL;
    while(pNode != NULL)
    {
        pAttr = &(pNode->data);

        float a = 0.1f;
        float dt = elipse * 0.001f;
        float da = a/dt;
        if (da>=1.0f)
        {
            da=0.9f;
        }
        Vector2D vSpeedNew ;
        vSpeedNew.x = pAttr->vSpeed.x * da;
        vSpeedNew.y = pAttr->vSpeed.y * da;
        pAttr->vPos.x += (vSpeedNew.x + pAttr->vSpeed.x)*dt*0.5f;
        pAttr->vPos.y += (vSpeedNew.y + pAttr->vSpeed.y)*dt*0.5f;
        pAttr->vSpeed = vSpeedNew;

        pAttr->nAge += elipse;		
        //�������ӵ�ǰ����
        if(pAttr->nAge>=pAttr->nLifeTime
            || pAttr->vSpeed==Vector2D(0.0f,0.0f))					
        {
            pAttr->bLive = false;
        }
        ///////////////////////
        pNode = pNode->next;
    }
    removeDeadParticle();
}


void CExplode::resetParticle(ParticleAttr *attr)//����һ�����ӵ�����
{
    attr->bLive = true;
    attr->nAge = 0;
    attr->nLifeTime = getRandomData(1000,2000);	//��������Ϊˢ��5��	
    if (m_pBrush != NULL)
    {
        
#ifdef YLH_BITMAP
        attr->crColor = RGB(rand()%255,rand()%255,rand()%255);
#else
        attr->crColor = rand()%(m_pBrush->size()+1);
#endif
    }
    
    attr->vPos = m_vCreatePos;	

    int v=300;
    float dist=float(rand()%v);
    float angle = (rand()%628)/100.0f;//���һ�����ⷽ��
    attr->vSpeed.x = dist*cos(angle);
    attr->vSpeed.y = dist*sin(angle);
}

void CExplode::render(HDC hDC)//��Ⱦ�����е�����
{
    if(m_pBrush == NULL)
    {
        return ;
    }
    if(!m_pBrush->getPenEnable())
    {
        return ;
    }


#ifdef YLH_BITMAP

    HBITMAP bmp = CreateCompatibleBitmap(hDC,BMP_WIDTH,BMP_HEIGHT);
    //HBITMAP bmp = CreateBitmap(BMP_WIDTH,BMP_HEIGHT,1,24,NULL);
    BITMAP bm;
    GetObject(bmp,sizeof(bm),&bm);

    ylh_reset(bm.bmWidthBytes*bm.bmHeight);

#else
    HBRUSH oldBrush=(HBRUSH)SelectObject(hDC,m_pBrush->atBrush(0));
    HPEN oldPen = (HPEN)SelectObject(hDC,m_pBrush->atPen(0));
#endif

    RECT rc;
    CMList<ParticleAttr>::LinkNode pNode;
    ParticleAttr *pAttr=NULL;
    for (pNode=m_lstParticles.begin(); pNode!=NULL; pNode=pNode->next)
    {
        pAttr = &(pNode->data);
        pAttr->vPos.toRect(2,2,&rc);

#ifdef YLH_BITMAP
        ylh_ellipse(&bm,&rc,pAttr->crColor);
#else
        HBRUSH brush=m_pBrush->atBrush(pAttr->crColor);
        SelectObject(hDC,brush);
        HPEN pen=m_pBrush->atPen(pAttr->crColor);
        SelectObject(hDC,pen);
        Ellipse(hDC,rc.left,rc.top,rc.right,rc.bottom);
       
#endif


    }

#ifdef YLH_BITMAP
    SetBitmapBits(bmp,bm.bmHeight*bm.bmWidthBytes,ylh_buffer);
    ylh_bitblt(hDC,0,0,BMP_WIDTH,BMP_HEIGHT,bmp);
    DeleteObject(bmp);
#else
    SelectObject(hDC,oldBrush);
    SelectObject(hDC,oldPen);
#endif

}

/////////////////////////////class CExplode///////////////////////////////
//////////////////////////////////////////////////////////////////////////



////////////////////////////class CSnow///////////////////////////////////
CSnow::CSnow()
{

}
CSnow::~CSnow(void)
{

}
void CSnow::init(int numParticles, RECT *rect)
{
    clear();
    m_nMaxNum = numParticles;
    m_rect = *rect;
    for (int i=0;i<m_nMaxNum; ++i)
    {
        addParticle();
    }
}
void CSnow::update(int elipse)//����ϵͳ�е���������
{
    CMList<ParticleAttr>::LinkNode pNode;
    ParticleAttr *pAttr=NULL;
    for(pNode=m_lstParticles.begin();
        pNode != m_lstParticles.end();
        pNode = pNode->next)
    {
        pAttr = &(pNode->data);
        float fElapse = elipse/1000.0f;//ת��Ϊs
        pAttr->vPos.x += pAttr->vSpeed.x * fElapse;
        pAttr->vPos.y += pAttr->vSpeed.y * fElapse;

        //�������ӵ�ǰ����
        if(!m_rect.isIn(int(pAttr->vPos.x),int(pAttr->vPos.y)))					
        {
            resetParticle(pAttr);
        }
        ///////////////////////
    }
}
void CSnow::render(HDC hDC)//��Ⱦ�����е�����
{
    HPEN oldPen = (HPEN)SelectObject(hDC,GetStockObject(WHITE_PEN));
    HBRUSH oldBrush=(HBRUSH)SelectObject(hDC,GetStockObject(WHITE_BRUSH));
    CMList<ParticleAttr>::LinkNode pNode;
    ParticleAttr *pAttr=NULL;
    for (pNode=m_lstParticles.begin(); pNode!=NULL; pNode=pNode->next)
    {
        pAttr = &(pNode->data);
        RECT rc;
        pAttr->vPos.toRect(pAttr->nAge,pAttr->nAge,&rc);
        Ellipse(hDC,rc.left,rc.top,rc.right,rc.bottom);
    }
    SelectObject(hDC,oldBrush);
    SelectObject(hDC,oldPen);
}
void CSnow::resetParticle(ParticleAttr *attr)//����һ�����ӵ�����
{
    attr->vPos = Vector2D(float(rand()%m_rect.width()),float(m_rect.top));

    attr->vSpeed.x = float(rand()%200-100);
    attr->vSpeed.y = float(rand()%200 + 80);
    attr->nAge = rand()%4+1;//����nAge������ѩ���뾶
}

////////////////////////////class CSnow///////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

CFire::CFire(void)
{
    m_ppFireData = NULL;
    m_bTransparent = true;
    m_nRedution = 3;
}

CFire::~CFire(void)
{
    clear();
}

bool CFire::init(HDC hDC,int w,int h)
{
    clear();

    m_nWidth = w;;
    m_nHeight = h;
    m_ppFireData = new FireColor*[m_nHeight];
    for(int i=0; i<m_nHeight; ++i)
    {
        m_ppFireData[i] = new FireColor[m_nWidth];
    }

    return m_dibBmp.create(hDC,w,h,32);
}


void CFire::put()						//���û�������
{
    for(int i=0;i<m_nWidth;++i)
    {
        int color = 0;
        int ran = rand()%300 ;
        color = (ran + 2*m_ppFireData[m_nHeight-1][i].r)/3;
        if (color>255){ color=255-rand()%10; }
        else if (color<0){ color=0; }
        m_ppFireData[m_nHeight-1][i].r = color;

//         color = (ran-30 + 2*m_ppFireData[m_nHeight-1][i].g)/3;
//         if (color>255){ color=255-rand()%10; }
//         else if (color<0){ color=0; }
//         m_ppFireData[m_nHeight-1][i].g = color;
// 
//         color = (ran-30 + 2*m_ppFireData[m_nHeight-1][i].b)/3;
//         if (color>255){ color=255-rand()%10; }
//         else if (color<0){ color=0; }
//         m_ppFireData[m_nHeight-1][i].b = color;
    }
}

void CFire::update(int elipse)		//���»�������
{
    put();

    for (int y=m_nHeight-2; y>=0; --y)
    {
        for(int x=1; x<m_nWidth-1; ++x)
        {
            int color = 0;
            //R
            //color += m_ppFireData[y][x].r;
            color += m_ppFireData[y+1][x-1].r;
            color += m_ppFireData[y+1][x].r;
            color += m_ppFireData[y+1][x+1].r;
            color = (color-m_nRedution)/3 ;
            if (color < 0){ color = 0; }
            m_ppFireData[y][x].r = color;
//             //G
//             color = 0;
//             color += m_ppFireData[y][x].g;
//             color += m_ppFireData[y+1][x-1].g;
//             color += m_ppFireData[y+1][x].g;
//             color += m_ppFireData[y+1][x+1].g;
//             color = (color-m_nRedution-2)/4 ;
//             if (color < 0){ color = 0; }
//             m_ppFireData[y][x].g = color;
//                  //B
//             color = 0;
//             color += m_ppFireData[y][x].b;
//             color += m_ppFireData[y+1][x-1].b;
//             color += m_ppFireData[y+1][x].b;
//             color += m_ppFireData[y+1][x+1].b;
//             color = (color-m_nRedution-2)/4 ;
//             if (color < 0){ color = 0; }
//             m_ppFireData[y][x].b = color;

        }
    }
    //������λͼ
    BYTE *pFireData = m_dibBmp.getBmpData();
    int len = m_nWidth*4;
    int s = m_nHeight-1;
    for (int y=0;y<m_nHeight;++y,--s)
    {
        memcpy(pFireData+y*m_dibBmp.getWidthBytes(),m_ppFireData[s],len);
    }
}

void CFire::render(HDC hDC,int x,int y)			//��Ⱦ�����е�����
{
    if (m_bTransparent)
    {
        HDC memDC = CreateCompatibleDC(0);
        SelectObject(memDC,m_dibBmp.getBitmap());

        TransparentBlt(hDC,x,y,
            m_nWidth,m_nHeight,
            memDC,0,0,m_nWidth,m_nHeight,0);

        DeleteDC(memDC);
    }
    else
    {
        m_dibBmp.sampleRender(hDC,x,y);
    }
}


void CFire::reset(void)               //���û���
{
    //memset(m_dibBmp.getBmpData(),0,m_dibBmp.getHeight()*m_dibBmp.getWidthBytes());
    for(int i=0; i<m_nHeight; ++i)
    {
        memset(m_ppFireData[i],0,m_nWidth*sizeof(FireColor));
    }
}

void CFire::clear()                   //���ϵͳ
{
    if (m_ppFireData != NULL)
    {
        for(int i=0; i<m_nHeight; ++i)
        {
            delete [] m_ppFireData[i];
        }
        delete [] m_ppFireData;
        m_ppFireData = NULL;
    }
}


//////////////////////////////////////////////////////////////////////////