#pragma once

#include "../../System/Public/config.h"

namespace DragonLib
{
    struct ViewPort
    {
        float TopLeftX;
        float TopLeftY;
        float Width;
        float Height;
        float MinDepth;
        float MaxDepth;
    };

    struct Rect
    {
        long Left;
        long Top;
        long Right;
        long Bottom;
    };

    using Handle = void*;

    struct Resource
    {

    };

    using Texture           = Resource;
    using ConstantBuffer    = Resource;
    using VertexBuffer      = Resource;
    using IndexBuffer       = Resource;

    class Renderer
    {
    public:
        virtual bool Initialize(Handle targetWindow, u32 width, u32 height) = 0;
        virtual void Finalize() = 0;

        virtual void Begin()    = 0;
        virtual void End()      = 0;
        virtual void Draw()     = 0;

        virtual void CreateVertexBuffer(void* vertices, size_t elemSize, u32 elemCount) = 0;
        virtual void RemoveVertexBuffer() = 0;

        virtual void CreateIndexBuffer(void* indices, size_t elemSize, u32 elemCount) = 0;
        virtual void RemoveIndexBuffer() = 0;

        virtual void CreateTexture2D(void* textureData, size_t elemSize, u32 elemWidth, u32 elemHeight) = 0;
        virtual void RemoveTexture2D() = 0;
    };
}
