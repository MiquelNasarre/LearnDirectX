#pragma once
#include "Drawable.h"

class PointLight : public Drawable
{
public:
	PointLight(Graphics& gfx, Color color, Vector3f Position, float Radius, UINT circlePoints = 40u);

	void update(Graphics& gfx);
	void updateRadius(Graphics& gfx, float radius);
	void updatePosition(Graphics& gfx, Vector3f position);
	void updateColor(Graphics& gfx, Color color);

private:

	struct VSconstBuffer {
		_float4vector Position;
		_float4vector observer;
		_float4vector radius;
	}vscBuff;

	struct PSconstBuffer {
		_float4color color;
	}pscBuff;

	struct Vertex {
		Vector2f reference;
		float intensity;
	};

	ConstantBuffer<VSconstBuffer>* pVSCB;
	ConstantBuffer<PSconstBuffer>* pPSCB;
};
