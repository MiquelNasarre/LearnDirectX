
struct VSOut
{
    float4 R3pos : PointPos;
    float3 Norm : Norm;
    float4 SCpos : SV_Position;
};

cbuffer Cbuff0 : register(b0)
{
    matrix projection;
};

VSOut main(float3 pos : Position, float3 norm : Normal)
{
    VSOut vso;
    vso.R3pos = float4(pos, 1.f);
    float4 test = mul(vso.R3pos, projection);
    vso.SCpos = float4(test.x, test.y, test.z / 1000000.f + 0.5f, 1.f);
    vso.Norm = norm;
    
    return vso;
}