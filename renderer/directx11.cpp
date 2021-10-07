
#include "../utility//param.h"
#include "../system/config.h"
#include "../system/window.h"

#include "../renderer/renderer.h"

#include "directx11.h"

#include <cassert>
#include <cstdio>
#include <io.h>
#include <malloc.h>

using namespace DragonLib;
using namespace DirectX;

void DirectX11::Initialize(Window* window)
{
    HRESULT hr = S_OK;

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
        D3D_DRIVER_TYPE_SOFTWARE,
    };


    for (int i = 0; i < 4; i++)
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

    m_Window = window;

    // レンダーターゲットビューの作成
    ID3D11Texture2D* renderTarget = nullptr;
    m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&renderTarget));
    assert(renderTarget);

    m_Device->CreateRenderTargetView(renderTarget, nullptr, m_RenderTargetView.GetAddressOf());
    renderTarget->Release();


    // デプスステンシルバッファの作成
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
    m_Device->CreateDepthStencilView(depthStencil, &depthStencilViewDesc, m_DepthStencilView.GetAddressOf());
    depthStencil->Release();

    m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());


    // ビューポート設定
    D3D11_VIEWPORT viewport;
    viewport.Width      = static_cast<float>( window->GetWidth() );
    viewport.Height     = static_cast<float>( window->GetHeight() );
    viewport.MinDepth   = 0.0f;
    viewport.MaxDepth   = 1.0f;
    viewport.TopLeftX   = 0;
    viewport.TopLeftY   = 0;
    m_DeviceContext->RSSetViewports(1, &viewport);


    // ラスタライザステート設定
    D3D11_RASTERIZER_DESC rasterizerDesc{};
    rasterizerDesc.FillMode             = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode             = D3D11_CULL_BACK;
    rasterizerDesc.DepthClipEnable      = true;
    rasterizerDesc.MultisampleEnable    = false;

    ID3D11RasterizerState* rs;
    m_Device->CreateRasterizerState(&rasterizerDesc, &rs);

    m_DeviceContext->RSSetState(rs);


    // ブレンドステート設定
    D3D11_BLEND_DESC blendDesc{};
    blendDesc.AlphaToCoverageEnable                 = false;
    blendDesc.IndependentBlendEnable                = false;
    blendDesc.RenderTarget[0].BlendEnable           = true;
    blendDesc.RenderTarget[0].SrcBlend              = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    ID3D11BlendState* blendState = nullptr;
    m_Device->CreateBlendState(&blendDesc, &blendState);
    m_DeviceContext->OMSetBlendState(blendState, blendFactor, 0xFFFFFFFF);


    // デプスステンシルステート設定
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
    depthStencilDesc.DepthEnable    = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc      = D3D11_COMPARISON_LESS_EQUAL;
    depthStencilDesc.StencilEnable  = false;

    m_Device->CreateDepthStencilState(&depthStencilDesc, m_DepthStateEnable.GetAddressOf());//深度有効ステート

    depthStencilDesc.DepthEnable = false;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    m_Device->CreateDepthStencilState(&depthStencilDesc, m_DepthStateDisable.GetAddressOf());//深度無効ステート

    m_DeviceContext->OMSetDepthStencilState(m_DepthStateEnable.Get(), 0U);


    // サンプラーステート設定
    D3D11_SAMPLER_DESC samplerDesc{};
    samplerDesc.Filter      = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU    = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV    = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW    = D3D11_TEXTURE_ADDRESS_WRAP;

    ID3D11SamplerState* samplerState = nullptr;
    m_Device->CreateSamplerState(&samplerDesc, &samplerState);

    m_DeviceContext->PSSetSamplers(0, 1, &samplerState);


    // 定数バッファ生成
    D3D11_BUFFER_DESC bufferDesc{};
    bufferDesc.ByteWidth            = sizeof(DirectX::XMMATRIX);
    bufferDesc.Usage                = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags            = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags       = 0;
    bufferDesc.MiscFlags            = 0;
    bufferDesc.StructureByteStride  = sizeof(float);

    m_Device->CreateBuffer(&bufferDesc, nullptr, m_WorldBuffer.GetAddressOf());
    m_DeviceContext->VSSetConstantBuffers(0, 1, m_WorldBuffer.GetAddressOf());

    m_Device->CreateBuffer(&bufferDesc, nullptr, m_ViewBuffer.GetAddressOf());
    m_DeviceContext->VSSetConstantBuffers(1, 1, m_ViewBuffer.GetAddressOf());

    m_Device->CreateBuffer(&bufferDesc, nullptr, m_ProjectionBuffer.GetAddressOf());
    m_DeviceContext->VSSetConstantBuffers(2, 1, m_ProjectionBuffer.GetAddressOf());


    bufferDesc.ByteWidth = sizeof(Material);

    m_Device->CreateBuffer(&bufferDesc, nullptr, m_MaterialBuffer.GetAddressOf());
    m_DeviceContext->VSSetConstantBuffers(3, 1, m_MaterialBuffer.GetAddressOf());


    bufferDesc.ByteWidth = sizeof(Light);

    m_Device->CreateBuffer(&bufferDesc, nullptr, m_LightBuffer.GetAddressOf());
    m_DeviceContext->VSSetConstantBuffers(4, 1, m_LightBuffer.GetAddressOf());
    m_DeviceContext->PSSetConstantBuffers(4, 1, m_LightBuffer.GetAddressOf());


    // ライト初期化
    Light light{};
    light.Enable = true;
    light.Direction = XMVector4Normalize(_mm_setr_ps(1.0f, -1.0f, 1.0f, 0.0f));
    light.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
    light.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    SetLight(light);


    // マテリアル初期化
    Material material{};
    material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    material.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    SetMaterial(material);

}
void DirectX11::Finalize()
{

}

void DirectX11::Begin(float r, float g, float b)
{
    float clearColor[4] = { r, g, b, 1.0f };
    m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), clearColor);
    m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DirectX11::End()
{
    m_SwapChain->Present(0, 0);
}

void DirectX11::SetDepthEnable(bool enable)
{
    if (enable)
    {
        m_DeviceContext->OMSetDepthStencilState(m_DepthStateEnable.Get(), 0);
    }
    else
    {
        m_DeviceContext->OMSetDepthStencilState(m_DepthStateDisable.Get(), 0);
    }
}

void DirectX11::SetWorldViewProjection2D()
{
    XMMATRIX world = XMMatrixIdentity();
    world = XMMatrixTranspose(world);
    m_DeviceContext->UpdateSubresource(m_WorldBuffer.Get(), 0, nullptr, &world, 0, 0);

    XMMATRIX view = XMMatrixIdentity();
    view = XMMatrixTranspose(world);
    m_DeviceContext->UpdateSubresource(m_ViewBuffer.Get(), 0, nullptr, &view, 0, 0);

    XMMATRIX projection;
    if (m_CoordSystem == CoordSystem_Left)
    {
        projection = XMMatrixOrthographicOffCenterLH(0.0f, m_Window->GetWidth(), m_Window->GetHeight(), 0.0f, 0.0f, 1.0f);
    }
    else
    {
        projection = XMMatrixOrthographicOffCenterRH(0.0f, m_Window->GetWidth(), m_Window->GetHeight(), 0.0f, 0.0f, 1.0f);
    }
    projection = XMMatrixTranspose(projection);
    m_DeviceContext->UpdateSubresource(m_ProjectionBuffer.Get(), 0, nullptr, &projection, 0, 0);
}


void DirectX11::SetWorldMatrix(DirectX::XMMATRIX* worldMatrix)
{
    XMMATRIX world = XMMatrixTranspose(*worldMatrix);
    m_DeviceContext->UpdateSubresource(m_WorldBuffer.Get(), 0, nullptr, &world, 0, 0);
}

void DirectX11::SetViewMatrix(DirectX::XMMATRIX* viewMatrix)
{
    XMMATRIX view = XMMatrixTranspose(*viewMatrix);
    m_DeviceContext->UpdateSubresource(m_ViewBuffer.Get(), 0, nullptr, &view, 0, 0);
}

void DirectX11::SetProjectionMatrix(DirectX::XMMATRIX* projectionMatrix)
{
    XMMATRIX projection = XMMatrixTranspose(*projectionMatrix);
    m_DeviceContext->UpdateSubresource(m_ProjectionBuffer.Get(), 0, nullptr, &projection, 0, 0);
}


void DirectX11::SetMaterial(Material material)
{
    m_DeviceContext->UpdateSubresource(m_MaterialBuffer.Get(), 0, nullptr, &material, 0, 0);
}

void DirectX11::SetLight(Light light)
{
    m_DeviceContext->UpdateSubresource(m_LightBuffer.Get(), 0, nullptr, &light, 0, 0);
}

void DirectX11::CreateVertexShader(ID3D11VertexShader** vertexShader, ID3D11InputLayout** vertexLayout, const char* fileName)
{
    FILE* fp = nullptr;
    long int fsize = 0;

    fopen_s(&fp, fileName, "rb");
    assert(fp);
    fsize = _filelength(_fileno(fp));
    unsigned char* buffer = reinterpret_cast<unsigned char*>(_malloca(fsize));
    assert(buffer);
    fread(buffer, fsize, 1, fp);
    fclose(fp);

    m_Device->CreateVertexShader(buffer, fsize, nullptr, vertexShader);


    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    UINT numElements = ARRAYSIZE(layout);

    m_Device->CreateInputLayout(layout,
        numElements,
        buffer,
        fsize,
        vertexLayout);

    _freea(buffer);
}

void DirectX11::CreatePixelShader(ID3D11PixelShader** pixelShader, const char* fileName)
{
    FILE* fp = nullptr;
    long int fsize = 0;

    fopen_s(&fp, fileName, "rb");
    assert(fp);
    fsize = _filelength(_fileno(fp));
    unsigned char* buffer = reinterpret_cast<unsigned char*>(_malloca(fsize));
    assert(buffer);
    fread(buffer, fsize, 1, fp);
    fclose(fp);

    m_Device->CreatePixelShader(buffer, fsize, nullptr, pixelShader);

    _freea(buffer);
}



void DirectX11::SetCoordSystem(uint32_t cmd)
{
    if (cmd == CoordSystem_Left || cmd == CoordSystem_Right)
    {
        m_CoordSystem = cmd;
    }
}


