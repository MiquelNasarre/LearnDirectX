
Texture2D tex : register(t0);

SamplerState splr;

cbuffer cBuff : register(b0)
{
    float4 obs;
    float4 ei;
    float4 zp;
}

cbuffer cBuff : register(b1)
{
    float wide;
    float2 windowDim;
}

float4 main(float4 svPos : SV_Position) : SV_Target
{   
    float2 coord = float2(2.f * svPos.r - windowDim.r, windowDim.g - 2.f * svPos.g) / 1400.f;
    
    float4 vp = normalize(obs * wide + ei * coord.r + zp * coord.g);
    
    if(vp.b<0)
        return tex.Sample(splr, float2(0.75f + (0.125f * vp.r) * (2 + vp.b), 0.5f + (0.25f * vp.g) * (2.f + vp.b)));
    
    return tex.Sample(splr, float2(0.25f + (0.125f * vp.r) * (2 - vp.b), 0.5f + (0.25f * vp.g) * (2.f - vp.b)));

}