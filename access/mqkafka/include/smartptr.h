/*====================================================================
模块名 ：
文件名 ：smartptr.h
相关文件：smartptr.cpp
实现功能：指针计数器的实现，即简易版的智能指针
作者 ：刘春光
版权 ：<Copyright(C) 2003-2019 Suzhou Keda Technology Co., Ltd. All rights reserved.>
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2019/03/04      V1.0         刘春光
====================================================================*/
#ifndef _SMART_PTR_H_
#define _SMART_PTR_H_

template <class S> \
class SmartPtr;

template <class T> \
class Smart_U_Ptr 
{
private:
    friend class SmartPtr<T>;
    //比较==运算符的友元声明
    template <class S>
    friend bool operator==(const void* pointer1, const SmartPtr<S> &sp);

    Smart_U_Ptr(T *p)
        :m_ip(p)
        ,m_useCount(1)
    {
    }
    ~Smart_U_Ptr()
    {
        if (NULL != m_ip)
        {
            delete m_ip;
            m_ip = NULL;
        }
    }
    T *m_ip;
    int m_useCount;
};
// template <class T>
// bool operator==(const void* const pointer1, const SmartPtr<T> const &sp);

template <class T>
class SmartPtr 
{
public:
    SmartPtr(T *p)
        :m_uptr(new Smart_U_Ptr<T>(p))
    {
    }
    SmartPtr(const SmartPtr &ptr)
        :m_uptr(ptr.m_uptr)
    {
        ++m_uptr->m_useCount;
    }
    SmartPtr& operator=(const SmartPtr &rhs)
    {
        bool uptrIsSame = this->m_uptr == rhs.m_uptr;
        if (!uptrIsSame) {
            ++rhs.m_uptr->m_useCount;
            if (--m_uptr->m_useCount == 0) {
                delete m_uptr;
                m_uptr = NULL;
            }
            m_uptr = rhs.m_uptr;
        }
        return *this;
    }
    T & operator *()        //重载*操作符
    {
        return *(m_uptr->m_ip);
    }
    T* operator ->()        //重载->操作符
    {
        return m_uptr->m_ip;
    }
    bool operator==(const SmartPtr<T> &sp) //重载==操作符
    {
        return m_uptr->m_ip == sp.m_uptr->m_ip ? true : false;
    }
    bool operator==(const void * &pointer1) //重载==操作符
    {
        return m_uptr->m_ip == pointer1 ? true : false;
    }
    ~SmartPtr()
    {
        if (--m_uptr->m_useCount == 0) 
        {
            delete m_uptr;
            m_uptr = NULL;
        }
    }
    //比较==运算符的友元声明
    template <class S>
    friend bool operator==(const void* pointer1, const SmartPtr<S> &sp);
private:
    Smart_U_Ptr<T> *m_uptr;
};
//实现相等比较操作符==的重载，以和空指针进行判断
template <class T>
bool operator==(const void* pointer1, const SmartPtr<T> &sp)
{
    return sp.m_uptr->m_ip == pointer1 ? true : false;
}

#endif //_SMART_PTR_H_

