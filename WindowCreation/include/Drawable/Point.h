#pragma once
#include "Drawable.h"

class Point : public Drawable
{
public:
	Point(Graphics& gfx, Vector3f position, float radius, Color col = Color::White);

	void updatePosition(Graphics& gfx, Vector3f position);
	void updateRadius(Graphics& gfx, float radius);
	void updateColor(Graphics& gfx, Color col);
	void updateRotation(Graphics& gfx, Vector3f axis, float angle, bool multiplicative = false);
	void updateRotation(Graphics& gfx, Quaternion rotation, bool multiplicative = false);

	void Draw(Graphics& gfx) override;
private:

	struct VSconstBuffer {
		_float4vector position;
		Quaternion rotation;
		float radius;
		float scale;
		Vector2f unused;
	}vscBuff;

	struct PSconstBuffer {
		_float4color color;
	}pscBuff;

	struct Vertex {
		Vector3f Normal;
		float zero = 0.f;
	};

	ConstantBuffer<VSconstBuffer>* pVSCB;
	ConstantBuffer<PSconstBuffer>* pPSCB;
};