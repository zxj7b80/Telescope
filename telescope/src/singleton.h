//
// C++ Interface: singleton
//
// Description:
//

//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <memory> //for auto_ptr
//#include <tr1/cstdint> //for int32_t,if you want to use cstdint directly,you must add -std=c++0x as a option for gcc.
template <class T>
class Singleton
{
public:
    static inline T& instance();
    static inline T* instance_p();
    static inline void release();
private:
    Singleton ( void ) {}
    virtual ~Singleton ( void ) {qDebug("singleton.");}//防止中途意外被删除
    Singleton ( const Singleton& );
    Singleton & operator= ( const Singleton & );

    static  std::auto_ptr<T> _instance;//一定要使用auto_ptr, 因为普通指针在这里不会被释放内存, 只有程序结束后回收给系统, 但如果这个类持有一些资源, 如网络连接等, 到时就麻烦了.因为智能指针对象会在程序结束的时析构，也就是释放他所持有的所有资源，包括内存和网络链接及数据库等。而
};

template <class T>
std::auto_ptr<T> Singleton<T>::_instance;

template <class T>
inline T& Singleton<T>::instance()
{
    // 	static T instance;
    // 	return instance;
    if( 0== _instance.get())
    {	qDebug("new instance");
        _instance.reset ( new T);
    }
    return *(_instance.get());
}

template <class T>
inline T* Singleton<T>::instance_p()
{   //qDebug("newbg");
    if( 0== _instance.get())
    {	qDebug("new instance_p");
        _instance.reset ( new T);
    }
    return _instance.get();
}

template <class T>
inline void Singleton<T>::release()
{
    _instance.release();//release the pointer to the object.When you use this ,you have to delete some resource like net connection or DB connection explicitly by your hand.
    //	_instance->release();//release the object itself pointed.RAII will do everything for you.(need to check later)
}
//Class that will implement the singleton mode,
//must use the macro in it's delare file

#define DECLARE_SINGLETON_CLASS( type ) \
    friend class std::auto_ptr< type >;\
    friend class Singleton< type >;

