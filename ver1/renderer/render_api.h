#pragma once

#include "renderer.h"

#if GRAPHIC_DIRECTX11

#include "../renderer/directx11.h"

#elif GRAPHIC_DIRECTX12

#include "../renderer/directx12.h"

#elif GRAPHIC_OPENGL

#include "../renderer/opengl.h"

#elif GRAPHIC_VULKAN

#include "../renderer/vulkan.h"

#endif

namespace DragonLib
{
    #if GRAPHIC_DIRECTX11

    typedef Renderer<DirectX11> RenderAPI;

    #elif GRAPHIC_DIRECTX12

    typedef Renderer<DirectX12> RenderAPI;

    #elif GRAPHIC_OPENGL

    typedef Renderer<OpenGL> RenderAPI;

    #elif GRAPHIC_VULKAN

    typedef Renderer<Vulkan> RenderAPI;

    #endif
}
