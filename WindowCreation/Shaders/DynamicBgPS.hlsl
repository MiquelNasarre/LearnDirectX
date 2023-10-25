
Texture2D tex : register(t0);

SamplerState splr;

float3 vectorProd(float3 v0, float3 v1)
{
    return float3(v0.g * v1.b - v1.g * v0.b, v0.b * v1.r - v1.b * v0.r, v0.r * v1.g - v1.r * v0.g);
}

cbuffer cBuff : register(b0)
{
    float3 obs;
}

cbuffer cBuff : register(b1)
{
    float2 wide;
}

float4 main(float2 tc : TexCoord) : SV_Target
{
    float theta0 = 0.8f / wide.r;
    float phi0 = 0.5f / wide.g;
    
    float theta = -theta0 + 2 * theta0 * tc.r;
    float phi = -phi0 + 2 * phi0 * tc.g;
    
    const float3 ei = float3(obs.g, -obs.r, 0.f) / sqrt(1 - obs.b * obs.b);
    const float3 ej = vectorProd(ei, obs);
    
    const float costh = cos(theta);
    const float cosph = cos(phi);
    const float sinth = sin(theta);
    const float sinph = sin(phi);
    
    const float3 vp = obs * costh * cosph + ei * sinth * cosph + ej * sinph;
    
    float phif = asin(vp.b);
    
    float thetaf = 0.f;
    if (vp.g>=0 && vp.r > 0)
        thetaf = atan(vp.g / vp.r);
    else if (vp.g>=0 && vp.r < 0)
        thetaf = 3.14159f - atan(-vp.g / vp.r);
    else if (vp.g<0 && vp.r < 0)
        thetaf = 3.14159f + atan(vp.g / vp.r);
    else if (vp.g<0 && vp.r > 0)
        thetaf = 2.f * 3.14159f - atan(-vp.g / vp.r);
    else if (vp.r == 0)
    {
        if (vp.g>=0)
            theta = 3.14159f / 2.f;
        else
            theta = 3 * 3.14159f / 2.f;
    }
    
    return tex.Sample(splr, float2(thetaf / 2.f / 3.14159f, (3.14159f / 2.f - phif) / 3.14159f));
}