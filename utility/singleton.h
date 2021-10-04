#pragma once

namespace DragonLib
{
    template <class T>
    class Singleton
    {
    public:
        void Create()
        {
            if (Instance == nullptr)
            {
                Instance = new T;
            }
        }
        void Destroy()
        {
            if (Instance != nullptr)
            {
                delete Instance;
                Instance = nullptr;
            }
        }
        T& GetInstance() const
        {
            return Instance;
        }

    private:
        static T* Instance;
    };

    template <class T>
    T* Singleton<T>::Instance = nullptr;
}
