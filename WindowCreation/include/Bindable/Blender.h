#pragma once
#include "Bindable.h"

class Blender : public Bindable
{
public:
	Blender(Graphics& gfx, bool blending, float* factors = NULL);
	void Bind(Graphics& gfx) override;

private:
	pCom<ID3D11BlendState> pBlender;
	bool blending;
	float* factors;
};