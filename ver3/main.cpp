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
	XMFLOAT4 Position;
	XMFLOAT4 Normal;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;
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

ComPtr<ID3D12Device>				g_Device;						// �f�o�C�X
ComPtr<ID3D12CommandQueue>			g_CmdQueue;						// �R�}���h�L���[
ComPtr<ID3D12CommandAllocator>		g_CmdAllocator;					// �R�}���h�A���P�[�^�[
ComPtr<ID3D12GraphicsCommandList>	g_CmdList;						// �O���t�B�b�N�X�R�}���h���X�g
ComPtr<IDXGISwapChain3>				g_SwapChain;					// �X���b�v�`�F�C��
ComPtr<ID3D12DescriptorHeap>		g_RtvHeap;						// �����_�[�^�[�Q�b�g�r���[�̃q�[�v
ComPtr<ID3D12DescriptorHeap>		g_DsvHeap;						// �f�v�X�X�e���V���r���[�̃q�[�v
ComPtr<ID3D12DescriptorHeap>		g_CbvHeap;						// �萔�o�b�t�@�r���[�̃q�[�v
ComPtr<ID3D12DescriptorHeap>		g_SrvHeap;						// �V�F�[�_�[���\�[�X�r���[�̃q�[�v
ComPtr<ID3D12Resource>				g_RenderTargets[BUFFER_COUNT];	// �����_�[�^�[�Q�b�g�̃o�b�t�@
ComPtr<ID3D12Resource>				g_DepthStencil;					// �f�v�X�X�e���V���̃o�b�t�@
ComPtr<ID3D12RootSignature>			g_RootSignature;				// ���[�g�V�O�l�`��
ComPtr<ID3D12PipelineState>			g_PipelineState;				// �p�C�v���C���X�e�[�g
ComPtr<ID3D12Fence>					g_Fence;						// �t�F���X

// �萔�o�b�t�@
ComPtr<ID3D12Resource>	g_ConstantBuffer;		// �萔�o�b�t�@
XMMATRIX			g_ConstantBufferData;	// �萔�o�b�t�@�̑��M����O�̉��u��
XMMATRIX*			g_CbvDataBegin;			// �萔�o�b�t�@(GPU)�̉��z�A�h���X���[

//ComPtr<ID3D12Resource>	g_WorldBuffer;		// ���[���h�s��
//ComPtr<ID3D12Resource>	g_ViewBuffer;		// �r���[�s��
//ComPtr<ID3D12Resource>	g_ProjectionBuffer;	// �v���W�F�N�V�����s��
//ComPtr<ID3D12Resource>	g_MaterialBuffer;	// �}�e���A��
//ComPtr<ID3D12Resource>	g_LightBuffer;		// ���C�g
//ComPtr<ID3D12Resource>	g_CameraBuffer;		// �J�������W

// ���_�o�b�t�@
ComPtr<ID3D12Resource>		g_VertexBuffer;		// ���_�o�b�t�@
D3D12_VERTEX_BUFFER_VIEW	g_VertexBufferView;	// ���_�o�b�t�@�r���[

UINT64 g_FenceValue		= 0;
HANDLE g_FenceEvent		= nullptr;
HANDLE g_EventHandle	= nullptr;

D3D12_VIEWPORT	g_Viewport;
D3D12_RECT		g_ScissorRect;

UINT g_FrameIndex				= 0;
UINT g_RtvDescriptorHeapSize	= 0;
UINT g_DsvDescriptorHeapSize	= 0;
UINT g_SrvDescriptorHeapSize	= 0;

UINT64 g_Width	= 0;
UINT   g_Height	= 0;

float g_RotateAngle = 0;


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
	// �n���h����Ԃɂ��āA�t�F���X�l�𑝉�
	const UINT64 fence = g_FenceValue;
	auto hr = g_CmdQueue->Signal(g_Fence.Get(), fence);
	assert(SUCCEEDED(hr));

	g_FenceValue++;

	// ������ҋ@
	if (g_Fence->GetCompletedValue() < fence)
	{
		hr = g_Fence->SetEventOnCompletion(fence, g_FenceEvent);
		assert(SUCCEEDED(hr));

		WaitForSingleObject( g_FenceEvent, INFINITE );
	}

	// �t���[���o�b�t�@�ԍ����X�V
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
	// �q�[�v�v���p�e�B�̐ݒ�
	D3D12_HEAP_PROPERTIES prop;
	ZeroMemory(&prop, sizeof(prop));
	prop.Type					= type;
	prop.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask		= 1;
	prop.VisibleNodeMask		= 1;

	// ���\�[�X�̐ݒ�
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

	// ���\�[�X�̍쐬
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

	// �E�B���h�E�̕�����
	g_Width		= WINDOW_WIDTH;
	g_Height	= WINDOW_HEIGHT;
	
	// �t�@�N�g���[�̍쐬
	ComPtr<IDXGIFactory6> factory;
	{
		UINT dxgiFactoryFlags = 0;
		#if defined(_DEBUG)
		{
			// �f�o�b�O���C���[�̗L����
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

	// �f�o�C�X�̍쐬
	{
		hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&g_Device));
		assert(SUCCEEDED(hr));
	}

	// �R�}���h�L���[�̍쐬
	{
		D3D12_COMMAND_QUEUE_DESC desc{};
		desc.Flags	= D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.Type	= D3D12_COMMAND_LIST_TYPE_DIRECT;

		hr = g_Device->CreateCommandQueue(&desc, IID_PPV_ARGS(&g_CmdQueue));
		assert(SUCCEEDED(hr));
	}

	// �R�}���h�A���P�[�^�[�̍쐬
	{
		hr = g_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_CmdAllocator));
		assert(SUCCEEDED(hr));
	}

	// �R�}���h���X�g�̍쐬
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

	// �R�}���h���X�g�ƃA���P�[�^�[�̃��Z�b�g
	/*
	{
		hr = g_CmdAllocator->Reset();
		assert(SUCCEEDED(hr));

		hr = g_CmdList->Reset(g_CmdAllocator.Get(), g_PipelineState.Get());
		assert(SUCCEEDED(hr));
	}
	*/

	// �t�F���X�̍쐬
	{
		hr = g_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_Fence));
		assert(SUCCEEDED(hr));
	}

	// �t�F���X�p�C�x���g�̍쐬
	{
		g_FenceValue = 1;
		g_FenceEvent = CreateEventEx( nullptr, nullptr, 0, EVENT_ALL_ACCESS );
		assert(g_FenceEvent != nullptr);
	}

	// �r���[�|�[�g�̐ݒ�
	{
		g_Viewport.TopLeftX = 0;
		g_Viewport.TopLeftY = 0;
		g_Viewport.Width	= static_cast<float>(g_Width);
		g_Viewport.Height	= static_cast<float>(g_Height);
		g_Viewport.MinDepth = 0.0f;
		g_Viewport.MaxDepth = 1.0f;
	}

	// �V�U�[�Z�`�̐ݒ�
	{
		g_ScissorRect.left		= 0;
		g_ScissorRect.right		= static_cast<long>(g_Width);
		g_ScissorRect.top		= 0;
		g_ScissorRect.bottom	= static_cast<long>(g_Height);
	}

	// �X���b�v�`�F�C���̍쐬
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

		// �X���b�v�`�F�C���̍쐬
		ComPtr<IDXGISwapChain> swapChain;
		hr = factory->CreateSwapChain(g_CmdQueue.Get(), &desc, &swapChain);
		assert(SUCCEEDED(hr));

		// IDXGISwapChain3�ɕϊ�
		hr = swapChain->QueryInterface(IID_PPV_ARGS(&g_SwapChain));
		assert(SUCCEEDED(hr));

		// �t���[���o�b�t�@�ԍ����擾
		g_FrameIndex = g_SwapChain->GetCurrentBackBufferIndex();
	}

	// �����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�q�[�v�̍쐬
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc{};
		desc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		desc.NumDescriptors	= BUFFER_COUNT;

		hr = g_Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_RtvHeap));
		assert(SUCCEEDED(hr));

		g_RtvDescriptorHeapSize = g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	// �����_�[�^�[�Q�b�g�r���[�̍쐬
	{
		// CPU�n���h���̐擪���擾
		D3D12_CPU_DESCRIPTOR_HANDLE handle = g_RtvHeap->GetCPUDescriptorHandleForHeapStart();

		D3D12_RENDER_TARGET_VIEW_DESC desc{};
		desc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		desc.ViewDimension			= D3D12_RTV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice		= 0;
		desc.Texture2D.PlaneSlice	= 0;

		for (UINT i = 0; i < BUFFER_COUNT; i++)
		{
			// �o�b�N�o�b�t�@�̎擾
			hr = g_SwapChain->GetBuffer(i, IID_PPV_ARGS(&g_RenderTargets[i]));
			assert(SUCCEEDED(hr));

			// �����_�[�^�[�Q�b�g�r���[���쐬
			g_Device->CreateRenderTargetView(g_RenderTargets[i].Get(), &desc, handle);

			// �n���h���̃|�C���^��i�߂�
			handle.ptr += g_RtvDescriptorHeapSize;
		}
	}

	// �[�x�X�e���V���r���[�p�f�B�X�N���v�^�q�[�v�̍쐬
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc{};
		desc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		desc.NumDescriptors	= BUFFER_COUNT;

		hr = g_Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_DsvHeap));
		assert(SUCCEEDED(hr));

		g_DsvDescriptorHeapSize = g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	}

	// �[�x�X�e���V���r���[�̍쐬
	{
		// �q�[�v�v���p�e�B�̐ݒ�
		D3D12_HEAP_PROPERTIES prop{};
		prop.Type					= D3D12_HEAP_TYPE_DEFAULT;
		prop.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		prop.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
		prop.CreationNodeMask		= 1;
		prop.VisibleNodeMask		= 1;

		// ���\�[�X�̐ݒ�
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

		// �N���A�l�̐ݒ�
		D3D12_CLEAR_VALUE clearValue{};
		clearValue.Format				= DXGI_FORMAT_D32_FLOAT;
		clearValue.DepthStencil.Depth	= 1.0f;
		clearValue.DepthStencil.Stencil	= 0;

		// ���\�[�X�̍쐬
		hr = g_Device->CreateCommittedResource(
			&prop,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			IID_PPV_ARGS(&g_DepthStencil));
		assert(SUCCEEDED(hr));

		// �[�x�X�e���V���r���[�̐ݒ�
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
		dsvDesc.Format			= DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension	= D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Flags			= D3D12_DSV_FLAG_NONE;

		// �[�x�X�e���V���r���[�̍쐬
		g_Device->CreateDepthStencilView(
			g_DepthStencil.Get(),
			&dsvDesc,
			g_DsvHeap->GetCPUDescriptorHandleForHeapStart());
	}

	// ���_�o�b�t�@�̍쐬
	{
		// ���_�f�[�^
		Vertex3D vertices[] =
		{		// position						normal							color					texcoord
			{ { -0.5f, -0.5f, 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f, 0.0f },  { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }, },
			{ {  0.0f,  0.5f, 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f, 0.0f },  { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f }, },
			{ {  0.5f, -0.5f, 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f, 0.0f },  { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f }, }
		};

		// �q�[�v�v���p�e�B�̐ݒ�
		D3D12_HEAP_PROPERTIES prop{};
		prop.Type					= D3D12_HEAP_TYPE_UPLOAD;
		prop.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		prop.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
		prop.CreationNodeMask		= 1;
		prop.VisibleNodeMask		= 1;

		// ���\�[�X�̐ݒ�
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

		// ���\�[�X�̍쐬
		hr = g_Device->CreateCommittedResource(
			&prop,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&g_VertexBuffer));
		assert(SUCCEEDED(hr));

		// �}�b�v
		Vertex3D* pData;
		hr = g_VertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pData));
		assert(SUCCEEDED(hr));

		// ���_�f�[�^�̃R�s�[
		//memcpy(pData, vertices, sizeof(vertices));

		for (int i = 0; i < 3; i++)
		{
			pData[i] = vertices[i];
		}

		// �A���}�b�v
		g_VertexBuffer->Unmap(0, nullptr);

		// ���_�o�b�t�@�r���[�̐ݒ�
		g_VertexBufferView.BufferLocation	= g_VertexBuffer->GetGPUVirtualAddress();
		g_VertexBufferView.StrideInBytes	= sizeof(Vertex3D);
		g_VertexBufferView.SizeInBytes		= sizeof(vertices);
	}

	// �萔�o�b�t�@�p�̃f�B�X�N���v�^�[�q�[�v�̍쐬
	{
		CreateConstantBufferDiscriptorHeap();
	}

	// �萔�o�b�t�@�̍쐬
	{
		CreateConstantBuffer(&g_ConstantBuffer, 256);

		// �萔�o�b�t�@�r���[�̐ݒ�
		D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
		desc.BufferLocation = g_ConstantBuffer->GetGPUVirtualAddress();
		desc.SizeInBytes = 256;

		// �萔�o�b�t�@�r���[�̍쐬
		g_Device->CreateConstantBufferView(&desc, g_CbvHeap->GetCPUDescriptorHandleForHeapStart());

		// �}�b�v (�I�����܂ŃA���}�b�v���Ȃ�)
		hr = g_ConstantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&g_CbvDataBegin));
		assert(SUCCEEDED(hr));

		// �A�X�y�N�g��̎Z�o
		float aspectRatio = static_cast<float>(g_Width) / static_cast<float>(g_Height);

		// �r���[�̐ݒ�
		XMVECTOR eye = { 0.0f, 0.0f, -5.0f };
		XMVECTOR at = { 0.0f, 0.0f,  0.0f };
		XMVECTOR up = { 0.0f, 1.0f,  0.0f };

		// �}�e���A���̐ݒ�
		MATERIAL mat = {};
		mat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		mat.Emission = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		mat.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		mat.Shininess = 0.75f;

		LIGHT light = {};
		light.Enable = false;
		light.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		light.Direction = { 0.0f, -0.3f, -0.7f, 0.0f };

		// �萔�o�b�t�@�f�[�^�̐ݒ�
		/*
		g_ConstantBufferData.World		= XMMatrixIdentity();
		g_ConstantBufferData.View		= XMMatrixLookAtLH(eye, at, up);
		//g_ConstantBufferData.Projection	= XMMatrixPerspectiveLH(static_cast<float>(g_Width), static_cast<float>(g_Height), 1.0f, 1000.0f);
		g_ConstantBufferData.Projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), aspectRatio, 0.1f, 100.0f);
		//g_ConstantBufferData.Material	= mat;
		//g_ConstantBufferData.Light		= light;
		//g_ConstantBufferData.Camera		= {}; // reserved
		*/

		g_ConstantBufferData = XMMatrixIdentity() /* XMMatrixLookAtLH(eye, at, up) * XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), aspectRatio, 0.1f, 100.0f)*/;

		// GPU�̃������փR�s�[
		memcpy(g_CbvDataBegin, &g_ConstantBufferData, sizeof(g_ConstantBufferData));
	}
	
	// ���[�g�V�O�l�`��
	{
		// �f�B�X�N���v�^�����W�̐ݒ�
		//D3D12_DESCRIPTOR_RANGE range{};
		//range.RangeType							= D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		//range.NumDescriptors					= 1;
		//range.BaseShaderRegister				= 0;
		//range.RegisterSpace						= 0;
		//range.OffsetInDescriptorsFromTableStart	= D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		// ���[�g�p�����[�^�̐ݒ�
		D3D12_ROOT_PARAMETER param{};
		//param.ParameterType							= D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		param.ParameterType							= D3D12_ROOT_PARAMETER_TYPE_CBV;
		param.ShaderVisibility						= D3D12_SHADER_VISIBILITY_VERTEX;
		param.Descriptor.ShaderRegister				= 0;
		param.Descriptor.RegisterSpace				= 0;
		//param.DescriptorTable.NumDescriptorRanges	= 1;
		//param.DescriptorTable.pDescriptorRanges		= &range;

		// ���[�g�V�O�l�`���̐ݒ�
		D3D12_ROOT_SIGNATURE_DESC desc{};
		desc.NumParameters		= 1;
		desc.pParameters		= &param;
		desc.NumStaticSamplers	= 0;
		desc.pStaticSamplers	= nullptr;
		desc.Flags  = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
					| D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS
					| D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS
					| D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS
					| D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;

		// �V���A���C�Y
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

	// �p�C�v���C���X�e�[�g
	{
		ComPtr<ID3DBlob> VSBlob;
		ComPtr<ID3DBlob> PSBlob;

		// �V�F�[�_�[�̓ǂݍ���
		hr = D3DReadFileToBlob(L"Asset/Shader/unlitTextureVS.cso", &VSBlob);
		assert(SUCCEEDED(hr));
		hr = D3DReadFileToBlob(L"Asset/Shader/unlitTexturePS.cso", &PSBlob);
		assert(SUCCEEDED(hr));

		// �����ȃ|�C���^��n���Ȃ����߂Ɋ֐����ŕێ�����
		const char position[]	= "POSITION";
		const char normal[]		= "NORMAL";
		const char color[]		= "COLOR";
		const char texCoord[]	= "TEXCOORD";

		// ���̓��C�A�E�g�̐ݒ�
		D3D12_INPUT_ELEMENT_DESC inputElements[] = {
			{ position,	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ normal,	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ color,	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ texCoord,	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};

		// ���X�^���C�U�[�X�e�[�g�̐ݒ�
		D3D12_RASTERIZER_DESC descRS{};
		descRS.FillMode					= D3D12_FILL_MODE_SOLID;
		descRS.CullMode					= D3D12_CULL_MODE_NONE;
		descRS.FrontCounterClockwise	= false;
		descRS.DepthBias				= D3D12_DEFAULT_DEPTH_BIAS;
		descRS.DepthBiasClamp			= D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		descRS.SlopeScaledDepthBias		= D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		descRS.DepthClipEnable			= true;
		descRS.MultisampleEnable		= false;
		descRS.AntialiasedLineEnable	= false;
		descRS.ForcedSampleCount		= 0;
		descRS.ConservativeRaster		= D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

		// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
		D3D12_RENDER_TARGET_BLEND_DESC descRTBS = {
			false, false,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_LOGIC_OP_NOOP,
			D3D12_COLOR_WRITE_ENABLE_ALL
		};

		// �u�����h�X�e�[�g�̐ݒ�
		D3D12_BLEND_DESC descBS{};
		descBS.AlphaToCoverageEnable	= false;
		descBS.IndependentBlendEnable	= false;
		for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		{
			descBS.RenderTarget[i] = descRTBS;
		}

		// �p�C�v���C���X�e�[�g�̐ݒ�
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

		// �p�C�v���C���X�e�[�g�̍쐬
		hr = g_Device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&g_PipelineState));
		assert(SUCCEEDED(hr));
	}
}

void FinalizeDirectX12()
{
	// �R�}���h�̏I����ҋ@
	WaitDrawDone();

	// �C�x���g�n���h�������
	CloseHandle(g_FenceEvent);
	g_FenceEvent = nullptr;

	for (UINT i = 0; i < BUFFER_COUNT; i++)
	{
		g_RenderTargets[i].Reset();
	}

	g_DepthStencil.Reset();

	g_SwapChain.Reset();
	g_Fence.Reset();
	g_CmdList.Reset();
	g_CmdQueue.Reset();
	g_Device.Reset();
}

void BeginDirectX12()
{
	// ��]�p�𑝂₷
	g_RotateAngle += (0.000001f);

	XMVECTOR axis = { 0.0f, 0.0f, 1.0f, 0.0f };

	// ���[���h�s����X�V
	//g_ConstantBufferData.World = XMMatrixRotationAxis(axis, g_RotateAngle);

	// �萔�o�b�t�@���X�V

	XMMATRIX world = XMMatrixIdentity();
	XMMATRIX view = XMMatrixLookAtLH({ 0, 0, -5 }, {}, { 0, 1, 0 });
	XMMATRIX projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), static_cast<float>(g_Width) / static_cast<float>(g_Height), 0.1f, 100.0f);

	g_ConstantBufferData = XMMatrixTranspose(world * view * projection);
	*g_CbvDataBegin = g_ConstantBufferData;

	//memcpy(g_CbvDataBegin, &g_ConstantBufferData, sizeof(g_ConstantBufferData));

	//g_CbvDataBegin->World = g_ConstantBufferData.World;
	//g_CbvDataBegin->View = g_ConstantBufferData.View;
	//g_CbvDataBegin->Projection = g_ConstantBufferData.Projection;


	// �R�}���h�A���P�[�^�ƃR�}���h���X�g�����Z�b�g
	g_CmdAllocator->Reset();
	g_CmdList->Reset(g_CmdAllocator.Get(), g_PipelineState.Get());

	// �f�B�X�N���v�^�q�[�v��ݒ�
	//g_CmdList->SetDescriptorHeaps(1, g_CbvHeap.GetAddressOf());

	// ���[�g�V�O�l�`����ݒ�
	g_CmdList->SetGraphicsRootSignature(g_RootSignature.Get());

	// �p�C�v���C���X�e�[�g�̐ݒ�
	g_CmdList->SetPipelineState(g_PipelineState.Get());

	// �f�B�X�N���v�^�q�[�v�e�[�u����ݒ�
	//g_CmdList->SetGraphicsRootDescriptorTable(0, g_CbvHeap->GetGPUDescriptorHandleForHeapStart());

	// �r���[�|�[�g�̐ݒ�
	g_CmdList->RSSetViewports(1, &g_Viewport);

	// �V�U�[�Z�`
	g_CmdList->RSSetScissorRects(1, &g_ScissorRect);

	// ���\�[�X�o���A�̐ݒ�
	// Present ---> RenderTarget
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type					= D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags					= D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource	= g_RenderTargets[g_FrameIndex].Get();
	barrier.Transition.StateBefore	= D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter	= D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource	= D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	g_CmdList->ResourceBarrier(1, &barrier);

	// �����_�[�^�[�Q�b�g�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE handleRTV = g_RtvHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_CPU_DESCRIPTOR_HANDLE handleDSV = g_DsvHeap->GetCPUDescriptorHandleForHeapStart();
	handleRTV.ptr += ( g_FrameIndex * g_RtvDescriptorHeapSize );

	// �����_�[�^�[�Q�b�g�̐ݒ�
	g_CmdList->OMSetRenderTargets(1, &handleRTV, false, &handleDSV);

	// �����_�[�^�[�Q�b�g�r���[���N���A
	const float clearColor[] = { 0.39f, 0.58f, 0.92f, 1.0f };
	g_CmdList->ClearRenderTargetView(handleRTV, clearColor, 0, nullptr);

	// �[�x�X�e���V���r���[���N���A
	g_CmdList->ClearDepthStencilView(handleDSV, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// �v���~�e�B�u�g�|���W�[�̐ݒ�
	g_CmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �萔�o�b�t�@�̑��M
	g_CmdList->SetGraphicsRootConstantBufferView(0, g_ConstantBuffer->GetGPUVirtualAddress());

	// ���_�o�b�t�@�r���[��ݒ�
	g_CmdList->IASetVertexBuffers(0, 1, &g_VertexBufferView);

	// �`��R�}���h�𐶐�
	g_CmdList->DrawInstanced(3, 1, 0, 0);

	// ���\�[�X�o���A�̐ݒ�
	// RenderTarget ---> Present
	barrier.Transition.StateBefore	= D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter	= D3D12_RESOURCE_STATE_PRESENT;
	g_CmdList->ResourceBarrier(1, &barrier);

	// �R�}���h�̋L�^���I��
	g_CmdList->Close();

	// �R�}���h���s
	ID3D12CommandList* cmdLists[] = { g_CmdList.Get() };
	g_CmdQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);

	// �\������
	g_SwapChain->Present(1, 0);

	// �R�}���h�̊�����ҋ@
	WaitDrawDone();
}

void EndDirectX12()
{

}
