#pragma once

#include "../../System/Public/config.h"
#include "../../Math/Public/math.h"

namespace DragonLib
{
    enum class RHIFormat : u8
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
    };

    struct InputLayout
    {
        const char* SemanticName;
        u32         SemanticIndex;
        RHIFormat      InputFormat;
    };

    using Vertex    = Float3;
    using Normal    = Float3;
    using TexCoord  = Float2;
    using Index     = u32;

    struct Color
    {
        union
        {
            struct
            {
                u8 a, b, g, r; // リトルエンディアンなので反転する
            };
            u32 color;
        };

        void SetColor(u8 r, u8 g, u8 b, u8 a)
        {
            color = (r) | (g << 2) | (b << 4) | (a << 6);
        }
        void SetColor(float r, float g, float b, float a)
        {
            SetColor(static_cast<u8>(r * 255), static_cast<u8>(g * 255), static_cast<u8>(b * 255), static_cast<u8>(a * 255));
        }
    };

    struct Resource
    {
        u8*     bufferLocation;
        size_t  elemSize;
        u32     elemWidthCount;
        u32     elemHeightCount;
    };

    class _declspec(novtable) Renderer
    {
    public:
        virtual bool Initialize(void* targetWindow, u32 width, u32 height) = 0;
        virtual void Finalize() = 0;

        virtual void Begin()    = 0;
        virtual void End()      = 0;
        virtual void Present()  = 0;

        virtual void CreateVertexBuffer(VertexBuffer** out, void* vertices, size_t elemSize, u32 elemCount) = 0;
        virtual void RemoveVertexBuffer(VertexBuffer* in) = 0;
        virtual void SetVertexBuffer(VertexBuffer* in) = 0;

        virtual IndexBuffer* CreateIndexBuffer(void* indices, size_t elemSize, u32 elemCount) = 0;
        virtual void RemoveIndexBuffer(IndexBuffer* in) = 0;
        virtual void SetIndexBuffer(IndexBuffer* in) = 0;

        virtual Texture2D* CreateTexture2D(void* textureData, size_t elemSize, u32 elemWidth, u32 elemHeight) = 0;
        virtual void RemoveTexture2D(Texture2D* in) = 0;
        virtual void SetTexture2D(Texture2D* in) = 0;

        virtual void SetWorldMatrix(Matrix* world) = 0;
        virtual void SetViewMatrix(Matrix* view) = 0;
        virtual void SetProjectionMatrix(Matrix* projection) = 0;
    };
}
