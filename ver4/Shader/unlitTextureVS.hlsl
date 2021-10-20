
#include "common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{

    //matrix wvp;
    //wvp = mul(World, View);
    //wvp = mul(wvp, Projection);

    Out.Position = mul(float4(In.Position, 1.0f), WVP);
    Out.Normal   = mul(float4(In.Normal,   0.0f), WVP);
    Out.TexCoord = In.TexCoord;
    Out.Diffuse  = In.Diffuse;

}

