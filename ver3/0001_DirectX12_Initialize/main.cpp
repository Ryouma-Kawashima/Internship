#include <tchar.h>
#include <windows.h>
#include <wrl/client.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <cassert>

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

bool InitializeDirectX12();
void FinalizeDirectX12();

struct Vertex3D
{
	XMVECTOR Position;
	XMVECTOR Normal;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;
};

struct ConstantBuffers
{
	XMMATRIX World;
	XMMATRIX View;
	XMMATRIX Projection;
};

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
ComPtr<ID3D12DescriptorHeap>		g_SrvHeap;						// シェーダーリソースビューのヒープ
ComPtr<ID3D12Resource>				g_RenderTargets[BUFFER_COUNT];	// レンダーターゲットのバッファ
ComPtr<ID3D12Resource>				g_DepthStencil;					// デプスステンシルのバッファ

// 定数バッファ
ComPtr<ID3D12Resource>	g_WorldBuffer;		// ワールド行列
ComPtr<ID3D12Resource>	g_ViewBuffer;		// ビュー行列
ComPtr<ID3D12Resource>	g_ProjectionBuffer;	// プロジェクション行列
ComPtr<ID3D12Resource>	g_MaterialBuffer;	// マテリアル
ComPtr<ID3D12Resource>	g_LightBuffer;		// ライト
ComPtr<ID3D12Resource>	g_CameraBuffer;		// カメラ座標

// 頂点バッファ
ComPtr<ID3D12Resource>		g_VertexBuffer;		// 頂点バッファ
D3D12_VERTEX_BUFFER_VIEW	g_VertexBufferView;	// 頂点バッファビュー

UINT g_FrameIndex				= 0;
UINT g_RtvDescriptorHeapSize	= 0;
UINT g_DsvDescriptorHeapSize	= 0;
UINT g_SrvDescriptorHeapSize	= 0;

UINT64 g_Width	= 0;
UINT64 g_Height	= 0;


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	InitializeWindow();
	
	assert(InitializeDirectX12());



	FinalizeDirectX12();
	FinalizeWindow();

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		(static_cast<int>(WINDOW_WIDTH) + GetSystemMetrics(SM_CXDLGFRAME) * 2),
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

	// リソースの作成
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

bool InitializeDirectX12()
{
	HRESULT hr = S_OK;

	g_Width		= WINDOW_WIDTH;
	g_Height	= WINDOW_HEIGHT;

	// ファクトリーの作成
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
		if (FAILED(hr))
		{
			return false;
		}
	}

	// デバイスの作成
	{
		hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&g_Device));
		if (FAILED(hr))
		{
			return false;
		}
	}

	// コマンドキューの作成
	{
		D3D12_COMMAND_QUEUE_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Flags	= D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.Type	= D3D12_COMMAND_LIST_TYPE_DIRECT;

		hr = g_Device->CreateCommandQueue(&desc, IID_PPV_ARGS(&g_CmdQueue));
		if (FAILED(hr))
		{
			return false;
		}
	}

	// コマンドアロケーターの作成
	{
		hr = g_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_CmdAllocator));
		if (FAILED(hr))
		{
			return false;
		}
	}

	// コマンドリストの作成
	{
		hr = g_Device->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			g_CmdAllocator.Get(),
			nullptr,
			IID_PPV_ARGS(&g_CmdList));
		if (FAILED(hr))
		{
			return false;
		}
	}

	// スワップチェインの作成
	{
		DXGI_SWAP_CHAIN_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.BufferCount		= BUFFER_COUNT;
		desc.BufferDesc.Width	= WINDOW_WIDTH;
		desc.BufferDesc.Height	= WINDOW_HEIGHT;
		desc.BufferDesc.Format	= DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferUsage		= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.SwapEffect			= DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.OutputWindow		= g_hWnd;
		desc.SampleDesc.Count	= 1;
		desc.Windowed			= true;

		ComPtr<IDXGISwapChain> swapChain;
		hr = factory->CreateSwapChain(g_CmdQueue.Get(), &desc, &swapChain);
		if (FAILED(hr))
		{
			return false;
		}

		hr = swapChain->QueryInterface(IID_PPV_ARGS(&g_SwapChain));
		if (FAILED(hr))
		{
			return false;
		}

		g_FrameIndex = g_SwapChain->GetCurrentBackBufferIndex();
	}

	// レンダーターゲットビュー用ディスクリプタヒープの作成
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		desc.NumDescriptors	= BUFFER_COUNT;

		hr = g_Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_RtvHeap));
		if (FAILED(hr))
		{
			return false;
		}

		g_RtvDescriptorHeapSize = g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	// レンダーターゲットビューの作成
	{
		// CPUハンドルの先頭を取得
		D3D12_CPU_DESCRIPTOR_HANDLE handle = g_RtvHeap->GetCPUDescriptorHandleForHeapStart();

		D3D12_RENDER_TARGET_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.ViewDimension			= D3D12_RTV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice		= 0;
		desc.Texture2D.PlaneSlice	= 0;

		for (UINT i = 0; i < BUFFER_COUNT; i++)
		{
			// バックバッファの取得
			hr = g_SwapChain->GetBuffer(i, IID_PPV_ARGS(&g_RenderTargets[i]));
			if (FAILED(hr))
			{
				return false;
			}

			g_Device->CreateRenderTargetView(g_RenderTargets[i].Get(), &desc, handle);

			handle.ptr += g_RtvDescriptorHeapSize;
		}
	}

	// 深度ステンシルビュー用ディスクリプタヒープの作成
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		desc.NumDescriptors	= BUFFER_COUNT;

		hr = g_Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_DsvHeap));
		if (FAILED(hr))
		{
			return false;
		}

		g_DsvDescriptorHeapSize = g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	// 深度ステンシルビューの作成
	{
		// ヒーププロパティの設定
		D3D12_HEAP_PROPERTIES prop;
		ZeroMemory(&prop, sizeof(prop));
		prop.Type					= D3D12_HEAP_TYPE_DEFAULT;
		prop.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		prop.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
		prop.CreationNodeMask		= 1;
		prop.VisibleNodeMask		= 1;

		// リソースの設定
		D3D12_RESOURCE_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
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
		D3D12_CLEAR_VALUE clearValue;
		ZeroMemory(&clearValue, sizeof(clearValue));
		clearValue.Format				= DXGI_FORMAT_D32_FLOAT;
		clearValue.DepthStencil.Depth	= 1.0f;
		clearValue.DepthStencil.Stencil	= 0;

		// リソースの作成
		hr = g_Device->CreateCommittedResource(
			&prop,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			IID_PPV_ARGS(&g_DepthStencil));
		if (FAILED(hr))
		{
			return false;
		}

		// 深度ステンシルビューの設定
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		ZeroMemory(&dsvDesc, sizeof(dsvDesc));
		dsvDesc.Format			= DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension	= D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Flags			= D3D12_DSV_FLAG_NONE;

		// 深度ステンシルビューの作成
		g_Device->CreateDepthStencilView(
			g_DepthStencil.Get(),
			&dsvDesc,
			g_DsvHeap->GetCPUDescriptorHandleForHeapStart());
	}

	// 頂点バッファの作成
	{
		// 頂点データ
		Vertex3D vertices[] =
		{
			{ {  0.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f },  { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f }, },
			{ {  1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f },  { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f }, },
			{ { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f },  { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f }, }
		};

		// ヒーププロパティの設定
		D3D12_HEAP_PROPERTIES prop;
		ZeroMemory(&prop, sizeof(prop));
		prop.Type					= D3D12_HEAP_TYPE_UPLOAD;
		prop.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		prop.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
		prop.CreationNodeMask		= 1;
		prop.VisibleNodeMask		= 1;

		// リソースの設定
		D3D12_RESOURCE_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
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

		// リソースの作成
		hr = g_Device->CreateCommittedResource(
			&prop,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&g_VertexBuffer));
		if (FAILED(hr))
		{
			return false;
		}

		// マップ
		UINT8* pData;
		hr = g_VertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pData));
		if (FAILED(hr))
		{
			return false;
		}

		// 頂点データのコピー
		memcpy(pData, vertices, sizeof(vertices));

		// アンマップ
		g_VertexBuffer->Unmap(0, nullptr);

		// 頂点バッファビューの設定
		g_VertexBufferView.BufferLocation	= g_VertexBuffer->GetGPUVirtualAddress();
		g_VertexBufferView.StrideInBytes	= sizeof(Vertex3D);
		g_VertexBufferView.SizeInBytes		= sizeof(vertices);
	}

	// 定数バッファ用のディスクリプターヒープの作成
	if (! CreateConstantBufferDiscriptorHeap())
	{

	}
	/*
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.NumDescriptors	= 1;
		desc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

		hr = g_Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_CbvHeap));
		if (FAILED(hr))
		{
			return false;
		}
	}
	*/

	// 定数バッファの作成
	{
		CreateConstantBuffer(&g_WorldBuffer, sizeof(XMMATRIX), );
		hr = CreateBuffer(D3D12_HEAP_TYPE_UPLOAD, sizeof(XMMATRIX), &g_WorldBuffer);
		if (FAILED(hr))
		{
			return false;
		}

		
	}
	
	return true;
}

void FinalizeDirectX12()
{

}
