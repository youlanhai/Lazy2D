#pragma once

#include <windows.h>
#include <string>
//宽字符串与字符串的转换
void CharToWString(std::wstring & str,const char *szChar);
WCHAR* CharToWChar(WCHAR *szWChar,const char *szChar);
char* WStringToChar(char *szChar,const std::wstring &str);
char* WCharToChar(char *szChar,const WCHAR *szWChar);

COLORREF getBlendColor(COLORREF cr,double x);
HFONT createFontEx(int height,char *szName);

void changeWorkDirToModulePath();

//区域类
class CGameRect
{
public:
	int x,y;
	int w,h;
	CGameRect(){x=y=w=h=0;}
	CGameRect(int x,int y,int w,int h)
	{
		this->x=x;
		this->y=y;
		this->w=w;
		this->h=h;
	}
	CGameRect(const CGameRect& rc)
	{
		memcpy(this,&rc,sizeof(CGameRect));
	}
    CGameRect(RECT *rc)
    {
        x = rc->left;
        y = rc->top;
        w = rc->right - x;
        h = rc->bottom - y;
    }
	bool IsHit(const CGameRect& rc)
	{
		if(x>rc.x+rc.w || x+w<rc.x || y>rc.y+rc.h || y+h<rc.y)
			return false;
		return true;
	}
	bool IsIn(POINT* pt)
	{
		if(pt->x >= x && pt->x <= x+w && pt->y>=y && pt->y <= y+h)
			return true;
		return false;
	}
	bool IsIn(int sx,int sy)
	{
		if(sx >= x && sx <= x+w && sy>=y && sy <= y+h)
			return true;
		return false;
	}
	RECT* ToRECT()
	{
		static RECT rc;
		rc.left=x;
		rc.top=y;
		rc.right=x+w;
		rc.bottom=y+h;
		return &rc;
	}
};

class CRect : public RECT
{
public:
    CRect(void)
    {
        left = top = right = bottom = 0;
    };
    CRect(int left_,int top_,int right_,int bottom_ )
    {
        left = left_ ;
        right = right_ ;
        top = top_ ;
        bottom = bottom_ ;
    }
    CRect(const CGameRect & rc)
    {
        left = rc.x;
        top = rc.y;
        right = rc.x+rc.w;
        bottom = rc.y+rc.h;
    }
    CRect(const RECT & rc)
    {
        *this = rc;
    }
    CRect(RECT * rc)
    {
        *this = *rc;
    }
    CRect & operator=(const RECT & rc)
    {
        left = rc.left;
        top = rc.top;
        right = rc.right;
        bottom = rc.bottom;
        return (*this);
    }

    CRect & operator=(const CRect & rc)
    {
        left = rc.left;
        top = rc.top;
        right = rc.right;
        bottom = rc.bottom;
        return (*this);
    }

    bool isIn(POINT *pt)
    {
        return isIn(pt->x,pt->y);
    }

    bool isIn(int x,int y)
    {
        if (x>=left && x<=right && y>=top && y<=bottom)
        {
            return true;
        }
        else return false;
    }
    bool isIntersect(RECT * rc)
    {
        if (right<rc->left  || left>rc->right 
            || bottom<rc->top || top>rc->bottom)
        {
            return false;
        }
        else return true;
    }

    inline int width(void){ return right-left; }
    inline int height(void){ return bottom-top; }
};

class CPoint : public POINT
{
public:
    CPoint() 
    {
        x = y = 0;
    };

    CPoint(const CPoint & pt)
    {
        x = pt.x;
        y = pt.y;
    }
    CPoint(POINT * pt) 
    {
        x = pt->x;
        y = pt->y;
    }
    CPoint(int x_ , int y_ )
    {
        x = x_ ;
        y = y_ ;
    }

    CPoint & operator=(const CPoint & pt)
    {
        x = pt.x;
        y = pt.y;
        return (*this);
    }
    CPoint & operator=(const POINT & pt)
    {
        x = pt.x;
        y = pt.y;
        return (*this);
    }
    bool operator==(const CPoint & pt)
    {
        return (x==pt.x && y==pt.y);
    }
    bool operator==(const POINT & pt)
    {
        return (x==pt.x && y==pt.y);
    }
    
};

class CSize : public SIZE
{
public:
    CSize(void)
    {
        cx = cy = 0;
    }
    CSize(int w,int h)
    {
        cx = w;
        cy = h;
    }
    CSize(const CSize & sz)
    {
        cx = sz.cx;
        cy = sz.cy;
    }
    CSize(CSize * psz)
    {
        cx = psz->cx;
        cy = psz->cy;
    }

    CSize & operator=(const CSize & sz)
    {
        cx = sz.cx;
        cy = sz.cy;
    }

    CSize & operator=(const SIZE & sz)
    {
        cx = sz.cx;
        cy = sz.cy;
    }

    bool operator==(const CSize & sz)
    {
        return (cx==sz.cx && cy==sz.cy);
    }

    bool operator==(const SIZE & sz)
    {
       return (cx==sz.cx && cy==sz.cy);
    }
};

//智能指针管理的基类
struct IBaseObject
{
public:
    IBaseObject(void)
    {
        m_nRefCount_i = 0;
        m_bManaged_i = true;
    }

    virtual ~IBaseObject(void) { }

    inline void  ref(void) { ++m_nRefCount_i; }

    inline void  unref(void) 
    {
        --m_nRefCount_i;
        if (0 == m_nRefCount_i && m_bManaged_i) 
        {
            delete this;
        }
    }
    inline bool isManaged(void){ return m_bManaged_i; }
    inline bool setManaged(bool manage){ m_bManaged_i = manage; }

private:
    int     m_nRefCount_i;
    bool    m_bManaged_i;
};


template<typename T>
class RefPtr 
{
public:
    RefPtr(T* ptr = 0)
    {
        m_ptr = 0;
        *this = ptr;
    }

    RefPtr(const RefPtr<T>& ptr) 
    {
        m_ptr = 0;
        *this = ptr;
    }

    ~RefPtr(void) 
    {
        if (m_ptr) 
        {
            m_ptr->unref();
            m_ptr = 0;
        }
    }

    RefPtr<T>& operator=(T* ptr) 
    {
        if (ptr != m_ptr) 
        {
            if (m_ptr) 
            {
                m_ptr->unref();
            }
            m_ptr = ptr;
            if (m_ptr) 
            {
                m_ptr->ref();
            }
        }
        return *this;
    }

    RefPtr<T>& operator=(const RefPtr<T>& ptr) 
    {
        *this = ptr.m_ptr;
        return *this;
    }

    T* operator->() const 
    {
        return m_ptr;
    }

    T& operator*() const 
    {
        return *m_ptr;
    }

    operator bool() const 
    {
        return (m_ptr != 0);
    }

    T* get() const 
    {
        return m_ptr;
    }

private:
    T* m_ptr;
};


template<typename T, typename U>
bool operator==(const RefPtr<T>& a, const RefPtr<U>& b) 
{
    return (a.get() == b.get());
}

template<typename T>
bool operator==(const RefPtr<T>& a, const T* b)
{
    return (a.get() == b);
}

template<typename T>
bool operator==(const T* a, const RefPtr<T>& b) 
{
    return (a == b.get());
}


template<typename T, typename U>
bool operator!=(const RefPtr<T>& a, const RefPtr<U>& b) 
{
    return (a.get() != b.get());
}

template<typename T>
bool operator!=(const RefPtr<T>& a, const T* b) 
{
    return (a.get() != b);
}

template<typename T>
bool operator!=(const T* a, const RefPtr<T>& b)
{
    return (a != b.get());
}

