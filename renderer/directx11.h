#pragma once

#include <wrl/client.h>
#include <d3d11.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

#pragma comment (lib,"d3d11.lib")
#pragma comment (lib,"dxgi.lib")
#pragma comment (lib,"D3DCompiler.lib")

namespace DragonLib
{
    class DirectX11 : public RenderBase
    {
    public:
        void Initialize(Window& window);
        void Finalize();

        void Begin();
        void End();

        void ClearScreen(uint32_t r, uint32_t g, uint32_t b);

    private:
        D3D_FEATURE_LEVEL m_FeatureLevel = D3D_FEATURE_LEVEL_11_1;

        Microsoft::WRL::ComPtr<ID3D11Device>            m_Device            = nullptr;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_DeviceContext     = nullptr;
        Microsoft::WRL::ComPtr<IDXGISwapChain>          m_SwapChain         = nullptr;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_RenderTargetView  = nullptr;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_DepthStencilView  = nullptr;                                     
        Microsoft::WRL::ComPtr<ID3D11Buffer>            m_WorldBuffer       = nullptr;
        Microsoft::WRL::ComPtr<ID3D11Buffer>            m_ViewBuffer        = nullptr;
        Microsoft::WRL::ComPtr<ID3D11Buffer>            m_ProjectionBuffer  = nullptr;
        Microsoft::WRL::ComPtr<ID3D11Buffer>            m_MaterialBuffer    = nullptr;
        Microsoft::WRL::ComPtr<ID3D11Buffer>            m_LightBuffer       = nullptr;                  
        Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DepthStateEnable  = nullptr;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DepthStateDisable = nullptr;
    };
}

