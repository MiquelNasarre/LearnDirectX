
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