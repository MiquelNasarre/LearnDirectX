
struct VSOut {
	float4 color : Color;
	float4 position : SV_Position;
};

VSOut main(float2 pos : Position, float4 col : Color)
{
	VSOut vso;
	vso.position = float4(pos.x, pos.y, 0.f, 1.f);
	vso.color = col;
	return vso;
}