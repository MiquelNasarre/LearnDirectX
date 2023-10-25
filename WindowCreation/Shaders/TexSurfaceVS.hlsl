
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
    float4 R3pos : PointPos;
    float4 center : Center;
    float4 Scpos : SV_Position;
};

VSOut main(float3 pos : Position, float2 tex : TexCoord)
{
    VSOut vso;
    vso.R3pos = mul(float4(pos, 1.f), rotation) + traslation;
    float4 test = mul(vso.R3pos - center, projection);
    vso.Scpos = float4(test.x, test.y, test.z / 10000000.f + 0.5f, 1.f);
    vso.tex = tex;
    vso.center = traslation;
    return vso;
}