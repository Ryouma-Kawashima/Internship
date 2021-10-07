#pragma once

#include "../window/window_api.h"
#include "../render/render_api.h"

namespace DragonLib
{
    class Manager
    {
    public:
        void Initialize();
        void Finalize();
        void Physics();
        void Update();
        void Draw();
        void Debug();

    private:
        WindowAPI* m_WindowAPI;
        RenderAPI* m_RenderAPI;
    };
}
