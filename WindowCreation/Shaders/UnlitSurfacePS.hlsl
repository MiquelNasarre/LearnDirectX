
Texture2D tex : register(t0);

SamplerState splr;

cbuffer cBuffc : register(b1)
{
    float4 color;
};

float4 main(float2 tc : TexCoord) : SV_Target
{
    return color * tex.Sample(splr, tc);
}