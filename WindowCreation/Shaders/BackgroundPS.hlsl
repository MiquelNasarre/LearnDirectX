
Texture2D tex : register(t0);

SamplerState splr;

float4 main(float2 tc : TexCoord) : SV_Target
{
    float4 col = tex.Sample(splr, tc);
    return col * col.r * col.g / 2.f;
}