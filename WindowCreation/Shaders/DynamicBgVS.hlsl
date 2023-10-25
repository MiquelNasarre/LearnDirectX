
struct VSOut
{
    float2 tex : TexCoord;
    float4 pos : SV_Position;
};

VSOut main(float2 pos : Position, float2 tex : TexCoord)
{
    VSOut vso;
    vso.tex = tex;
    vso.pos = float4(pos, 1.f, 1.f);
    return vso;
}