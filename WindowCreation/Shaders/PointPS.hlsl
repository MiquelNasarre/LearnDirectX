
cbuffer cBuff0 : register(b0)
{
    float4 color;
}

float4 main() : SV_Target
{
	return color;
}