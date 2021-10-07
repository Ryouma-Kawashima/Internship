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

        void Begin(float r, float g, float b);
        void End();

        void SetCoordSystem(uint32_t cmd);

    private:
        uint32_t m_CoordSystem = CoordSystem_Right;
    };
}
