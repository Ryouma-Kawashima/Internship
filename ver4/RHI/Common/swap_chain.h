#pragma once

#include "util.h"

namespace DragonLib
{
    struct RHISwapChain
    {
        virtual uint GetCurrentBackBufferIndex() = 0;
    };
}
