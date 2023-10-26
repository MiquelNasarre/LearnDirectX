
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
    float2 coord = float2((2 * svPos.r / windowDim.r - 1.f) * windowDim.r / windowDim.g, 1.f - 2.f * svPos.g / windowDim.g) * windowDim.g / 1400.f;
    
    const float4 dir = normalize(wide * coord.r * ei + wide * coord.g * zp);
    
    const float angle = sqrt(wide * wide * coord.r * coord.r + wide * wide * coord.g * coord.g);
    const float test = angle * (40.f - angle) / 40.f;
    
    float4 vp = obs * cos(test) + dir * sin(test);
    
    if(vp.b<0)
        return tex.Sample(splr, float2(0.75f + (0.125f * vp.r) * (2 + vp.b), 0.5f + (0.25f * vp.g) * (2.f + vp.b)));
    
    return tex.Sample(splr, float2(0.25f + (0.125f * vp.r) * (2 - vp.b), 0.5f + (0.25f * vp.g) * (2.f - vp.b)));

}