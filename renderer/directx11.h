#pragma once

#include <wrl/client.h>
#include <d3d11.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

// é©çÏÇµÇΩÇÁè¡Ç∑
#include <DirectXMath.h>

#pragma comment (lib,"d3d11.lib")
#pragma comment (lib,"dxgi.lib")
#pragma comment (lib,"D3DCompiler.lib")

namespace DragonLib
{
    

    struct Vertex3D
    {
        DirectX::XMFLOAT3 Position;
        DirectX::XMFLOAT3 Normal;
        DirectX::XMFLOAT3 Diffuse;
        DirectX::XMFLOAT3 TexCoord;
    };



    struct Material
    {
        DirectX::XMFLOAT4 Ambient;
        DirectX::XMFLOAT4 Diffuse;
        DirectX::XMFLOAT4 Speculaar;
        DirectX::XMFLOAT4 Emission;
        float		Shininess;
        float		Dummy[3];
    };



    struct Light
    {
        bool		Enable;
        bool		Dummy[3];
        DirectX::XMVECTOR Direction;
        DirectX::XMFLOAT4 Diffuse;
        DirectX::XMFLOAT4 Ambient;
    };



    class DirectX11 : public RenderBase
    {
    public:
        void Initialize(Window* window);
        void Finalize();

        void Begin(float r, float g, float b);
        void End();

        void SetDepthEnable(bool enable);
        void SetWorldViewProjection2D();
        void SetWorldMatrix(DirectX::XMMATRIX* worldMatrix);
        void SetViewMatrix(DirectX::XMMATRIX* viewMatrix);
        void SetProjectionMatrix(DirectX::XMMATRIX* projectionMatrix);

        void SetMaterial(Material material);
        void SetLight(Light light);

        void SetCoordSystem(uint32_t cmd);

        void CreateVertexShader(ID3D11VertexShader** vertexShader, ID3D11InputLayout** vertexLayout, const char* fileName);
        void CreatePixelShader(ID3D11PixelShader** pixelShader, const char* fileName);

    private:
        Window* m_Window;
        uint32_t m_CoordSystem = CoordSystem_Left;

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

