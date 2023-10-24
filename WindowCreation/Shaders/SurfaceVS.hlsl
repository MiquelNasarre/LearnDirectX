
struct VSOut
{
    float4 R3pos : PointPos;
    float4 Norm : Norm;
    float4 SCpos : SV_Position;
};

cbuffer Cbuff0 : register(b0)
{
    matrix projection;
    float4 center;
};

cbuffer Cbuff1 : register(b1)
{
    matrix rotation;
};

VSOut main(float3 pos : Position, float3 norm : Normal)
{
    VSOut vso;
    vso.R3pos = mul(float4(pos, 1.f), rotation);
    float4 test = mul(vso.R3pos - center, projection);
    vso.SCpos = float4(test.x, test.y, test.z / 10000000.f + 0.5f, 1.f);
    vso.Norm = mul(float4(norm, 1.f), rotation);
    
    return vso;
}