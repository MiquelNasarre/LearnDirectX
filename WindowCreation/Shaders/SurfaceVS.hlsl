#include "Quaternion.hlsli"

cbuffer Cbuff0 : register(b0)
{
    matrix projection;
    float4 center;
};

cbuffer Cbuff1 : register(b1)
{
    float4 traslation;
    float4 quaternion;
};

struct VSOut
{
    float4 color : Color;
    float4 R3pos : PointPos;
    float4 norm : Norm;
    float4 SCpos : SV_Position;
};

VSOut main(float3 pos : Position, float3 norm : Normal, float4 color : Color)
{
    VSOut vso;
    
    vso.norm = Q2V(qRot(quaternion, float4(0, norm)));
    vso.R3pos = Q2V(qRot(quaternion, float4(0, pos))) + traslation;
    
    float4 test = mul(vso.R3pos - center, projection);
    vso.SCpos = float4(test.x, test.y, test.z / 10000000.f + 0.5f, 1.f);
    vso.color = color;
    
    return vso;
}