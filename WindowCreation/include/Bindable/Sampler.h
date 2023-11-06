#pragma once
#include "Bindable.h"

class Sampler : public Bindable
{
public:
	Sampler(Graphics& gfx, D3D11_FILTER filter);
	void Bind(Graphics& gfx) override;
private:
	pCom<ID3D11SamplerState> pSampler;
};
