
cbuffer Cbuff
{
    float4 norm[12];
};

float normfloat3(float3 v)
{
    return sqrt(v.r * v.r + v.g * v.g + v.b * v.b);
}

float dotprd(float3 v0, float3 v1)
{
    return v0.r * v1.r + v0.g * v1.g + v0.b * v1.b;
}

float4 main(float4 color : Color, float4 pos : PointsPos, uint tid : SV_PrimitiveID) : SV_Target
{
    
    float3 lightsource = float3(0.f, 1.f, 1.f);
    float lightintensity = 1.5f;
    float noexposurelightintensity = 1.f;
    float distance = normfloat3(lightsource - (float3)pos);
    float exposure = dotprd((float3)norm[tid], lightsource / normfloat3(lightsource));
    
    float light = noexposurelightintensity / distance / distance;
    if(exposure > 0)
        light += lightintensity * exposure / distance / distance;
    
    float ambientlight = 0.3f;
    
    return (norm[tid] + float4(1.f, 1.f, 1.f, 0.f)) / 2; //float4(0.3f,0.3f,0.3f,0.f) * (light + ambientlight);

}