#include <tchar.h>
#include <windows.h>
#include <wrl/client.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <string>
#include <cassert>

#define _USE_MATH_DEFINES
//#include <cmath>
#include <math.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define CLASS_NAME TEXT("App Class")
#define WINDOW_CAPTION TEXT("Window Caption")

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

using namespace Microsoft::WRL;
using namespace DirectX;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void InitializeWindow();
void FinalizeWindow();

void InitializeDirectX12();
void FinalizeDirectX12();
void BeginDirectX12();
void EndDirectX12();

struct Vertex3D
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT2 TexCoord;
	XMFLOAT4 Diffuse;
};

struct MATERIAL
{
	XMFLOAT4	Ambient;
	XMFLOAT4	Diffuse;
	XMFLOAT4	Specular;
	XMFLOAT4	Emission;
	float		Shininess;
	float		Dummy[3];
};

struct LIGHT
{
	bool     Enable;
	bool     Dummy[3];
	XMFLOAT4 Direction;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Ambient;
};

/*
struct ConstantBuffer
{
	XMMATRIX	World;
	XMMATRIX	View;
	XMMATRIX	Projection;
	//MATERIAL	Material;
	//LIGHT		Light;
	//XMVECTOR	Camera;
};
*/

constexpr UINT BUFFER_COUNT = 2;


HWND		g_hWnd;
HINSTANCE	g_hInstance;

ComPtr<ID3D12Device>				g_Device;						// デバイス
ComPtr<ID3D12CommandQueue>			g_CmdQueue;						// コマンドキュー
ComPtr<ID3D12CommandAllocator>		g_CmdAllocator;					// コマンドアロケーター
ComPtr<ID3D12GraphicsCommandList>	g_CmdList;						// グラフィックスコマンドリスト
ComPtr<IDXGISwapChain3>				g_SwapChain;					// スワップチェイン
ComPtr<ID3D12DescriptorHeap>		g_RtvHeap;						// レンダーターゲットビューのヒープ
ComPtr<ID3D12DescriptorHeap>		g_DsvHeap;						// デプスステンシルビューのヒープ
ComPtr<ID3D12DescriptorHeap>		g_CbvHeap;						// 定数バッファビューのヒープ
ComPtr<ID3D12DescriptorHeap>		g_SrvHeap;
ComPtr<ID3D12DescriptorHeap>		g_CbvSrvHeap;						// シェーダーリソースビューのヒープ
ComPtr<ID3D12Resource>				g_RenderTargets[BUFFER_COUNT];	// レンダーターゲットのバッファ
ComPtr<ID3D12Resource>				g_DepthStencil;					// デプスステンシルのバッファ
ComPtr<ID3D12RootSignature>			g_RootSignature;				// ルートシグネチャ
ComPtr<ID3D12PipelineState>			g_PipelineState;				// パイプラインステート
ComPtr<ID3D12Fence>					g_Fence;						// フェンス

// 定数バッファ
ComPtr<ID3D12Resource>	g_ConstantBuffer;		// 定数バッファ
XMMATRIX				g_ConstantBufferData;	// 定数バッファの送信する前の仮置き
XMMATRIX*				g_CbvDataBegin;			// 定数バッファ(GPU)の仮想アドレス収納

//ComPtr<ID3D12Resource>	g_WorldBuffer;		// ワールド行列
//ComPtr<ID3D12Resource>	g_ViewBuffer;		// ビュー行列
//ComPtr<ID3D12Resource>	g_ProjectionBuffer;	// プロジェクション行列
//ComPtr<ID3D12Resource>	g_MaterialBuffer;	// マテリアル
//ComPtr<ID3D12Resource>	g_LightBuffer;		// ライト
//ComPtr<ID3D12Resource>	g_CameraBuffer;		// カメラ座標

// 頂点バッファ
ComPtr<ID3D12Resource>		g_VertexBuffer;		// 頂点バッファ
D3D12_VERTEX_BUFFER_VIEW	g_VertexBufferView;	// 頂点バッファビュー

// インデックスバッファ
ComPtr<ID3D12Resource>		g_IndexBuffer;
D3D12_INDEX_BUFFER_VIEW		g_IndexBufferView;

UINT64 g_FenceValue		= 0;
HANDLE g_FenceEvent		= nullptr;
HANDLE g_EventHandle	= nullptr;

D3D12_VIEWPORT	g_Viewport;
D3D12_RECT		g_ScissorRect;

UINT g_FrameIndex				= 0;
UINT g_RtvDescriptorHeapSize	= 0;
UINT g_DsvDescriptorHeapSize	= 0;
UINT g_CbvSrvDescriptorHeapSize	= 0;

UINT64 g_Width	= 0;
UINT   g_Height	= 0;

float g_RotateAngle = 0.0f;

ComPtr<ID3D12Resource>	g_Texture; // テクスチャ


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	InitializeWindow();
	InitializeDirectX12();

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	MSG msg;
	while (1)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			BeginDirectX12();
			EndDirectX12();
		}
	}

	FinalizeDirectX12();
	FinalizeWindow();

    return 0;
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

void InitializeWindow()
{
	g_hInstance = GetModuleHandle(nullptr);
	assert(g_hInstance);

	WNDCLASSEX windowClass =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		g_hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};

	RegisterClassEx(&windowClass);

	g_hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_CAPTION,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(static_cast<int>(WINDOW_WIDTH)  + GetSystemMetrics(SM_CXDLGFRAME) * 2),
		(static_cast<int>(WINDOW_HEIGHT) + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)),
		NULL,
		NULL,
		g_hInstance,
		NULL);
	assert(g_hWnd);
}
void FinalizeWindow()
{
	UnregisterClass(CLASS_NAME, g_hInstance);
}

void WaitDrawDone()
{
	// ハンドル状態にして、フェンス値を増加
	const UINT64 fence = g_FenceValue;
	auto hr = g_CmdQueue->Signal(g_Fence.Get(), fence);
	assert(SUCCEEDED(hr));

	g_FenceValue++;

	// 完了を待機
	if (g_Fence->GetCompletedValue() < fence)
	{
		hr = g_Fence->SetEventOnCompletion(fence, g_FenceEvent);
		assert(SUCCEEDED(hr));

		WaitForSingleObject( g_FenceEvent, INFINITE );
	}

	// フレームバッファ番号を更新
	g_FrameIndex = g_SwapChain->GetCurrentBackBufferIndex();
}

bool CreateBufferDiscriptorHeap(ID3D12DescriptorHeap** descriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_DESCRIPTOR_HEAP_FLAGS flag = D3D12_DESCRIPTOR_HEAP_FLAG_NONE)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.NumDescriptors	= 1;
	desc.Flags			= flag;
	desc.Type			= type;

	return SUCCEEDED(g_Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(descriptorHeap)));
}
bool CreateBuffer(ID3D12Resource** resource, UINT64 size, D3D12_HEAP_TYPE type = D3D12_HEAP_TYPE_DEFAULT)
{
	// ヒーププロパティの設定
	D3D12_HEAP_PROPERTIES prop;
	ZeroMemory(&prop, sizeof(prop));
	prop.Type					= type;
	prop.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask		= 1;
	prop.VisibleNodeMask		= 1;

	// リソースの設定
	D3D12_RESOURCE_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Dimension			= D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment			= 0;
	desc.Width				= size;
	desc.Height				= 1;
	desc.DepthOrArraySize	= 1;
	desc.MipLevels			= 1;
	desc.Format				= DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count	= 1;
	desc.SampleDesc.Quality	= 0;
	desc.Layout				= D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags				= D3D12_RESOURCE_FLAG_NONE;

	// リソースの生成
	return SUCCEEDED(g_Device->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(resource)));
}
bool CreateConstantBufferDiscriptorHeap()
{
	return CreateBufferDiscriptorHeap(
		&g_CbvHeap,
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
}
bool CreateConstantBuffer(ID3D12Resource** resource, UINT64 size)
{
	return CreateBuffer(resource, size, D3D12_HEAP_TYPE_UPLOAD);
}

void InitializeDirectX12()
{
	HRESULT hr = S_OK;

	// ウィンドウの幅を代入
	g_Width		= WINDOW_WIDTH;
	g_Height	= WINDOW_HEIGHT;
	
	// ファクトリーの生成
	ComPtr<IDXGIFactory6> factory;
	{
		UINT dxgiFactoryFlags = 0;
		#if defined(_DEBUG)
		{
			// デバッグレイヤーの有効化
			ComPtr<ID3D12Debug> debugController;
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
			{
				debugController->EnableDebugLayer();
				dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
			}
		}
		#endif
		
		hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory));
		assert(SUCCEEDED(hr));
	}

	// デバイスの生成
	{
		hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&g_Device));
		assert(SUCCEEDED(hr));
	}

	// コマンドキューの生成
	{
		D3D12_COMMAND_QUEUE_DESC desc{};
		desc.Flags	= D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.Type	= D3D12_COMMAND_LIST_TYPE_DIRECT;

		hr = g_Device->CreateCommandQueue(&desc, IID_PPV_ARGS(&g_CmdQueue));
		assert(SUCCEEDED(hr));
	}

	// コマンドアロケーターの生成
	{
		hr = g_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_CmdAllocator));
		assert(SUCCEEDED(hr));
	}

	// コマンドリストの生成
	{
		hr = g_Device->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			g_CmdAllocator.Get(),
			nullptr,
			IID_PPV_ARGS(&g_CmdList));
		assert(SUCCEEDED(hr));

		hr = g_CmdList->Close();
		assert(SUCCEEDED(hr));
	}

	// コマンドリストとアロケーターのリセット
	/*
	{
		hr = g_CmdAllocator->Reset();
		assert(SUCCEEDED(hr));

		hr = g_CmdList->Reset(g_CmdAllocator.Get(), g_PipelineState.Get());
		assert(SUCCEEDED(hr));
	}
	*/

	// フェンスの生成
	{
		hr = g_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_Fence));
		assert(SUCCEEDED(hr));
	}

	// フェンス用イベントの生成
	{
		g_FenceValue = 1;
		g_FenceEvent = CreateEventEx( nullptr, nullptr, 0, EVENT_ALL_ACCESS );
		assert(g_FenceEvent != nullptr);
	}

	// ビューポートの設定
	{
		g_Viewport.TopLeftX = 0;
		g_Viewport.TopLeftY = 0;
		g_Viewport.Width	= static_cast<float>(g_Width);
		g_Viewport.Height	= static_cast<float>(g_Height);
		g_Viewport.MinDepth = 0.0f;
		g_Viewport.MaxDepth = 1.0f;
	}

	// シザー短形の設定
	{
		g_ScissorRect.left		= 0;
		g_ScissorRect.right		= static_cast<long>(g_Width);
		g_ScissorRect.top		= 0;
		g_ScissorRect.bottom	= static_cast<long>(g_Height);
	}

	// スワップチェインの生成
	{
		DXGI_SWAP_CHAIN_DESC desc{};
		desc.BufferCount		= BUFFER_COUNT;
		desc.BufferDesc.Width	= static_cast<UINT>(g_Width);
		desc.BufferDesc.Height	= static_cast<UINT>(g_Height);
		desc.BufferDesc.Format	= DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferUsage		= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.SwapEffect			= DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.OutputWindow		= g_hWnd;
		desc.SampleDesc.Count	= 1;
		desc.Windowed			= true;

		// スワップチェインの生成
		ComPtr<IDXGISwapChain> swapChain;
		hr = factory->CreateSwapChain(g_CmdQueue.Get(), &desc, &swapChain);
		assert(SUCCEEDED(hr));

		// IDXGISwapChain3に変換
		hr = swapChain->QueryInterface(IID_PPV_ARGS(&g_SwapChain));
		assert(SUCCEEDED(hr));

		// フレームバッファ番号を取得
		g_FrameIndex = g_SwapChain->GetCurrentBackBufferIndex();
	}

	// レンダーターゲットビュー用ディスクリプタヒープの生成
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc{};
		desc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		desc.NumDescriptors	= BUFFER_COUNT;

		hr = g_Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_RtvHeap));
		assert(SUCCEEDED(hr));

		g_RtvDescriptorHeapSize = g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	// レンダーターゲットビューの生成
	{
		// CPUハンドルの先頭を取得
		D3D12_CPU_DESCRIPTOR_HANDLE handle = g_RtvHeap->GetCPUDescriptorHandleForHeapStart();

		D3D12_RENDER_TARGET_VIEW_DESC desc{};
		desc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		desc.ViewDimension			= D3D12_RTV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice		= 0;
		desc.Texture2D.PlaneSlice	= 0;

		for (UINT i = 0; i < BUFFER_COUNT; i++)
		{
			// バックバッファの取得
			hr = g_SwapChain->GetBuffer(i, IID_PPV_ARGS(&g_RenderTargets[i]));
			assert(SUCCEEDED(hr));

			// レンダーターゲットビューを生成
			g_Device->CreateRenderTargetView(g_RenderTargets[i].Get(), &desc, handle);

			// ハンドルのポインタを進める
			handle.ptr += g_RtvDescriptorHeapSize;
		}
	}

	// 深度ステンシルビュー用ディスクリプタヒープの生成
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc{};
		desc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		desc.NumDescriptors	= BUFFER_COUNT;

		hr = g_Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_DsvHeap));
		assert(SUCCEEDED(hr));

		g_DsvDescriptorHeapSize = g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	}

	// 深度ステンシルビューの生成
	{
		// ヒーププロパティの設定
		D3D12_HEAP_PROPERTIES prop{};
		prop.Type					= D3D12_HEAP_TYPE_DEFAULT;
		prop.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		prop.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
		prop.CreationNodeMask		= 1;
		prop.VisibleNodeMask		= 1;

		// リソースの設定
		D3D12_RESOURCE_DESC desc{};
		desc.Dimension			= D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		desc.Alignment			= 0;
		desc.Width				= g_Width;
		desc.Height				= g_Height;
		desc.DepthOrArraySize	= 1;
		desc.MipLevels			= 0;
		desc.Format				= DXGI_FORMAT_D32_FLOAT;
		desc.SampleDesc.Count	= 1;
		desc.SampleDesc.Quality	= 0;
		desc.Flags				= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		desc.Layout				= D3D12_TEXTURE_LAYOUT_UNKNOWN;

		// クリア値の設定
		D3D12_CLEAR_VALUE clearValue{};
		clearValue.Format				= DXGI_FORMAT_D32_FLOAT;
		clearValue.DepthStencil.Depth	= 1.0f;
		clearValue.DepthStencil.Stencil	= 0;

		// リソースの生成
		hr = g_Device->CreateCommittedResource(
			&prop,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			IID_PPV_ARGS(&g_DepthStencil));
		assert(SUCCEEDED(hr));

		// 深度ステンシルビューの設定
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
		dsvDesc.Format			= DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension	= D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Flags			= D3D12_DSV_FLAG_NONE;

		// 深度ステンシルビューの生成
		g_Device->CreateDepthStencilView(
			g_DepthStencil.Get(),
			&dsvDesc,
			g_DsvHeap->GetCPUDescriptorHandleForHeapStart());
	}

	// 定数バッファ用のディスクリプターヒープの生成
	{
		CreateConstantBufferDiscriptorHeap();
	}

	// 定数バッファの生成
	{
		CreateConstantBuffer(&g_ConstantBuffer, 256);

		// 定数バッファビューの設定
		D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
		desc.BufferLocation	= g_ConstantBuffer->GetGPUVirtualAddress();
		desc.SizeInBytes	= 256;

		// 定数バッファビューの生成
		g_Device->CreateConstantBufferView(&desc, g_CbvHeap->GetCPUDescriptorHandleForHeapStart());

		// マップ (終了時までアンマップしない)
		hr = g_ConstantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&g_CbvDataBegin));
		assert(SUCCEEDED(hr));

		// アスペクト比の算出
		float aspectRatio = static_cast<float>(g_Width) / static_cast<float>(g_Height);

		// ビューの設定
		XMVECTOR eye	= { 0.0f, 0.0f, -5.0f };
		XMVECTOR at		= { 0.0f, 0.0f,  0.0f };
		XMVECTOR up		= { 0.0f, 1.0f,  0.0f };

		// マテリアルの初期設定
		MATERIAL mat{};
		mat.Diffuse		= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.Ambient		= XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		mat.Emission	= XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		mat.Specular	= XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		mat.Shininess	= 0.75f;

		// ライトの初期設定
		LIGHT light{};
		light.Enable	= false;
		light.Diffuse	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light.Ambient	= XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		light.Direction	= { 0.0f, -0.3f, -0.7f, 0.0f };

		// 定数バッファデータの設定
		//g_ConstantBufferData.World		= XMMatrixIdentity();
		//g_ConstantBufferData.View		= XMMatrixLookAtLH(eye, at, up);
		//g_ConstantBufferData.Projection	= XMMatrixPerspectiveLH(static_cast<float>(g_Width), static_cast<float>(g_Height), 1.0f, 1000.0f);
		//g_ConstantBufferData.Projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), aspectRatio, 0.1f, 100.0f);
		//g_ConstantBufferData.Material	= mat;
		//g_ConstantBufferData.Light		= light;
		//g_ConstantBufferData.Camera		= {}; // reserved

		g_ConstantBufferData = XMMatrixIdentity() * XMMatrixLookAtLH(eye, at, up) * XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), aspectRatio, 0.1f, 100.0f);

		// GPUのメモリへコピー
		memcpy(g_CbvDataBegin, &g_ConstantBufferData, sizeof(g_ConstantBufferData));
	}

	// ルートシグネチャ
	{
		// ディスクリプタレンジの設定
		D3D12_DESCRIPTOR_RANGE range[1]{};
		/*
		range[0].RangeType							= D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		range[0].NumDescriptors						= 1;
		range[0].BaseShaderRegister					= 0;
		range[0].RegisterSpace						= 0;
		range[0].OffsetInDescriptorsFromTableStart	= D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		*/

		range[0].RangeType							= D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		range[0].NumDescriptors						= 1;
		range[0].BaseShaderRegister					= 0;
		range[0].RegisterSpace						= 0;
		range[0].OffsetInDescriptorsFromTableStart	= D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		// ルートパラメータの設定
		D3D12_ROOT_PARAMETER param[2]{};
		param[0].ParameterType							= D3D12_ROOT_PARAMETER_TYPE_CBV;
		param[0].ShaderVisibility						= D3D12_SHADER_VISIBILITY_VERTEX;
		param[0].Descriptor.RegisterSpace				= 0;
		param[0].Descriptor.ShaderRegister				= 0;

		param[1].ParameterType							= D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		param[1].ShaderVisibility						= D3D12_SHADER_VISIBILITY_PIXEL;
		param[1].DescriptorTable.NumDescriptorRanges	= 1;
		param[1].DescriptorTable.pDescriptorRanges		= &range[0];

		/*
		D3D12_ROOT_PARAMETER param[1]{};
		param[0].ParameterType							= D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		param[0].ShaderVisibility						= D3D12_SHADER_VISIBILITY_ALL;
		param[0].DescriptorTable.NumDescriptorRanges	= 2;
		param[0].DescriptorTable.pDescriptorRanges		= range;
		*/

		// サンプラーの設定
		D3D12_STATIC_SAMPLER_DESC samplerDesc{};
		samplerDesc.AddressU			= D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 横方向の繰り返し
		samplerDesc.AddressV			= D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 縦方向の繰り返し
		samplerDesc.AddressW			= D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 奥行きの繰り返し
		samplerDesc.BorderColor			= D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK; // ボーダーは黒
		samplerDesc.Filter				= D3D12_FILTER_MIN_MAG_MIP_LINEAR; // 線形補間
		samplerDesc.MaxLOD				= D3D12_FLOAT32_MAX; // ミップマップ最大値
		samplerDesc.MinLOD				= 0.0f; // ミップマップ最小値
		samplerDesc.ShaderVisibility	= D3D12_SHADER_VISIBILITY_PIXEL; // ピクセルシェーダーから見える
		samplerDesc.ComparisonFunc		= D3D12_COMPARISON_FUNC_NEVER; // リサンプリングしない

		// ルートシグネチャの設定
		D3D12_ROOT_SIGNATURE_DESC desc{};
		desc.NumParameters		= _countof(param);
		desc.pParameters		= param;
		desc.NumStaticSamplers	= 1;
		desc.pStaticSamplers	= &samplerDesc;
		desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;

		// シリアライズ
		hr = D3D12SerializeRootSignature(
			&desc,
			D3D_ROOT_SIGNATURE_VERSION_1,
			&signature,
			&error);
		assert(SUCCEEDED(hr));

		hr = g_Device->CreateRootSignature(
			0,
			signature->GetBufferPointer(),
			signature->GetBufferSize(),
			IID_PPV_ARGS(&g_RootSignature));
		assert(SUCCEEDED(hr));
	}

	// パイプラインステート
	{
		ComPtr<ID3DBlob> VSBlob;
		ComPtr<ID3DBlob> PSBlob;

		// シェーダーの読み込み
		hr = D3DReadFileToBlob(L"Asset/Shader/unlitTextureVS.cso", &VSBlob);
		assert(SUCCEEDED(hr));
		hr = D3DReadFileToBlob(L"Asset/Shader/unlitTexturePS.cso", &PSBlob);
		assert(SUCCEEDED(hr));

		// 無効なポインタを渡さないために関数内で保持する
		const char position[]	= "POSITION";
		const char normal[]		= "NORMAL";
		const char texCoord[]	= "TEXCOORD";
		const char color[]		= "COLOR";

		// 入力レイアウトの設定
		D3D12_INPUT_ELEMENT_DESC inputElements[] = {
			{ position,	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ normal,	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ texCoord,	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ color,	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};

		// ラスタライザーステートの設定
		D3D12_RASTERIZER_DESC descRS{};
		descRS.FillMode					= D3D12_FILL_MODE_SOLID;
		descRS.CullMode					= D3D12_CULL_MODE_BACK;
		descRS.FrontCounterClockwise	= false;
		descRS.DepthBias				= D3D12_DEFAULT_DEPTH_BIAS;
		descRS.DepthBiasClamp			= D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		descRS.SlopeScaledDepthBias		= D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		descRS.DepthClipEnable			= true;
		descRS.MultisampleEnable		= false;
		descRS.AntialiasedLineEnable	= false;
		descRS.ForcedSampleCount		= 0;
		descRS.ConservativeRaster		= D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

		// レンダーターゲットのブレンド設定
		D3D12_RENDER_TARGET_BLEND_DESC descRTBS = {
			false, false,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_LOGIC_OP_NOOP,
			D3D12_COLOR_WRITE_ENABLE_ALL
		};

		// ブレンドステートの設定
		D3D12_BLEND_DESC descBS{};
		descBS.AlphaToCoverageEnable	= false;
		descBS.IndependentBlendEnable	= false;
		for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		{
			descBS.RenderTarget[i] = descRTBS;
		}

		// パイプラインステートの設定
		D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
		desc.InputLayout						= { inputElements, _countof(inputElements) };
		desc.pRootSignature						= g_RootSignature.Get();
		desc.VS									= { reinterpret_cast<UINT8*>(VSBlob->GetBufferPointer()), VSBlob->GetBufferSize() };
		desc.PS									= { reinterpret_cast<UINT8*>(PSBlob->GetBufferPointer()), PSBlob->GetBufferSize() };
		desc.RasterizerState					= descRS;
		desc.BlendState							= descBS;
		desc.DepthStencilState.DepthEnable		= false;
		desc.DepthStencilState.StencilEnable	= false;
		desc.SampleMask							= UINT_MAX;
		desc.PrimitiveTopologyType				= D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		desc.NumRenderTargets					= 1;
		desc.RTVFormats[0]						= DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		desc.DSVFormat							= DXGI_FORMAT_D32_FLOAT;
		desc.SampleDesc.Count					= 1;

		// パイプラインステートの生成
		hr = g_Device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&g_PipelineState));
		assert(SUCCEEDED(hr));
	}

	// 頂点バッファの生成
	{
		// 頂点データ
		Vertex3D vertices[] =
		{
			// 正面
			{ { -0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f,  1.0f }, { 0.0f, 0.0f },  { 1.0f, 1.0f, 1.0f, 1.0f }  },
			{ { -0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f,  1.0f }, { 0.0f, 1.0f },  { 1.0f, 1.0f, 1.0f, 1.0f }  },
			{ {  0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f,  1.0f }, { 1.0f, 0.0f },  { 1.0f, 1.0f, 1.0f, 1.0f }  },
			{ {  0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f,  1.0f }, { 1.0f, 1.0f },  { 1.0f, 1.0f, 1.0f, 1.0f }  },
		};

		// ヒーププロパティの設定
		D3D12_HEAP_PROPERTIES prop{};
		prop.Type					= D3D12_HEAP_TYPE_UPLOAD;
		prop.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		prop.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
		prop.CreationNodeMask		= 1;
		prop.VisibleNodeMask		= 1;

		// リソースの設定
		D3D12_RESOURCE_DESC desc{};
		desc.Dimension			= D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Alignment			= 0;
		desc.Width				= sizeof(vertices);
		desc.Height				= 1;
		desc.DepthOrArraySize	= 1;
		desc.MipLevels			= 1;
		desc.Format				= DXGI_FORMAT_UNKNOWN;
		desc.SampleDesc.Count	= 1;
		desc.SampleDesc.Quality	= 0;
		desc.Layout				= D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		desc.Flags				= D3D12_RESOURCE_FLAG_NONE;

		// リソースの生成
		hr = g_Device->CreateCommittedResource(
			&prop,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&g_VertexBuffer));
		assert(SUCCEEDED(hr));

		// マップ
		Vertex3D* pData;
		hr = g_VertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pData));
		assert(SUCCEEDED(hr));

		// 頂点データのコピー
		memcpy(pData, vertices, sizeof(vertices));

		// アンマップ
		g_VertexBuffer->Unmap(0, nullptr);

		// 頂点バッファビューの設定
		g_VertexBufferView.BufferLocation	= g_VertexBuffer->GetGPUVirtualAddress();
		g_VertexBufferView.StrideInBytes	= sizeof(Vertex3D);
		g_VertexBufferView.SizeInBytes		= sizeof(vertices);
	}

	// インデックスバッファの生成
	{
		uint32_t index[] =
		{
			   0, 1, 2, 3
		};

		// ヒーププロパティの設定
		D3D12_HEAP_PROPERTIES prop{};
		prop.Type					= D3D12_HEAP_TYPE_UPLOAD;
		prop.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		prop.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
		prop.CreationNodeMask		= 1;
		prop.VisibleNodeMask		= 1;

		// リソースの設定
		D3D12_RESOURCE_DESC desc{};
		desc.Dimension			= D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Alignment			= 0;
		desc.Width				= sizeof(index);
		desc.Height				= 1;
		desc.DepthOrArraySize	= 1;
		desc.MipLevels			= 1;
		desc.Format				= DXGI_FORMAT_UNKNOWN;
		desc.SampleDesc.Count	= 1;
		desc.SampleDesc.Quality	= 0;
		desc.Layout				= D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		desc.Flags				= D3D12_RESOURCE_FLAG_NONE;

		// リソースの生成
		hr = g_Device->CreateCommittedResource(
			&prop,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&g_IndexBuffer));
		assert(SUCCEEDED(hr));

		// マップ
		uint32_t* pData;
		hr = g_IndexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pData));
		assert(SUCCEEDED(hr));

		// 頂点データのコピー
		memcpy(pData, index, sizeof(index));

		// アンマップ
		g_IndexBuffer->Unmap(0, nullptr);

		// 頂点バッファビューの設定
		g_IndexBufferView.BufferLocation	= g_IndexBuffer->GetGPUVirtualAddress();
		g_IndexBufferView.Format			= DXGI_FORMAT_R32_UINT;
		g_IndexBufferView.SizeInBytes		= sizeof(index);
	}

	// テクスチャの生成
	{
		// ヒーププロパティの設定
		D3D12_HEAP_PROPERTIES prop{};
		prop.Type					= D3D12_HEAP_TYPE_CUSTOM;
		prop.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
		prop.MemoryPoolPreference	= D3D12_MEMORY_POOL_L0;
		prop.CreationNodeMask		= 0;
		prop.VisibleNodeMask		= 0;

		// リソースの設定
		D3D12_RESOURCE_DESC desc{};
		desc.Dimension			= D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		desc.Width				= 256;
		desc.Height				= 256;
		desc.DepthOrArraySize	= 1;
		desc.MipLevels			= 1;
		desc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count	= 1;
		desc.SampleDesc.Quality	= 0;
		desc.Flags				= D3D12_RESOURCE_FLAG_NONE;
		desc.Layout				= D3D12_TEXTURE_LAYOUT_UNKNOWN;

		// リソースの生成
		hr = g_Device->CreateCommittedResource(
			&prop,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			nullptr,
			IID_PPV_ARGS(&g_Texture));
		assert(SUCCEEDED(hr));
	}

	// テクスチャデータの作成
	uint32_t pixels[256 * 256]{};
	{
		// ABGRの順で処理
		for (UINT y = 0; y < 256; y++)
		{
			for (UINT x = 0; x < 256; x++)
			{
				if (y < 127 && x < 127 || y >= 128 && x >= 128)
				{
					pixels[x + y * 256] = 0xFFFFFFFF;
				}
				else
				{
					pixels[x + y * 256] = 0xFF000000;
				}
			}
		}
	}

	// サブリソースの更新
	{
		hr = g_Texture->WriteToSubresource(
			0,
			nullptr, // 全領域へコピー
			pixels, // コピー元データ
			sizeof(uint32_t) * 256, // 1ラインサイズ
			sizeof(uint32_t) * 256 * 256); // 全サイズ
		assert(SUCCEEDED(hr));
	}

	// 定数バッファビューとシェーダーリソースビューのデスクリプターヒープを生成
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc{};
		desc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.NodeMask		= 0;
		desc.NumDescriptors = 2;
		desc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

		hr = g_Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_CbvSrvHeap));
		assert(SUCCEEDED(hr));

		g_CbvSrvDescriptorHeapSize = g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	// 定数バッファビューとシェーダーリソースビューのデスクリプターヒープを生成
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc{};
		desc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.NodeMask		= 0;
		desc.NumDescriptors = 1;
		desc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

		hr = g_Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_CbvSrvHeap));
		assert(SUCCEEDED(hr));

		g_CbvSrvDescriptorHeapSize = g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	// シェーダーリソースビューのディスクリプターヒープの生成
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc{};
		desc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		desc.NumDescriptors	= 1;
		desc.NodeMask		= 0;

		hr = g_Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_SrvHeap));
		assert(SUCCEEDED(hr));
	}

	// シェーダーリソースビューの生成
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Format						= DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.Shader4ComponentMapping	= D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		desc.ViewDimension				= D3D12_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels		= 1;

		g_Device->CreateShaderResourceView(
			g_Texture.Get(),
			&desc,
			g_SrvHeap->GetCPUDescriptorHandleForHeapStart());
	}
}

void FinalizeDirectX12()
{
	// コマンドの終了を待機
	WaitDrawDone();

	// イベントハンドルを閉じる
	CloseHandle(g_FenceEvent);
	g_FenceEvent = nullptr;

	for (UINT i = 0; i < BUFFER_COUNT; i++)
	{
		g_RenderTargets[i].Reset();
	}

	g_DepthStencil	.Reset();
	g_SwapChain		.Reset();
	g_Fence			.Reset();
	g_CmdList		.Reset();
	g_CmdQueue		.Reset();
	g_Device		.Reset();
}

void BeginDirectX12()
{
	// 回転角を増やす
	g_RotateAngle += (0.05f);

	XMVECTOR axis = { 0.0f, 0.0f, 1.0f, 0.0f };

	// ワールド行列を更新
	//g_ConstantBufferData.World = XMMatrixRotationAxis(axis, g_RotateAngle);

	// 定数バッファを更新
	XMMATRIX world		= XMMatrixRotationAxis(axis, g_RotateAngle);
	XMMATRIX view		= XMMatrixLookAtLH({ 0, 0, -5 }, { 0, 0, 0 }, { 0, 1, 0 });
	XMMATRIX projection	= XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), static_cast<float>(g_Width) / static_cast<float>(g_Height), 0.1f, 100.0f);

	g_ConstantBufferData = XMMatrixTranspose(world * view * projection);
	*g_CbvDataBegin = g_ConstantBufferData;

	//memcpy(g_CbvDataBegin, &g_ConstantBufferData, sizeof(g_ConstantBufferData));

	//g_CbvDataBegin->World = g_ConstantBufferData.World;
	//g_CbvDataBegin->View = g_ConstantBufferData.View;
	//g_CbvDataBegin->Projection = g_ConstantBufferData.Projection;


	// コマンドアロケータとコマンドリストをリセット
	g_CmdAllocator->Reset();
	g_CmdList->Reset(g_CmdAllocator.Get(), g_PipelineState.Get());

	// パイプラインステートの設定
	g_CmdList->SetPipelineState(g_PipelineState.Get());

	// ビューポートの設定
	g_CmdList->RSSetViewports(1, &g_Viewport);

	// シザー短形
	g_CmdList->RSSetScissorRects(1, &g_ScissorRect);



	// ルートシグネチャを設定
	g_CmdList->SetGraphicsRootSignature(g_RootSignature.Get());

	// ディスクリプタヒープテーブルを設定
	g_CmdList->SetGraphicsRootConstantBufferView(0, g_ConstantBuffer->GetGPUVirtualAddress());

	g_CmdList->SetDescriptorHeaps(1, g_SrvHeap.GetAddressOf());
	g_CmdList->SetGraphicsRootDescriptorTable(1, g_SrvHeap->GetGPUDescriptorHandleForHeapStart());
	



	// リソースバリアの設定
	// Present ---> RenderTarget
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type					= D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags					= D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource	= g_RenderTargets[g_FrameIndex].Get();
	barrier.Transition.StateBefore	= D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter	= D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource	= D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	g_CmdList->ResourceBarrier(1, &barrier);

	// レンダーターゲットのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE handleRTV = g_RtvHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_CPU_DESCRIPTOR_HANDLE handleDSV = g_DsvHeap->GetCPUDescriptorHandleForHeapStart();
	handleRTV.ptr += ( g_FrameIndex * g_RtvDescriptorHeapSize );

	// レンダーターゲットの設定
	g_CmdList->OMSetRenderTargets(1, &handleRTV, false, &handleDSV);

	// レンダーターゲットビューをクリア
	const float clearColor[] = { 0.39f, 0.58f, 0.92f, 1.0f };
	g_CmdList->ClearRenderTargetView(handleRTV, clearColor, 0, nullptr);

	// 深度ステンシルビューをクリア
	g_CmdList->ClearDepthStencilView(handleDSV, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);




	// 定数バッファのルートシグネチャの設定
	//g_CmdList->SetGraphicsRootSignature(g_RootSignature.Get());

	// 定数バッファのディスクリプタヒープの設定
	//g_CmdList->SetDescriptorHeaps(1, g_CbvHeap.GetAddressOf());

	// 定数バッファビューの設定
	//g_CmdList->SetGraphicsRootConstantBufferView(0, g_ConstantBuffer->GetGPUVirtualAddress());





	// 頂点バッファビューの設定
	g_CmdList->IASetVertexBuffers(0, 1, &g_VertexBufferView);

	// インデックスバッファビューの設定
	g_CmdList->IASetIndexBuffer(&g_IndexBufferView);

	// プリミティブトポロジーの設定
	g_CmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 描画コマンドの生成
	//g_CmdList->DrawInstanced(4, 1, 0, 0);
	g_CmdList->DrawIndexedInstanced(4, 1, 0, 0, 0);






	// リソースバリアの設定
	// RenderTarget ---> Present
	barrier.Transition.StateBefore	= D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter	= D3D12_RESOURCE_STATE_PRESENT;
	g_CmdList->ResourceBarrier(1, &barrier);

	// コマンドの記録を終了
	g_CmdList->Close();

	// コマンド実行
	ID3D12CommandList* cmdLists[] = { g_CmdList.Get() };
	g_CmdQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);

	// 表示する
	g_SwapChain->Present(1, 0);

	// コマンドの完了を待機
	WaitDrawDone();
}

void EndDirectX12()
{

}
