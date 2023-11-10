
cbuffer cBuff : register(b1)
{
    float4 tex;
}

struct VSOut
{
    float2 tex : TexCoord;
    float4 pos : SV_Position;
};

VSOut main(float2 pos : Position)
{
    VSOut vso;
    
    if (pos.x == -1.f)
        vso.tex.x = tex.r;
    else 
        vso.tex.x = tex.b;
    
    if (pos.y == 1.f)
        vso.tex.y = tex.g;
    else
        vso.tex.y = tex.a;
    
    vso.pos = float4(pos, 1.f, 1.f);
    return vso;
}