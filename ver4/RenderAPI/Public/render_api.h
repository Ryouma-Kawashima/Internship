#pragma once

#if RENDERER_TYPE == DIRECTX_12

#include "../Private/directx12.h"
namespace DragonLib 
{
    using RenderAPI = DirectX12;
}

#elif RENDERER_TYPE == DIRECTX_11

#include "../Private/directx11.h"
namespace DragonLib
{
    using RenderAPI = DirectX11;
}

#endif
