
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