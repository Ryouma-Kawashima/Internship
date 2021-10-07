#pragma once
// ------------------------------
// Author: Ryouma Kawashima
// Create: 2021/10/07
// Update: 2021/10/07
// ------------------------------
// Description
// 
// ------------------------------

#if USE_WINDOWS
#include "windows.h"
#else

#endif

namespace DragonLib
{
    template
    <
        class T,
        bool IsBased = std::is_base_of_v<WindowBase, T>
    >
    class Window
    {
    public:
        void Initialize(float width, float height)
        {
            m_Window.Initialize(width, height);
        }
        void Finalize()
        {
            m_Window.Finalize();
        }

        void Show(int cmd)
        {
            m_Window.Show(cmd);
        }
        void Update()
        {
            m_Window.Update();
        }

        void* GetHandle()
        {
            m_Window.GetHandle();
        }
        float GetWidth()
        {
            m_Window.GetWidth();
        }
        float GetHeight()
        {
            m_Window.GetHeight();
        }

    private:
        T m_Window;
    };

    #if USE_WINDOWS
    typedef Window<Windows> WindowAPI;
    #endif
}