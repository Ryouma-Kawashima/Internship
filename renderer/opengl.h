#pragma once

#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "opengl32.lib")

namespace DragonLib
{
    class OpenGL : public RendererBase
    {
    public:
        void Initialize();
        void Finalize();

        void ClearScreen(uint32_t r, uint32_t g, uint32_t b);
    };
}
