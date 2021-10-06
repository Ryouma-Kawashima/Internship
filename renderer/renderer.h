#pragma once

#if defined ( ENGINE_DIRECTX11 )
#include "../renderer/directx11.h"
#elif defined ( ENGINE_DIRECTX12 )
#include "../renderer/directx12.h"
#elif defined ( ENGINE_OPENGL )
#include "../renderer/opengl.h"
#elif defined ( ENGINE_VULKAN )
#include "../renderer/vulkan.h"
#endif

namespace DragonLib
{
    template <class T, bool IsBased = std::is_base_of<RendererBase, T>::value>
    class Renderer
    {
        static_assert(IsBased, "T is not base of RenderBase");
    };

    template <class T>
    class Renderer<T, true>
    {
    public:
        void inline Initialize()
        { 
            SP_CAST(T)->Initialize(); 
        }
        void inline Finalize()
        {
            SP_CAST(T)->Finalize();
        }

        void inline ClearScreen(uint32_t r, uint32_t g, uint32_t b)
        {
            SP_CAST(T)->ClearScreen(r, g, b);
        }

        void inline SetCoordSystem(uint32_t cmd)
        {
            m_CoordSystem = COORD_SYSTEM;
        }

    private:
        uint32_t m_CoordSystem = COORD_SYSTEM;
    };

    #if defined ( ENGINE_DIRECTX11 )
    typedef Renderer<DirectX11> RenderAPI;
    #elif defined ( ENGINE_DIRECTX12 )
    typedef Renderer<DirectX12> RenderAPI;
    #elif defined ( ENGINE_OPENGL )
    typedef Renderer<OpenGL> RenderAPI;
    #elif defined ( ENGINE_VULKAN )
    typedef Renderer<Vulkan> RenderAPI;
    #endif
}
