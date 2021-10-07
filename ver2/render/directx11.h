#pragma once
// ------------------------------
// Author: Ryouma Kawashima
// Create: 2021/10/07
// Update: 2021/10/07
// ------------------------------
// Description
// 
// ------------------------------

#include <wrl/client.h>
#include <d3d11.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

// 自作したら消す
#include <DirectXMath.h>

#pragma comment (lib,"d3d11.lib")
#pragma comment (lib,"dxgi.lib")
#pragma comment (lib,"D3DCompiler.lib")

namespace DragonLib
{
    class DirectX11
    {
    public:
        void Initialize(WindowAPI* window);
        void Finalize();

        void Begin();
        void End();

    private:
        WindowAPI* m_Window;

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
