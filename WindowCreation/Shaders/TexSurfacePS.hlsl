
Texture2D tex0 : register(t0);

Texture2D tex1 : register(t1);

SamplerState splr;

struct Lightsource
{
    float2 intensity;
    //<-- theres a hidden float2 in here just for dramatic purpose
    float4 color;
    float3 position;
    //<-- and a float here as well!!
};

cbuffer cBuff : register(b0)
{
    Lightsource lights[8];
};

cbuffer cBuffc : register(b1)
{
    float4 color;
};

float4 main(float2 tc : TexCoord, float3 pos : PointPos, float3 norm : Norm, bool front : SV_IsFrontFace) : SV_Target
{
    if (!front)
        norm = -norm;
    
    float4 totalLight = float4(0.f, 0.f, 0.f, 0.f);
    float dist = 0;
    float exposure = 0;
    float maxexposure = -1;
    
    for (int i = 0; i < 8; i++)
    {
        if (!lights[i].intensity.r && !lights[i].intensity.g)
            continue;
        
        dist = distance(lights[i].position, pos);
        exposure = dot(norm, (lights[i].position - pos) / dist);
    
        float light = lights[i].intensity.g / dist / dist;
        if (exposure > 0)
            light += lights[i].intensity.r * exposure / dist / dist;
        totalLight += light * lights[i].color, 1.f;
        
        if (exposure > maxexposure && lights[i].intensity.r)
            maxexposure = exposure;
    }
    
    if (maxexposure > 0.1)
        return tex0.Sample(splr, tc) * totalLight;
    else if (maxexposure < -0.1)
        return tex1.Sample(splr, tc) * totalLight;
    else
        return totalLight * color * (tex1.Sample(splr, tc) * (0.1 - maxexposure) + tex0.Sample(splr, tc) * (0.1 + maxexposure)) * 5;
}