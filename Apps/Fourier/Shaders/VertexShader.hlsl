
float4 qMul(float4 q0, float4 q1)
{
    return float4(
		q0.r * q1.r - q0.g * q1.g - q0.b * q1.b - q0.a * q1.a,
		q0.r * q1.g + q0.g * q1.r + q0.b * q1.a - q0.a * q1.b,
		q0.r * q1.b + q0.b * q1.r + q0.a * q1.g - q0.g * q1.a,
		q0.r * q1.a + q0.a * q1.r + q0.g * q1.b - q0.b * q1.g
	);
}

float4 qMul(float4 q0, float4 pos, float4 q1)
{
    return qMul(qMul(q0, pos), q1);
}

float4 qRot(float4 q, float4 pos)
{
    return qMul(q, pos, float4(q.r, -q.g, -q.b, -q.a));
}

float4 Q2V(float4 q)
{
    return float4(q.g, q.b, q.a, 1.f);
}

float4 V2Q(float4 v)
{
    return float4(0.f, v.r, v.g, v.b);
}

// quaternion crap ^^

cbuffer Cbuff0 : register(b0)
{
    matrix projection;
    float4 center;
};

cbuffer Cbuff1 : register(b1)
{
    float4 traslation;
    float4 quaternion;
};

struct VSOut
{
    float4 R3pos : PointPos;
    float4 norm : Norm;
    float4 color : Color;
    float4 SCpos : SV_Position;
};

VSOut main(float3 pos : Position, float3 norm : Normal, float4 color : Color)
{
    VSOut vso;
    
    vso.norm = Q2V(qRot(quaternion, float4(0, norm)));
    vso.R3pos = Q2V(qRot(quaternion, float4(0, pos))) + traslation;
    vso.color = color;
    
    float4 test = mul(vso.R3pos - center, projection);
    vso.SCpos = float4(test.x, test.y, test.z / 10000000.f + 0.5f, 1.f);
    
    return vso;
}