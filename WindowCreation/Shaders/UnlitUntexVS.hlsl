
cbuffer Cbuff0 : register(b0)
{
    matrix projection;
    float4 center;
};

cbuffer Cbuff1 : register(b1)
{
    matrix rotation;
    float4 traslation;
};

struct VSOut
{
    float4 color : Color;
    float4 SCpos : SV_Position;
};

VSOut main(float3 pos : Position, float3 norm : Normal, float4 color : Color)
{
    VSOut vso;
    float4 R3 = mul(float4(pos, 1.f), rotation) + traslation;
    float4 test = mul(R3 - center, projection);
    vso.SCpos = float4(test.x, test.y, test.z / 10000000.f + 0.5f, 1.f);
    vso.color = color;
    return vso;
}