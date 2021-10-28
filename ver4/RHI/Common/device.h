#pragma once

#include "util.h"

namespace DragonLib
{
    class RHICommandBuffer;
    class RHIRenderTargetView;
    class RHIDepthStencilView;
    class RHISwapChain;
    class RHIResource;
    class RHIFence;
    class RHIRasterizerState;

    struct RHIDevice
    {
        virtual bool CreateDepthStencilView(RHIDepthStencilDesc& desc, RHIDepthStencilView** ppDepthStencilView) = 0;
        virtual bool CreateRenderTargetView(RHIRenderTargetDesc& desc, RHIRenderTargetView** ppRenderTargetView) = 0;
        virtual bool CreateSwapChain(RHISwapChainDesc& desc, RHISwapChain** ppSwapChain) = 0;
        virtual bool CreateRasterizerState(RHIRasterizerStateDesc& desc, RHIRasterizerState** ppRasterizerState) = 0;
        virtual bool CreateFence(RHIFence** ppFence) = 0;
    };
}
