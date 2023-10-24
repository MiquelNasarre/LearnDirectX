
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

float4 main(float3 pos : PointPos, float3 norm : Norm) : SV_Target
{
    Lightsource lights[4];
    lights[0].intensity = 60.f;
    lights[0].NEintensity = 0.f;
    lights[0].color = float4(1.f, 0.2f, 0.2f, 1.f);
    lights[0].position = float3(0.f, 8.f, 8.f);
    
    lights[1].intensity = 60.f;
    lights[1].NEintensity = 0.f;
    lights[1].color = float4(0.f, 1.f, 0.f, 1.f);
    lights[1].position = float3(0.f, -8.f, 8.f);
    
    lights[2].intensity = 60.f;
    lights[2].NEintensity = 0.f;
    lights[2].color = float4(0.5f, 0.f, 1.f, 1.f);
    lights[2].position = float3(-8.f, 0.f, -8.f);
    
    lights[3].intensity = 60.f;
    lights[3].NEintensity = 0.f;
    lights[3].color = float4(1.f, 1.f, 0.f, 1.f);
    lights[3].position = float3(8.f, 0.f, -8.f);
    
    
    float4 totalLight = float4(0.f, 0.f, 0.f, 0.f);
    
    for (int i = 0; i < 4; i++)
    {
        float distance = normfloat3(lights[i].position - pos);
        float exposure = dotprd(norm, (lights[i].position - pos) / distance);
        float light = lights[i].NEintensity / distance / distance;
        if (exposure > 0)
            light += lights[i].intensity * exposure / distance / distance;
        totalLight += light * lights[i].color;
    }
    
    return totalLight;
}