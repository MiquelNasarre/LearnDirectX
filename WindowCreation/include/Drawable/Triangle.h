#pragma once
#include "Drawable.h"

class Triangle : public Drawable
{
public:
	Triangle(Graphics& gfx, Color color);
	void Update(Graphics& gfx, Vector2f ZXrotation, Vector3f position);
	void Update(Graphics& gfx, Vector2f ZXrotation, float dt);
private:
	struct VSconstBuffer {
		_float4vector translation;
		Quaternion rotation;
	}vscBuff;

	struct PSconstBuffer {
		_float4vector norm4[12];
	}pscBuff;

	std::vector<Vector3f> norms;
	ConstantBuffer<VSconstBuffer>* pVSCB;
	ConstantBuffer<PSconstBuffer>* pPSCB;

};