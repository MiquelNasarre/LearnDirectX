
struct VSOut
{
    float4 color : Color;
    float4 R3pos : PointPos;
    float4 SCpos : SV_Position;
};

cbuffer Cbuff0 : register(b0)
{
    matrix projection;
    float4 center;
};

cbuffer Cbuff1 : register(b1)
{
    float4 translation;
};

VSOut main(float3 pos : Position, float4 col : Color)
{
    VSOut vso;
    vso.R3pos = float4(pos, 1.f) + translation;
    float4 test = mul(vso.R3pos - center, projection);
    vso.SCpos = float4(test.x, test.y, test.z / 10000000.f + 0.5f, 1.f);
    vso.color = col;
    
    return vso;
}