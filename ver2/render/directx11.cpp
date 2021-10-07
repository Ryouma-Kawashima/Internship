#include "../utility/static_polymorphism.h"
#include "../utility/parameter.h"
#include "../system/config.h"

#include "../window/window_api.h"

#include "directx11.h"

using namespace DragonLib;
using namespace DirectX;

void DirectX11::Initialize(WindowAPI* window)
{
    HRESULT hr = S_OK;

    UINT flag = 0;
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    #if defined(__DEBUG) || defined(_DEBUG)
    flag = D3D11_CREATE_DEVICE_DEBUG;
    #endif

    // デバイス、スワップチェーンの作成
    DXGI_SWAP_CHAIN_DESC swapChainDesc{};
    swapChainDesc.BufferCount                           = 1;
    swapChainDesc.BufferDesc.Width                      = static_cast<unsigned int>( window->GetWidth() );
    swapChainDesc.BufferDesc.Height                     = static_cast<unsigned int>( window->GetHeight() );
    swapChainDesc.BufferDesc.Format                     = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator      = FPS;
    swapChainDesc.BufferDesc.RefreshRate.Denominator    = 1;
    swapChainDesc.BufferDesc.ScanlineOrdering           = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling                    = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.BufferUsage                           = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow                          = reinterpret_cast<HWND>( window->GetHandle() );
    swapChainDesc.SampleDesc.Count                      = 1;
    swapChainDesc.SampleDesc.Quality                    = 0;
    swapChainDesc.SwapEffect                            = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Windowed                              = true;
    swapChainDesc.Flags                                 = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        flag,
        featureLevels,
        2,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        m_SwapChain.GetAddressOf(),
        m_Device.GetAddressOf(),
        &m_FeatureLevel,
        m_DeviceContext.GetAddressOf());







    m_Window = window;
}
void DirectX11::Finalize()
{

}

void DirectX11::Begin()
{

}
void DirectX11::End()
{

}
