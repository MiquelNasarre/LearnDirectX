
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
    float2 tex : TexCoord;
    float4 SCpos : SV_Position;
};

VSOut main(float3 pos : Position, float2 tex : TexCoord)
{
    VSOut vso;
    float4 R3 = mul(float4(pos, 1.f), rotation) + traslation;
    float4 test = mul(R3 - center, projection);
    vso.SCpos = float4(test.x, test.y, test.z / 10000000.f + 0.5f, 1.f);
    vso.tex = tex;
    return vso;
}