#pragma once

#include "../Common/rhi.h"
#include "device.h"

namespace DragonLib
{
    class RHI<RHIRenderType::DIRECTX12>
    {
        static bool CreateDevice(RHIDevice** ppDevice, RHICommandBuffer** ppCommandBuffer)
        {
            return true;
        }
    };
}
