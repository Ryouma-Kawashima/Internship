#pragma once

namespace DragonLib
{
    class RenderBase {};

    template <class T, bool IsBased = std::is_base_of<RenderBase, T>::value>
    class Renderer
    {
    public:
        void Initialize(Window* window)
        {
            m_Renderer.Initialize(window);
        }
        void Finalize()
        {
            m_Renderer.Finalize();
        }

        void Begin(float r = BG_COLOR_R, float g = BG_COLOR_G, float b = BG_COLOR_B)
        {
            m_Renderer.Begin(r, g, b);
        }
        void End()
        {
            m_Renderer.End();
        }

        void SetCoordSystem(uint32_t cmd)
        {
            if (cmd == CoordSystem_Left || cmd == CoordSystem_Right)
            {
                m_CoordSystem = cmd;
            }
        }

    private:
        uint32_t m_CoordSystem = COORD_SYSTEM;
        T m_Renderer;
    };
}
