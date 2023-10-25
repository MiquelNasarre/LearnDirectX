#pragma once
#include "Drawable.h"

class TexSurface : public Drawable
{
public:
	TexSurface(Graphics& gfx, std::string filename = "");
	void updateRotation(Graphics& gfx, float rotationZ, float rotationX, Vector3f position = Vector3f());

private:
	struct VSconstBuffer {
		_float4matrix rotattion;
		_float4vector traslation;
	}vscBuff;

	ConstantBuffer<VSconstBuffer>* pVSCB;
};