#pragma once
#include "Drawable.h"

class Background : public Drawable
{
public:
	Background(Graphics& gfx, std::string filename, bool MakeDynamic = false);
	void updateTexture(Graphics& gfx, std::string filename);
	void updateObserver(Graphics& gfx, Vector3f obs);
	void updateWideness(Graphics& gfx, Vector2f wideness);

private:
	ConstantBuffer<_float4vector>* pscBuff0 = NULL;
	ConstantBuffer<_float4vector>* pscBuff1 = NULL;
};