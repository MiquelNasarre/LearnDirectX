
struct VSOut
{
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
    float radius;
    float scale;
}

VSOut main(float4 norm : Normal)
{
    
    VSOut vso;
    float4 test = mul(position + norm * radius / scale - center, projection);
    vso.SCpos = float4(test.x, test.y, test.z / 10000000.f + 0.5f, 1.f);
    
    return vso;
}