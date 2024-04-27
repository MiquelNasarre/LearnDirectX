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
    float4 SCpos : SV_Position;
};

VSOut main(float3 pos : Position, float4 color : Color)
{
    VSOut vso;
    vso.color = color;
    float4 R3Pos = Q2V(qRot(quaternion, float4(0, pos))) + traslation;
    float4 test = mul(R3Pos - center, projection);
    vso.SCpos = float4(test.x, test.y, test.z / 10000000.f + 0.5, 1.f);
    return vso;
}