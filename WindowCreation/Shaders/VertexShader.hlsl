
struct VSOut {
	float4 color : Color;
    float4 R3pos : PointPos;
	float4 SCpos : SV_Position;
};

cbuffer Cbuff
{
    matrix movement;
    matrix perspective;
};

VSOut main(float3 pos : Position, float4 col : Color)
{
	VSOut vso;
    vso.R3pos = mul(float4(pos, 1.f), movement);
    float4 test = mul(vso.R3pos, perspective);
    vso.SCpos = float4(test.x, test.y, 0.f, 1.f);
	vso.color = col;
    
	return vso;
}