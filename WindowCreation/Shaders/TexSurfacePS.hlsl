
float normfloat3(float3 v)
{
    return sqrt(v.r * v.r + v.g * v.g + v.b * v.b);
}

float dotprd(float3 v0, float3 v1)
{
    return v0.r * v1.r + v0.g * v1.g + v0.b * v1.b;
}

struct Lightsource
{
    float intensity;
    float NEintensity;
    float4 color;
    float3 position;
};

Texture2D tex0 : register(t0);

Texture2D tex1 : register(t1);

SamplerState splr;

float4 main(float2 tc : TexCoord, float3 pos : PointPos) : SV_Target
{
    Lightsource sun;
    sun.intensity = 320.f;
    sun.NEintensity = 100.f;
    sun.color = float4(1.f, 1.f, 1.f, 1.f);
    sun.position = float3(16.f, 0.f, 6.f);
    
    
    float4 totalLight = float4(0.f, 0.f, 0.f, 0.f);
    
    float distance = normfloat3(sun.position - pos);
    float exposure = dotprd(pos, (sun.position - pos) / distance);
    float light = sun.NEintensity / distance / distance;
    if (exposure > 0)
        light += sun.intensity * exposure / distance / distance;
    totalLight += light * sun.color;
    if (exposure>0.1)
        return tex0.Sample(splr, tc) * totalLight;
    else if (exposure<-0.1)
        return tex1.Sample(splr, tc) * totalLight;
    else
        return totalLight * (tex1.Sample(splr, tc) * (0.1 - exposure) + tex0.Sample(splr, tc) * (0.1 + exposure)) * 5;

}