#pragma once

#include <windows.h>
#include <wrl.h>
#pragma comment(lib, "winmm.lib")

namespace DragonLib
{
    class Window
    {
    public:
        void Initialize(float width, float height, TCHAR* windowName = nullptr);
        void Finalize();
        void SetWindowSize(float width, float height);
        void SetWindowPosition(float width, float height);
        void SetWindowShow(int32_t cmdShow);
        void* GetHandle();
        float GetWidth();
        float GetHeight();

        #if defined(_WIN64)
        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        #else
        static HRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        #endif

    private:
        HWND        m_hWnd          = nullptr;
        HINSTANCE   m_hInstance     = nullptr;
        WNDCLASSEX  m_WindowClass   = {};
        WNDPROC     m_WndProc       = WindowProc;
        float       m_Width         = 0;
        float       m_Height        = 0;
    };
}
