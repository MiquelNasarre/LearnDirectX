#pragma once
#include "Bindable.h"

class Rasterizer : public Bindable
{
public:
	Rasterizer(Graphics& gfx, bool doubleSided);
	void Bind(Graphics& gfx) override;
private:
	pCom<ID3D11RasterizerState> pRasterizer;
};