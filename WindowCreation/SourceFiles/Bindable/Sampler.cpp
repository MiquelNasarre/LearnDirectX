#include "Bindable/Sampler.h"
#include "ExceptionMacros.h"

Sampler::Sampler(Graphics& gfx)
{
	INFOMAN(gfx);

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	GFX_THROW_INFO(GetDevice(gfx)->CreateSamplerState(&samplerDesc, &pSampler));
}

void Sampler::Bind(Graphics& gfx)
{
	INFOMAN(gfx);
	GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetSamplers(0, 1, pSampler.GetAddressOf()));
}