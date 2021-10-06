#pragma once

#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "opengl32.lib")

namespace DragonLib
{
    class OpenGL : public RenderBase
    {
    public:
        void Initialize(Window& window);
        void Finalize();

        void Begin();
        void End();

        void ClearScreen(uint32_t r, uint32_t g, uint32_t b);

        
    };
}
