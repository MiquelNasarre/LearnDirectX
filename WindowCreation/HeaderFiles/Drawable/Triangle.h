#pragma once
#include "Drawable/Drawable.h"
#include <random>

class Triangle : public Drawable
{
	struct VSconstBuffer {
		_float4vector translation;
		_float4matrix rotation;
		_float4matrix projection;
	}vscBuff;

	struct PSconstBuffer {
		_float4vector norm4[24];
	}pscBuff;
public:
	Triangle(Graphics& gfx);
	void Update(float dt) noexcept;
private:
	// positional
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;
	// speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
};