#pragma once

#include "../Common/rhi.h"
#include "device.h"

namespace DragonLib
{
    class RHI<RHIRenderType::DIRECTX11>
    {
        static bool CreateDevice(RHIDevice** ppDevice, RHICommandBuffer** ppCommandBuffer)
        {
            
            return true;
        }

        static bool CreateDeviceAndSwapChain()
        {

        }
    };
}
