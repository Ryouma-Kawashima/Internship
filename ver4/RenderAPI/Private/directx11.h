#pragma once

#include <wrl/client.h>
#include <d3d11.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "../Public/common.h"
#include "windows.h"

namespace DragonLib
{
    class DirectX11 : public Renderer
    {

    };
}
