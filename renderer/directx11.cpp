
#include "../utility//param.h"
#include "../system/config.h"
#include "../system/window.h"

#include "../renderer/renderer.h"

#include "directx11.h"

#include <cassert>

using namespace DragonLib;

void DirectX11::Initialize(Window& window)
{
    HRESULT hr = S_OK;

    // デバイス、スワップチェーンの作成
    DXGI_SWAP_CHAIN_DESC swapChainDesc{};
    swapChainDesc.BufferCount                           = 1;
    swapChainDesc.BufferDesc.Width                      = static_cast<UINT>( window.GetWidth() );
    swapChainDesc.BufferDesc.Height                     = static_cast<UINT>( window.GetHeight() );
    swapChainDesc.BufferDesc.Format                     = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator      = FPS;
    swapChainDesc.BufferDesc.RefreshRate.Denominator    = 1;
    swapChainDesc.BufferDesc.ScanlineOrdering           = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling                    = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.BufferUsage                           = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow                          = reinterpret_cast<HWND>( window.GetHandle() );
    swapChainDesc.SampleDesc.Count                      = 1;
    swapChainDesc.SampleDesc.Quality                    = 0;
    swapChainDesc.SwapEffect                            = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Windowed                              = true;
    swapChainDesc.Flags                                 = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    D3D_FEATURE_LEVEL featureLevels[] = 
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    D3D_DRIVER_TYPE driverType[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };


    for (int i = 0; i < 3; i++)
    {
        hr = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            0,
            featureLevels,
            2,
            D3D11_SDK_VERSION,
            &swapChainDesc,
            &m_SwapChain,
            &m_Device,
            &m_FeatureLevel,
            &m_DeviceContext);
    
        if (SUCCEEDED(hr))
        {
            break;
        }
    }
    assert(m_Device);
    assert(m_DeviceContext);
    assert(m_SwapChain);

    // レンダーターゲットビューの作成
    ID3D11Texture2D* renderTarget = nullptr;
    m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&renderTarget);
    assert(renderTarget);

    m_Device->CreateRenderTargetView(renderTarget, nullptr, &m_RenderTargetView);
    renderTarget->Release();


    // デプスステンシルバッファ作成
    ID3D11Texture2D* depthStencil = nullptr;
    D3D11_TEXTURE2D_DESC textureDesc{};
    textureDesc.Width           = swapChainDesc.BufferDesc.Width;
    textureDesc.Height          = swapChainDesc.BufferDesc.Height;
    textureDesc.MipLevels       = 1;
    textureDesc.ArraySize       = 1;
    textureDesc.Format          = DXGI_FORMAT_D16_UNORM;
    textureDesc.SampleDesc      = swapChainDesc.SampleDesc;
    textureDesc.Usage           = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags       = D3D11_BIND_DEPTH_STENCIL;
    textureDesc.CPUAccessFlags  = 0;
    textureDesc.MiscFlags       = 0;
    m_Device->CreateTexture2D(&textureDesc, nullptr, &depthStencil);
    assert(depthStencil);


    // デプスステンシルビュー作成
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
    depthStencilViewDesc.Format         = textureDesc.Format;
    depthStencilViewDesc.ViewDimension  = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Flags          = 0;
    m_Device->CreateDepthStencilView(depthStencil, &depthStencilViewDesc, &m_DepthStencilView);
    depthStencil->Release();

    m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView.Get());


    // ビューポート設定
    D3D11_VIEWPORT viewport;
    viewport.Width      = static_cast<float>( window.GetWidth() );
    viewport.Height     = static_cast<float>( window.GetHeight() );
    viewport.MinDepth   = 0.0f;
    viewport.MaxDepth   = 1.0f;
    viewport.TopLeftX   = 0;
    viewport.TopLeftY   = 0;
    m_DeviceContext->RSSetViewports(1, &viewport);


    // ラスタライザステート設定
    D3D11_RASTERIZER_DESC rasterizerDesc{};
    rasterizerDesc.FillMode             = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode             = D3D11_CULL_BACK;
    rasterizerDesc.DepthClipEnable      = TRUE;
    rasterizerDesc.MultisampleEnable    = FALSE;

    ID3D11RasterizerState* rs;
    m_Device->CreateRasterizerState(&rasterizerDesc, &rs);

    m_DeviceContext->RSSetState(rs);


    // ブレンドステート設定
    D3D11_BLEND_DESC blendDesc{};
    blendDesc.AlphaToCoverageEnable                 = FALSE;
    blendDesc.IndependentBlendEnable                = FALSE;
    blendDesc.RenderTarget[0].BlendEnable           = TRUE;
    blendDesc.RenderTarget[0].SrcBlend              = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    ID3D11BlendState* blendState = NULL;
    m_Device->CreateBlendState(&blendDesc, &blendState);
    m_DeviceContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);


    // デプスステンシルステート設定
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
    depthStencilDesc.DepthEnable    = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc      = D3D11_COMPARISON_LESS_EQUAL;
    depthStencilDesc.StencilEnable  = FALSE;

    m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateEnable);//深度有効ステート

    depthStencilDesc.DepthEnable = FALSE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateDisable);//深度無効ステート

    m_DeviceContext->OMSetDepthStencilState(m_DepthStateEnable.Get(), NULL);


    // サンプラーステート設定
    D3D11_SAMPLER_DESC samplerDesc{};
    samplerDesc.Filter      = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU    = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV    = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW    = D3D11_TEXTURE_ADDRESS_WRAP;

    ID3D11SamplerState* samplerState = NULL;
    m_Device->CreateSamplerState(&samplerDesc, &samplerState);

    m_DeviceContext->PSSetSamplers(0, 1, &samplerState);


    /*
    // 定数バッファ生成
    D3D11_BUFFER_DESC bufferDesc{};
    bufferDesc.ByteWidth = sizeof(D3DXMATRIX);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = sizeof(float);

    m_Device->CreateBuffer(&bufferDesc, NULL, &m_WorldBuffer);
    m_DeviceContext->VSSetConstantBuffers(0, 1, &m_WorldBuffer);

    m_Device->CreateBuffer(&bufferDesc, NULL, &m_ViewBuffer);
    m_DeviceContext->VSSetConstantBuffers(1, 1, &m_ViewBuffer);

    m_Device->CreateBuffer(&bufferDesc, NULL, &m_ProjectionBuffer);
    m_DeviceContext->VSSetConstantBuffers(2, 1, &m_ProjectionBuffer);


    bufferDesc.ByteWidth = sizeof(MATERIAL);

    m_Device->CreateBuffer(&bufferDesc, NULL, &m_MaterialBuffer);
    m_DeviceContext->VSSetConstantBuffers(3, 1, &m_MaterialBuffer);


    bufferDesc.ByteWidth = sizeof(LIGHT);

    m_Device->CreateBuffer(&bufferDesc, NULL, &m_LightBuffer);
    m_DeviceContext->VSSetConstantBuffers(4, 1, &m_LightBuffer);
    m_DeviceContext->PSSetConstantBuffers(4, 1, &m_LightBuffer);


    // ライト初期化
    LIGHT light{};
    light.Enable = true;
    light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);
    D3DXVec4Normalize(&light.Direction, &light.Direction);
    light.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
    light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    SetLight(light);


    // マテリアル初期化
    MATERIAL material{};
    material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    SetMaterial(material);
    */

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

void DirectX11::ClearScreen(uint32_t r, uint32_t g, uint32_t b)
{
    UNREFERENCED_PARAMETER(r);
    UNREFERENCED_PARAMETER(g);
    UNREFERENCED_PARAMETER(b);
}


