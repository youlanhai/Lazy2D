//mlist.h
#pragma once


#ifndef LISTCOMPARE
#define LISTCOMPARE
//元素比较
template<typename Type>
struct ListCompare
{
    inline bool operator ()(const Type & t1, const Type & t2)
    {
        if(t1 == t2)
        {
            return true;
        }
    }
};
#endif

//链表结点
template<typename Type>
struct ListNode
{
    Type data;
    struct ListNode *next;
    ListNode(void):next(NULL)
    {}
};

//链表类。单向链表。
template<typename Type>
class CMList
{
public:
    typedef typename ListNode<Type> ListNode;
    typedef ListNode *LinkNode;
public:

    CMList() : m_nPoolSize(0) { }

	~CMList(){ destroy(); }

    //链表是否为空
    inline bool empty(void){ return (end() == begin()); }

    //统计链表元素个数
    int size( void )
    {
        int count=0;
        for(LinkNode p = begin(); p != end(); p=p->next)
        {
            ++count;
        }
        return count;
    }

    //向链头添加元素
	void push_front(const Type & t)
    {
        LinkNode p=newNode(t);
        push_front(p);
    }
    //向链头添加元素
    inline void push_front(LinkNode p)
    {
        if(NULL != p)
        {
            p->next = m_head.next;
            m_head.next = p;
        }
    }

	LinkNode find(Type t,ListCompare<Type> cmp=ListCompare<Type>())
    {
        for(LinkNode p=begin(); p != end(); p=p->next)
        {
            if ( cmp(p->data, t))
            {
                return p;
            }
        }
        return NULL;
    }

    //清空链表。仅将结点移到内存池。
    void clear()
    {
        for(LinkNode p = begin(); p != end(); )
        {
            p=deleteNode(p);
        }
        m_head.next=NULL;
    }  

    //销毁所有资源，包括内存池
    void destroy()
    {
        clear();
        //!!!不可使用成员函数!
        LinkNode p = m_memoryPool.next;
        LinkNode pTemp = NULL;
        while (p != NULL)
        {
            pTemp = p;
            p = p->next;
            delete pTemp;
        }
    }

    //分配新节点
    LinkNode newNode(void)
    {
        //!!!不可使用成员函数!
        if(NULL == m_memoryPool.next)
        {
            return new ListNode();
        }
        else
        {
            LinkNode p = m_memoryPool.next;
            m_memoryPool.next = p->next;
            p->next = NULL;
            --m_nPoolSize;
            return p;
        }
    }
    //分配新节点
    LinkNode newNode(const Type & t)
    {
        LinkNode p=newNode();
        p->data = t;
        return p;
    }

    //删除结点。返回pNode的后继结点地址。
    LinkNode deleteNode(ListNode *pNode)
    {
        if (pNode != NULL)
        {
            LinkNode p = pNode->next;

            pNode->next = m_memoryPool.next;
            m_memoryPool.next = pNode;

            ++m_nPoolSize;
            return p;
        }
        else
        {
            return NULL;
        }
    }

    //预留空间。本质上是增加内存池容量。
    void reserve(int size)
    {
        for(int i=m_nPoolSize; i < size; ++i)
        {
            deleteNode(new ListNode());
        }
    }

    inline int sizePool( void ){ return m_nPoolSize; }
  
    inline LinkNode head(void){ return &m_head; }
    inline LinkNode end(void){ return NULL; }
    inline LinkNode begin(void){ return m_head.next; }

private:
	ListNode	m_head;		    //头结点
	ListNode	m_memoryPool;   //内存池
    int         m_nPoolSize;    //内存池元素个数
};
