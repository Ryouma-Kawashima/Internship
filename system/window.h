#pragma once

#include <windows.h>
#pragma comment(lib, "winmm.lib")

namespace DragonLib
{
    class Window
    {
    public:
        Window() noexcept = delete;
        Window(uint32_t width, uint32_t height, TCHAR* windowName = nullptr);
        ~Window() noexcept;

    public:
        void SetWindowSize(uint32_t width, uint32_t height);
        void SetWindowPosition(uint32_t width, uint32_t height);
        void SetWindowShow(int32_t cmdShow);
        static HRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    private:
        HWND        m_hWnd          = nullptr;
        HINSTANCE   m_hInstance     = nullptr;
        WNDCLASSEX  m_WindowClass   = {};
        WNDPROC     m_WndProc       = WindowProc;
    };
}
