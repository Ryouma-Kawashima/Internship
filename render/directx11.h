#pragma once

#include "render.h"

#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace DragonLib
{
    class DirectX11 : public RenderBase
    {
    public:
        void Initialize();
        void Finalize();

        void ClearScreen(uint32_t r, uint32_t g, uint32_t b);
    };
}

