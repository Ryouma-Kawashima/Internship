#pragma once

#include "util.h"

namespace DragonLib
{
    class RHISwapChain;

    struct RHIFence : public RHIDeviceChild
    {
        virtual bool CreateSwapChain(RHISwapChain** ppSwapChain) = 0;
    };
}
