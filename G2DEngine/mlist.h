//mlist.h
#pragma once


#ifndef LISTCOMPARE
#define LISTCOMPARE
//Ԫ�رȽ�
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

//������
template<typename Type>
struct ListNode
{
    Type data;
    struct ListNode *next;
    ListNode(void):next(NULL)
    {}
};

//�����ࡣ��������
template<typename Type>
class CMList
{
public:
    typedef typename ListNode<Type> ListNode;
    typedef ListNode *LinkNode;
public:

    CMList() : m_nPoolSize(0) { }

	~CMList(){ destroy(); }

    //�����Ƿ�Ϊ��
    inline bool empty(void){ return (end() == begin()); }

    //ͳ������Ԫ�ظ���
    int size( void )
    {
        int count=0;
        for(LinkNode p = begin(); p != end(); p=p->next)
        {
            ++count;
        }
        return count;
    }

    //����ͷ���Ԫ��
	void push_front(const Type & t)
    {
        LinkNode p=newNode(t);
        push_front(p);
    }
    //����ͷ���Ԫ��
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

    //���������������Ƶ��ڴ�ء�
    void clear()
    {
        for(LinkNode p = begin(); p != end(); )
        {
            p=deleteNode(p);
        }
        m_head.next=NULL;
    }  

    //����������Դ�������ڴ��
    void destroy()
    {
        clear();
        //!!!����ʹ�ó�Ա����!
        LinkNode p = m_memoryPool.next;
        LinkNode pTemp = NULL;
        while (p != NULL)
        {
            pTemp = p;
            p = p->next;
            delete pTemp;
        }
    }

    //�����½ڵ�
    LinkNode newNode(void)
    {
        //!!!����ʹ�ó�Ա����!
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
    //�����½ڵ�
    LinkNode newNode(const Type & t)
    {
        LinkNode p=newNode();
        p->data = t;
        return p;
    }

    //ɾ����㡣����pNode�ĺ�̽���ַ��
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

    //Ԥ���ռ䡣�������������ڴ��������
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
	ListNode	m_head;		    //ͷ���
	ListNode	m_memoryPool;   //�ڴ��
    int         m_nPoolSize;    //�ڴ��Ԫ�ظ���
};
