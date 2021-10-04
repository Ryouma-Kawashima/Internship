#pragma once
// -----------------------------------
// Author: Ryouma Kawashima
// Create: 2021/10/04
// Update: 2021/10/04
// -----------------------------------
#include "../utility/params.h"

namespace DragonLib
{
    class Window
    {
    public:
        virtual void Create(uint32_t width, uint32_t height) = 0;
        virtual void Destroy() = 0;
        virtual POINTER GetHandle() = 0;

    public:
        
    };
}
