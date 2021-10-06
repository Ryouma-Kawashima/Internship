#pragma once

#include <windows.h>
#include <wrl.h>
#pragma comment(lib, "winmm.lib")

namespace DragonLib
{
    class Window
    {
    public:
        void Initialize(uint32_t width, uint32_t height, TCHAR* windowName = nullptr);
        void Finalize();
        void SetWindowSize(uint32_t width, uint32_t height);
        void SetWindowPosition(uint32_t width, uint32_t height);
        void SetWindowShow(int32_t cmdShow);
        HWND GetHandle();
        uint32_t GetWidth();
        uint32_t GetHeight();

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
        uint32_t    m_Width         = 0;
        uint32_t    m_Height        = 0;
    };
}
