#pragma once
#include "Bindable.h"
#include "ExceptionMacros.h"
#include <d3d11.h>

#define VERTEX_CONSTANT_BUFFER_TYPE 0
#define PIXEL_CONSTANT_BUFFER_TYPE  1

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	void Update(Graphics& gfx, const C& consts)
	{
		INFOMAN(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->UpdateSubresource(pConstantBuffer.Get(), 0u, NULL, &consts, 0u, 0u));
	}

	ConstantBuffer(Graphics& gfx, const C& consts, unsigned char type)
		:Type{ type }
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DEFAULT;
		cbd.CPUAccessFlags = 0u;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(consts);
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cbd, &csd, &pConstantBuffer));
	}

	ConstantBuffer(Graphics& gfx, unsigned char type)
		:Type{ type }
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DEFAULT;
		cbd.CPUAccessFlags = 0u;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(C);
		cbd.StructureByteStride = 0u;
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cbd, NULL, &pConstantBuffer));
	}

	void Bind(Graphics& gfx) override
	{
		INFOMAN(gfx);
		if (Type == VERTEX_CONSTANT_BUFFER_TYPE) {
			GFX_THROW_INFO_ONLY(GetContext(gfx)->VSSetConstantBuffers(1u, 1u, pConstantBuffer.GetAddressOf()));
		}
		else if (Type == PIXEL_CONSTANT_BUFFER_TYPE) {
			GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf()));
		}
	}

private:
	pCom<ID3D11Buffer> pConstantBuffer;
	const unsigned char Type;
};
