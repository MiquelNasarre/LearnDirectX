
struct VSOut
{
    float intensity : Intensity;
    float4 SCpos : SV_Position;
};

cbuffer cBuff0 : register(b0)
{
    matrix projection;
    float4 center;
}

cbuffer cBuff1 : register(b1)
{
    float4 position;
    float4 observer;
    float radius;
}

VSOut main(float2 pos : Reference, float intensity : Intensity)
{
    float4 e1 = normalize(float4(observer.y, -observer.x, 0.f, 0.f));
    float4 e2 = normalize(float4(-observer.x * observer.z, -observer.z * observer.y, observer.y * observer.y + observer.x * observer.x, 0.f));
    
    VSOut vso;
    vso.intensity = intensity;
    float4 test = mul(position + (e1 * pos.x + e2 * pos.y) * radius - center, projection);
    vso.SCpos = float4(test.x, test.y, test.z / 10000000.f + 0.5f, 1.f);
    
    return vso;
}