#ifndef _GS_AUTO_LOCK_H
#define _GS_AUTO_LOCK_H
/************************************************************************/
/* �ò��ֽ�Ϊwindows�汾                                                */
/************************************************************************/
#include <windows.h>  
#include <WinBase.h>  

class GSMutex  
{  
public:  
    GSMutex(void) { InitializeCriticalSection(&m_hOS); }
    ~GSMutex(void) { DeleteCriticalSection(&m_hOS); }
	
	//����
    void Lock() { EnterCriticalSection(&m_hOS); }

	//����
    void Unlock() { LeaveCriticalSection(&m_hOS); }
    
    //true��ȡ���ɹ���falseʧ�� 
    bool TryLock()       
	{
		if (!TryEnterCriticalSection(&m_hOS))  
		{  
			return false;  
		}  
		
		return true;
	}
	
private:  
    CRITICAL_SECTION m_hOS;  
};  


class GSAutoLock  
{  
public:  
    GSAutoLock(GSMutex& mutex):m_mutex(mutex) {m_mutex.Lock();} 
    ~GSAutoLock(void) {m_mutex.Unlock();}  
	
private :  
    GSMutex& m_mutex;  
};  

#endif //_GS_AUTO_LOCK_H