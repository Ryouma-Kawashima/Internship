#pragma once
// ------------------------------
// Author: Ryouma Kawashima
// Create: 2021/10/07
// Update: 2021/10/07
// ------------------------------
// Description
// 
// ------------------------------

#include <windows.h>
#pragma comment(lib, "winmm.lib")

#include "window_base.h"

namespace DragonLib
{
    class Windows : public WindowBase
    {
    public:
        void Initialize(float width, float height);
        void Finalize();

        void Show(int cmd);
        void Update();

        void* GetHandle();
        float GetWidth();
        float GetHeight();

        #if defined(_WIN64)
        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        #else
        static HRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        #endif

    private:
        float m_Width;
        float m_Height;

        HWND m_WindowHandle;
        HINSTANCE m_InstanceHandle;
    };
}
