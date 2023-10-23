
cbuffer Cbuff
{
    float4 norm[24];
};

float normfloat3(float3 v)
{
    return sqrt(v.r * v.r + v.g * v.g + v.b * v.b);
}

float dotprd(float3 v0, float3 v1)
{
    return v0.r * v1.r + v0.g * v1.g + v0.b * v1.b;
}

float4 main(float4 color : Color, float4 pos : PointPos, uint tid : SV_PrimitiveID) : SV_Target
{
    float lightintensity            = 2.f;
    float noexposurelightintensity  = 2.f;
    float ambientlight              = 0.0f;
    
    float3 lightsource = float3(0.f, 2.f, 2.f);
    
    float distance = normfloat3(lightsource - (float3)pos);
    float exposure = dotprd((float3)norm[tid], (lightsource - (float3)pos) / distance);
    
    float light = noexposurelightintensity / distance / distance;
    if(exposure > 0)
        light += lightintensity * exposure / distance / distance;
    
    return color * (light + ambientlight);

}