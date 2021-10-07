#include "../utility/param.h"
#include "../system/config.h"
#include "window.h"
#include <cassert>

using namespace DragonLib;

void Window::Initialize(float width, float height, TCHAR* windowName)
{
	m_hInstance = GetModuleHandle(NULL);
	assert(m_hInstance);

	if (windowName == nullptr)
	{
		windowName = const_cast<TCHAR*>(WINDOW_NAME);
	}

	m_WindowClass =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		m_hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};

	RegisterClassEx(&m_WindowClass);

	m_hWnd = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(static_cast<int>(width)  + GetSystemMetrics(SM_CXDLGFRAME) * 2),
		(static_cast<int>(height) + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)),
		NULL,
		NULL,
		m_hInstance,
		NULL);
	assert(m_hWnd);

	m_Width		= width;
	m_Height	= height;
}

void Window::Finalize()
{
	UnregisterClass(CLASS_NAME, m_WindowClass.hInstance);
}

void Window::SetWindowSize(float width, float height)
{
	UNREFERENCED_PARAMETER(width);
	UNREFERENCED_PARAMETER(height);
}

void Window::SetWindowPosition(float width, float height)
{
	UNREFERENCED_PARAMETER(width);
	UNREFERENCED_PARAMETER(height);
}

void Window::SetWindowShow(int32_t cmdShow)
{
	ShowWindow(m_hWnd, cmdShow);
	UpdateWindow(m_hWnd);
}

void* Window::GetHandle()
{
	return m_hWnd;
}
float Window::GetWidth()
{
	return m_Width;
}
float Window::GetHeight()
{
	return m_Height;
}

#if defined(_WIN64)
LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
#else
HRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
#endif
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hWnd);
		}
		break;

	default:
		break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
