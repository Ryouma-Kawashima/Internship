#include "../utility/param.h"
#include "../system/config.h"
#include "../system/window.h"

#include "../renderer/renderer.h"

#include "opengl.h"

using namespace DragonLib;

void OpenGL::Initialize(Window& window)
{
    UNREFERENCED_PARAMETER(window);
}
void OpenGL::Finalize()
{

}

void OpenGL::Begin(float r, float g, float b)
{
    UNREFERENCED_PARAMETER(r);
    UNREFERENCED_PARAMETER(g);
    UNREFERENCED_PARAMETER(b);
}
void OpenGL::End()
{

}

void OpenGL::SetCoordSystem(uint32_t cmd)
{
    if (cmd == CoordSystem_Left || cmd == CoordSystem_Right)
    {
        m_CoordSystem = cmd;
    }
}

