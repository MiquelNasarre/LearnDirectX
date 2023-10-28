
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

    float phif = asin(vp.b);
    
    float thetaf = 0.f;
    if (vp.g >= 0 && vp.r > 0)
        thetaf = atan(vp.g / vp.r);
    else if (vp.g >= 0 && vp.r < 0)
        thetaf = 3.14159f - atan(-vp.g / vp.r);
    else if (vp.g < 0 && vp.r < 0)
        thetaf = 3.14159f + atan(vp.g / vp.r);
    else if (vp.g < 0 && vp.r > 0)
        thetaf = 2.f * 3.14159f - atan(-vp.g / vp.r);
    else if (vp.r == 0)
    {
        if (vp.g >= 0)
            thetaf = 3.14159f / 2.f;
        else
            thetaf = 3 * 3.14159f / 2.f;
    }
    
    return tex.Sample(splr, float2(thetaf / 2.f / 3.14159f, -(3.14159f / 2.f - phif) / 3.14159f));
}