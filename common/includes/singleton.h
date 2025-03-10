// file : singleton.h
// name : eric garcia

#ifndef SINGLETON_H
#define SINGLETON_H

/*
  What is the purpose of this class?
  Google CRTP (Curiously reacurring template pattern).
  TLDR:
  We can create a templated base class that other classes can inherit
  from. This will give those inherited classes methods defined in
  the templated class.
  So in this case, we have a base class singleton, so that other
  classes can inherit the singleton class design pattern. You might
  think that this isn't necessary, but the amount of space saved by
  only writing out getInstance and destroy instance is MASSIVE when
  we have a TON of singleton classes. Also handy because
  we don't have to initialize the static instance member for every class,
  rather we can just do it once here.
*/

template <typename T>
class Singleton
{
public:
    Singleton(const Singleton&) = delete;
    Singleton & operator=(const Singleton&) = delete;
    
    static T * getInstance()
    {
        if (instance == NULL)
            instance = new T();
        return instance;
    }
    static void destroyInstance()
    {
        if (instance != NULL)
        {
            delete instance;
            instance = NULL;
        }
    }
protected:
    Singleton() = default;
    virtual ~Singleton() = default;
    
    static T * instance;
};

template<typename T>
T * Singleton<T>::instance(NULL);


#endif 
