
struct VSOut {
	float4 color : Color;
    float4 R3pos : PointPos;
	float4 SCpos : SV_Position;
};

cbuffer Cbuff
{
    float4 translation;
    matrix rotation;
    matrix projection;
};

VSOut main(float3 pos : Position, float4 col : Color)
{
	VSOut vso;
    vso.R3pos = mul(float4(pos, 1.f), rotation) + translation;
    float4 test = mul(vso.R3pos, projection);
    vso.SCpos = float4(test.x, test.y, test.z / 1000000.f + 0.5f, 1.f);
	vso.color = col;
    
	return vso;
}