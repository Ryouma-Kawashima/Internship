

struct VS_IN
{
    float3 Position : POSITION;
    float3 Normal   : NORMAL;
    float2 TexCoord : TEXCOORD;
    float4 Diffuse  : COLOR;
};

struct PS_IN
{
    float4 Position         : SV_POSITION;
    float4 WorldPosition    : POSITION;
    float4 Normal           : NORMAL;
    float4 Diffuse          : COLOR;
    float2 TexCoord         : TEXCOORD;
};

struct MATERIAL
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float  Shininess;
    float3 Dummy;
};

struct LIGHT
{
    bool   Enable;
    bool3  Dummy;
    float4 Direction;
    float4 Diffuse;
    float4 Ambient;
};

/*
cbuffer ConsntantBuffer : register(b1)
{
    matrix World        : packoffset(c0);
    matrix View         : packoffset(c4);
    matrix Projection   : packoffset(c8);
}
*/

cbuffer WVPBuffer : register(b0)
{
    matrix WVP;
}

/*
cbuffer WorldBuffer : register(b0)
{
    matrix World;
}
cbuffer ViewBuffer : register(b1)
{
    matrix View;
}
cbuffer ProjectionBuffer : register(b2)
{
    matrix Projection;
}
cbuffer MaterialBuffer : register(b3)
{
    MATERIAL Material;
}
cbuffer LightBuffer : register(b4)
{
    LIGHT Light;
}
cbuffer CameraBuffer : register(b5)
{
    float4 CameraPosition;
}
*/
