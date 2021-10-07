#pragma once
// ------------------------------
// Author: Ryouma Kawashima
// Create: 2021/10/07
// Update: 2021/10/07
// ------------------------------
// Description
// 
// ------------------------------

#include "../utility/static_polymorphism.h"
#include "../utility/parameter.h"
#include "../system/config.h"

#include "render_base.h"

#include "../window/window_api.h"

#if USE_DIRECTX11
#include "directx11.h"
#elif USE_DIRECTX12
#include "directx12.h"
#endif

namespace DragonLib
{
    template
        <
        class T,
        bool IsBased = std::is_base_of_v<RenderBase, T>
        >
        class Render
    {
    public:
        void Initialize()
        {
            m_Renderer.Initialize(WindowAPI* window);
        }
        void Finalize()
        {
            m_Renderer.Finalize();
        }

        void Begin()
        {
            m_Renderer.Begin();
        }
        void End()
        {
            m_Renderer.End();
        }

    private:
        T m_Renderer;
    };

    #if USE_DIRECTX11
    typedef Render<DirectX11> RenderAPI;
    #elif USE_DIRECTX12
    typedef Render<DirectX12> RenderAPI;
    #endif
}