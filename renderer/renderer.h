#pragma once

namespace DragonLib
{
    class RenderBase {};

    template <class T, bool IsBased = std::is_base_of<RenderBase, T>::value>
    class Renderer
    {
    public:
        void Initialize(Window& window)
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

        void ClearScreen(uint32_t r, uint32_t g, uint32_t b)
        {
            m_Renderer.ClearScreen(r, g, b);
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
