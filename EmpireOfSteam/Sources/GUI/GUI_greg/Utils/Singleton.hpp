#ifndef SINGLETON_HPP
#define SINGLETON_HPP

template<typename T> class CSingleton
{
    public:
    /*
      Cette fonction retourne l'unique instance de l'objet
    */
        static T* GetInstance(void)
        {
            if(singleton==0)
            {
            singleton = new T;
            }
            return singleton;
        }
    /*
      Cette fonction tue l'unique instance de l'objet
    */
        static void Kill()
        {
            if(singleton!=0)
            {
            delete singleton ;
            singleton=0;
            }
        }

    protected:
    CSingleton(){}
    virtual ~CSingleton(){}
    static T *singleton;
};

template <typename T> T* CSingleton<T>::singleton = 0;

#endif
