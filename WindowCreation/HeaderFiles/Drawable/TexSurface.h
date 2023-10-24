#pragma once
#include "Drawable.h"

class TexSurface : public Drawable
{
public:
	TexSurface(Graphics& gfx);
	void updateRotation(Graphics& gfx, float rotationZ, float rotationX);

private:
	ConstantBuffer<_float4matrix>* pVSCB;
};