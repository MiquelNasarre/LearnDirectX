#include "Bindable/Rasterizer.h"
#include "Exception/ExceptionMacros.h"

Rasterizer::Rasterizer(Graphics& gfx, bool doubleSided)
{
	INFOMAN(gfx);

	D3D11_RASTERIZER_DESC rasterDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
	rasterDesc.CullMode = doubleSided ? D3D11_CULL_NONE : D3D11_CULL_BACK;

	GFX_THROW_INFO(GetDevice(gfx)->CreateRasterizerState(&rasterDesc, &pRasterizer));
}

void Rasterizer::Bind(Graphics& gfx)
{
	INFOMAN(gfx);

	GFX_THROW_INFO_ONLY(GetContext(gfx)->RSSetState(pRasterizer.Get()));
}
