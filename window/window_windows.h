#pragma once
// -----------------------------------
// Author: Ryouma Kawashima
// Create: 2021/10/04
// Update: 2021/10/04
// -----------------------------------
#include "./window_base.h"

#include <windows.h>
#pragma comment(lib, "winmm.lib")

namespace DragonLib
{
    class Window_Windows : public Window
    {
    public:
        void Create(uint32_t width, uint32_t height);
        void Destroy();

    protected:
        Accessor<Window_Windows, POINTER>       Handle      = nullptr;
        Accessor<Window_Windows, std::string>   WindowName  = std::string(WINDOW_NAME);
        Accessor<Window_Windows, std::string>   ClassName   = std::string(APPCLASS_NAME);
    };
}
