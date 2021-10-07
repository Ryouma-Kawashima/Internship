#pragma once
// ------------------------------
// Author: Ryouma Kawashima
// Create: 2021/10/07
// Update: 2021/10/07
// ------------------------------
// Description
// 
// ------------------------------

#include "render_base.h"

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
        void Initialize(WindowAPI* window)
        {
            m_Renderer.Initialize(window);
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
    using RenderAPI = Render<DirectX11>;
    #elif USE_DIRECTX12
    using RenderAPI = Render<DirectX12>;
    #endif
}