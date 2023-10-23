#pragma once
#include "Drawable.h"

class Triangle : public Drawable
{
	struct VSconstBuffer {
		_float4vector translation;
		_float4matrix rotation;
	}vscBuff;

	struct PSconstBuffer {
		_float4vector norm4[12];
	}pscBuff;
public:
	Triangle(Graphics& gfx, Color color);
	void Update(Graphics& gfx, Vector2f ZXrotation, Vector3f position);
	void Update(Graphics& gfx, Vector2f ZXrotation, float dt);
private:
	std::vector<Vector3f> norms;
	ConstantBuffer<VSconstBuffer>* pVSCB;
	ConstantBuffer<PSconstBuffer>* pPSCB;

	Vector3f Position		= Vector3f();
	Vector3f Velocity		= Vector3f();
	Vector3f Acceleration	= Vector3f();
	Vector3f dA				= Vector3f();
};