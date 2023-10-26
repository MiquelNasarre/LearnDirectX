#pragma once
#include "Drawable.h"

enum PROJECTION_TYPES {
	PT_MERCATOR,
	PT_AZIMUTHAL
};

class Background : public Drawable
{
public:
	Background(Graphics& gfx, std::string filename, bool MakeDynamic = false, PROJECTION_TYPES ProjectionType = PT_MERCATOR);
	void updateTexture(Graphics& gfx, std::string filename);
	void updateObserver(Graphics& gfx, Vector3f obs);
	void updateWideness(Graphics& gfx, float FOV, Vector2f WindowDimensions);

private:

	struct PSconstBuffer {
		_float4vector obs;
		_float4vector ei;
		_float4vector zp;
	}cBuff0;

	ConstantBuffer<PSconstBuffer>* pscBuff0 = NULL;
	ConstantBuffer<_float4vector>* pscBuff1 = NULL;
};