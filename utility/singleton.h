#pragma once

namespace DragonLib
{
    template <class T>
    class Singleton
    {
    public:
        void Create()
        {
            if (m_Instance == nullptr)
            {
                m_Instance = new T;
            }
        }
        void Destroy()
        {
            if (m_Instance != nullptr)
            {
                delete m_Instance;
                m_Instance = nullptr;
            }
        }
        T* GetInstance()
        {
            return m_Instance;
        }

    private:
        T* m_Instance;
    };
}
