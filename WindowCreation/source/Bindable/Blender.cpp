#include "Bindable/Blender.h"
#include "Exception/ExceptionMacros.h"

Blender::Blender(Graphics& gfx, bool blending, float* factors)
	:blending{ blending },
	factors{ factors }
{
	INFOMAN(gfx);

	D3D11_BLEND_DESC blendDesc = {};
	auto& brt = blendDesc.RenderTarget[0];
	if (blending)
	{
		brt.BlendEnable = true;
		brt.SrcBlend = D3D11_BLEND_ONE;
		brt.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		brt.BlendOp = D3D11_BLEND_OP_ADD;
		brt.SrcBlendAlpha = D3D11_BLEND_ZERO;
		brt.DestBlendAlpha = D3D11_BLEND_ZERO;
		brt.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		brt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	else
	{
		brt.BlendEnable = false;
		brt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	GFX_THROW_INFO(GetDevice(gfx)->CreateBlendState(&blendDesc, &pBlender));
}

void Blender::Bind(Graphics& gfx)
{
	INFOMAN(gfx);

	GFX_THROW_INFO_ONLY(GetContext(gfx)->OMSetBlendState(pBlender.Get(), factors, 0xFFFFFFFFu));
}
