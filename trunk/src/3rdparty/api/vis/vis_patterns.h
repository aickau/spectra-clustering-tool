#ifndef _VIS_PATTERNS_H
#define _VIS_PATTERNS_H

#include <assert.h>

/** Singleton pattern for smoother code. see: Design Patterns, Erich Gamma et al.
 */
template<typename T>
class Singleton 
{
public : 
    static T& Instance();
    static void Destroy();

protected : 
    inline explicit Singleton()
	{ 
		assert(Singleton::instance_ == 0); 
		Singleton::instance_ = static_cast<T*>(this); 
	}

    inline ~Singleton()
	{ 
		Singleton::instance_ = 0; 
	}

private : 
    static T*		CreateInstance();
    static void		ScheduleForDestruction(void (*)());
    static void		DestroyInstance(T*);

    inline explicit Singleton(Singleton const&) {}
    inline			Singleton& operator=(Singleton const&) { return *this; }

    static T* instance_;
};   

/** all classes get global access via Classname::Instance().ProcedureName 
 * Instance returns a valid this pointer of the corresponding object.
 * before calling Classname::Instance() make sure to create one object  
 */
template<typename T> 
typename T& Singleton<T>::Instance() 
{
    if ( Singleton::instance_ == 0 ) 
	{
        Singleton::instance_ = CreateInstance();
        ScheduleForDestruction(Singleton::Destroy);
    }
    return *(Singleton::instance_);
}

template<typename T>
void Singleton<T>::Destroy() 
{
    if ( Singleton::instance_ != 0 ) 
	{
        DestroyInstance(Singleton::instance_);
        Singleton::instance_ = 0;
    }
}

template<typename T>
inline typename T* Singleton<T>::CreateInstance() 
{
    return new T();
}

template<typename T>
inline void Singleton<T>::ScheduleForDestruction(void (*pFun)()) 
{
	atexit(pFun);
}

template<typename T>
inline void Singleton<T>::DestroyInstance(T* p) 
{
    delete p;
}

template<typename T>
typename T* Singleton<T>::instance_ = 0;

#endif