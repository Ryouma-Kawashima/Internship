#pragma once

#include "../utility/static_polymorphism.h"

namespace DragonLib
{
    class RenderBase {};

    template <class T, bool IsBased = std::is_base_of<RenderBase, T>::value>
    class Render
    {
        static_assert(IsBased, "T is not base of RenderBase");
    };

    template <class T>
    class Render<T, true>
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

        // r,g,b: 0 ~ 255
        void inline ClearScreen(uint32_t r, uint32_t g, uint32_t b)
        {
            SP_CAST(T)->ClearScreen(r, g, b);
        }
    };
}
