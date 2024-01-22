
cbuffer cBuff : register(b0)
{
    float4 color;
}

float4 main(float intensity : Intensity) : SV_Target
{
    return float4(color.r, color.g, color.b, color.a * intensity);
}