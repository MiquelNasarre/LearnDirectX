#include "Quaternion.hlsli"

cbuffer Cbuff0 : register(b0)
{
    matrix projection;
    float4 center;
};

cbuffer Cbuff1 : register(b1)
{
    float4 traslation;
    float4 quaternion;
    float2 screenDisplacement;
};

struct VSOut
{
    float4 R3pos : PointPos;
    float4 norm : Norm;
    float4 color : Color;
    float4 SCpos : SV_Position;
};

float3 calculateNorm(float3 dylm, float3 pos)
{
    float3 norm;
    if (pos.z == 1.f)
        norm = float3(0.f, 0.f, 1.f);
    else if (pos.z == -1.f)
        norm = float3(0.f, 0.f, -1.f);
    else
    {
        float costheta = pos.z;
        float sintheta = sqrt(1 - pos.z * pos.z);
        float cosphi = pos.x / sintheta;
        float sinphi = pos.y / sintheta;
        
        norm = normalize(dylm.x * float3(
			-sintheta * costheta * cosphi * dylm.y + sinphi * dylm.z + sintheta * sintheta * cosphi * dylm.x,
			-sintheta * costheta * sinphi * dylm.y - cosphi * dylm.z + sintheta * sintheta * sinphi * dylm.x,
			sintheta * (sintheta * dylm.y + costheta * dylm.x)
		));
    }
    return norm;
}

VSOut main(float3 pos : Position, float3 dylm : Dylm, float4 color : Color)
{
    VSOut vso;
    
    float3 norm = calculateNorm(dylm, pos);
    
    vso.norm = Q2V(qRot(quaternion, float4(0, norm)));
    vso.R3pos = Q2V(qRot(quaternion, float4(0, abs(dylm.x) * pos))) + traslation;
    vso.color = color;
    
    float4 test = mul(vso.R3pos - center, projection);
    vso.SCpos = float4(test.x + screenDisplacement.x, test.y + screenDisplacement.y, test.z / 10000000.f + 0.5f, 1.f);
    
    return vso;
}