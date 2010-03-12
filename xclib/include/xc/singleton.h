
#ifndef _ITISSIEN_SINGLETON_H_
#define _ITISSIEN_SINGLETON_H_

namespace xc {

// class singleton has the same goal as all singletons: create one instance of
// a class on demand, then dish it out as requested.

/** Singleton
*/
template <class T>
class singleton : private T
{
private:
    singleton();
    ~singleton();

public:
    static T &instance();
    static bool inited(bool init=false);
};

template <class T>
inline singleton<T>::singleton()
{
    singleton<T>::inited(true);
}

template <class T>
inline singleton<T>::~singleton()
{
    /* no-op */
}

template <class T>
/*static*/ T &singleton<T>::instance()
{
    // function-local static to force this to work correctly at static
    // initialization time.
    static bool ininit = false;
    if (ininit && !singleton<T>::inited())
    {
        //printf("CYCLED %s!!!!!!!\n", __PRETTY_FUNCTION__);
        throw 1; //toso
    }
    ininit = true;
    static singleton<T> s_oT;
    ininit = false;
    return(s_oT);
}

template <class T>
/*static*/ bool singleton<T>::inited(bool init)
{
    static bool s_inited = false;
    if (init)
        s_inited = true;
    return s_inited;
}


} // namespace isn

#endif // _ITISSIEN_SINGLETON_H_

