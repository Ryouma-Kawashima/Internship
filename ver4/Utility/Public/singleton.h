#pragma once

namespace DragonLib
{
    template <class T>
    class Singleton
    {
    public:
        static void Create()
        {
            if (m_Instance == nullptr)
            {
                m_Instance = new T;
            }
        }
        static void Destroy()
        {
            if (m_Instance != nullptr)
            {
                delete m_Instance;
                m_Instance = nullptr;
            }
        }
        static T* GetInstance()
        {
            return m_Instance;
        }

    private:
        static T* m_Instance;
    };

    template <class T>
    T* Singleton<T>::m_Instance = nullptr;
}
