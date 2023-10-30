
struct VSOut
{
    float4 color : Color;
    float4 SCpos : SV_Position;
};

cbuffer cBuff0 : register(b0)
{
    matrix projection;
    float4 center;
};

cbuffer cBuff1 : register(b1)
{
    matrix rotation;
    float4 traslation;
}

VSOut main( float4 pos : Position, float4 color : Color)
{
    VSOut vso;
    vso.color = color;
    float4 R3Pos = mul(pos, rotation) + traslation;
    float4 test = mul(R3Pos - center, projection);
    vso.SCpos = float4(test.x, test.y, test.z / 10000000.f + 0.5, 1.f);
	return vso;
}