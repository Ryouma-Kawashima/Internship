#include "../utility/param.h"
#include "../system/config.h"
#include "window.h"
#include <cassert>

using namespace DragonLib;

void Window::Initialize(uint32_t width, uint32_t height, TCHAR* windowName)
{
	//m_hInstance = GetModuleHandle(NULL);
	//assert(m_hInstance);

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
		(width + GetSystemMetrics(SM_CXDLGFRAME) * 2),
		(height + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)),
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

void Window::SetWindowSize(uint32_t width, uint32_t height)
{
	UNREFERENCED_PARAMETER(width);
	UNREFERENCED_PARAMETER(height);
}

void Window::SetWindowPosition(uint32_t width, uint32_t height)
{
	UNREFERENCED_PARAMETER(width);
	UNREFERENCED_PARAMETER(height);
}

void Window::SetWindowShow(int32_t cmdShow)
{
	ShowWindow(m_hWnd, cmdShow);
	UpdateWindow(m_hWnd);
}

HWND Window::GetHandle()
{
	return m_hWnd;
}
uint32_t Window::GetWidth()
{
	return m_Width;
}
uint32_t Window::GetHeight()
{
	return m_Height;
}

HRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
