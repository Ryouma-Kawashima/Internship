#pragma once

#include "../../Utility/Public/parameter.h"

namespace DragonLib
{
    enum class RHIRenderType : uint
    {
        DIRECTX11,
        DIRECTX12,
    };

    enum class RHIFormat : uint
    {
        R8,
        R8G8,
        R8G8B8,
        R8G8B8A8,

        R16,
        R16G16,
        R16G16B16,
        R16G16B16A16,

        R32,
        R32G32,
        R32G32B32,
        R32G32B32A32,

        R32_UINT,
        R32G32_UINT,
        R32G32B32_UINT,
        R32G32B32A32_UINT,

        R32_FLOAT,
        R32G32_FLOAT,
        R32G32B32_FLOAT,
        R32G32B32A32_FLOAT,

        D16,
        D32,
    };

    enum class RHISwapEffect : uint
    {
        DISCARD,
        SEQUENTIAL,
        FLIP_SEQUENTIAL,
        FLIP_DISCARD,
    };

    enum class RHIScanlineOrder : uint
    {
        UNSPECIFIED,
        PROGRESSIVE,
        UPPER_FIELD_FIRST,
        LOWER_FIELD_FIRST,
    };

    enum class RHIScaling : uint
    {
        UNSPECIFIED,
        CENTERED,
        STRETCHED,
    };

    enum class RHIUsage : uint
    {
        DEFAULT,
        IMMUTABLE,
        DYNAMIC,
        STAGING,
    };

    enum class RHIFillMode : uint
    {
        WIREFRAME,
        SOLID,
    };

    enum class RHICullMode : uint
    {
        NONE,
        FRONT,
        BACK,
    };

    struct RHIUnknown
    {
        virtual void Release() = 0;
    };

    struct RHIDeviceChild : public RHIUnknown
    {
        virtual void Reset() = 0;
    };

    struct RHIViewPort
    {
        float TopLeftX;
        float TopLeftY;
        float Width;
        float Height;
        float MinDepth;
        float MaxDepth;
    };

    struct RHIRect
    {
        long Left;
        long Right;
        long Top;
        long Bottom;
    };

    struct RHIRational
    {
        uint Numerator;
        uint Denominator;
    };

    struct RHIModeDesc
    {
        uint                Width;
        uint                Height;
        RHIRational         RefreshRate;
        RHIFormat           Format;
        RHIScanlineOrder    ScanlineOrdering;
        RHIScaling          Scaling;
    };

    struct RHISampleDesc
    {
        uint Count;
        uint Quality;
    };

    struct RHISwapChainDesc
    {
        RHIModeDesc     BufferDesc;
        RHISampleDesc   SampleDesc;
        uint            BufferUsage;
        uint            BufferCount;
        Handle          OutputWindow;
        RHISwapEffect   SwapEffect;
        bool            Windowed;
        uint            Flags;
    };

    struct RHIResource
    {

    };

    struct RHITexture2DDesc
    {
        uint            Width;
        uint            Height;
        uint            MipLevels;
        uint            ArraySize;
        RHIFormat       Format;
        RHISampleDesc   SampleDesc;
        RHIUsage        Usage;
        uint            BindFlags;
        uint            CPUAccessFlags;
        uint            MiscFlags;
    };

    struct RHITexture2D : public RHIResource
    {

    };

    struct RHIRasterizerStateDesc
    {
        RHIFillMode     FillMode;
        RHICullMode     CullMode;
        bool            FrontCounterClockwise;
        int             DepthBias;
        float           DepthBiasClamp;
        float           SlopeScaledDepthBias;
        bool            DepthClipEnable;
        bool            ScissorEnable;
        bool            MultisampleEnable;
        bool            AntialiasedLineEnable;
    };

    struct RHIBufferDesc
    {
        uint        ByteWidth;
        RHIUsage    Usage;
        uint        BindFlags;
        uint        CPUAccessFlags;
        uint        MiscFlags;
        uint        StructureByteStride;
    };

    struct RHIInputLayoutDesc
    {
        const char* SemanticName;
        uint        SemanticIndex;
        RHIFormat   Format;
    };

    struct RHIDepthStencilDesc
    {

    };

    struct RHIRenderTargetDesc
    {

    };
}
