// ------------------------------
// Author: Ryouma Kawashima
// Create: 2021/10/07
// Update: 2021/10/07
// ------------------------------
// Description
// 
// ------------------------------

#include "windows.h"
#include "../system/config.h"
#include <cassert>

using namespace DragonLib;

void Windows::Initialize(float width, float height)
{
	HINSTANCE hInstance = GetModuleHandle(nullptr);
	assert(hInstance);

	WNDCLASSEX windowClass =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};

	RegisterClassEx(&windowClass);

	m_WindowHandle = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_CAPTION,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(static_cast<int>(width)  + GetSystemMetrics(SM_CXDLGFRAME) * 2),
		(static_cast<int>(height) + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)),
		NULL,
		NULL,
		hInstance,
		NULL);
	assert(m_WindowHandle);
}
void Windows::Finalize()
{
	UnregisterClass(CLASS_NAME, m_InstanceHandle);
}

void Windows::Show(int cmd)
{
	ShowWindow(m_WindowHandle, cmd);
}
void Windows::Update()
{
	UpdateWindow(m_WindowHandle);
}

void* Windows::GetHandle()
{
	return m_WindowHandle;
}
float Windows::GetWidth()
{
	return m_Width;
}
float Windows::GetHeight()
{
	return m_Height;
}

#if defined(_WIN64)
LRESULT CALLBACK Windows::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
#else
HRESULT CALLBACK Windows::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
