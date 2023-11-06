#pragma once
#include "Bindable.h"

enum TYPE_ConstBuffer {
 VERTEX_CONSTANT_BUFFER_TYPE,
 PIXEL_CONSTANT_BUFFER_TYPE
};


#define CONSTANT_BUFFER_DEFAULT_SLOT -1

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	void Update(Graphics& gfx, const C& consts);
	void Update(Graphics& gfx, const C* consts);
	ConstantBuffer(Graphics& gfx, const C& consts, TYPE_ConstBuffer type, const int slot = CONSTANT_BUFFER_DEFAULT_SLOT);
	ConstantBuffer(Graphics& gfx, TYPE_ConstBuffer type, const int slot = CONSTANT_BUFFER_DEFAULT_SLOT);
	void Bind(Graphics& gfx) override;

private:
	pCom<ID3D11Buffer> pConstantBuffer;
	TYPE_ConstBuffer Type;
	UINT Slot;
};

// Function Implementations

template<typename C>
void ConstantBuffer<C>::Update(Graphics& gfx, const C& consts)
{
	//INFOMAN(gfx);
	//GFX_THROW_INFO_ONLY();
	GetContext(gfx)->UpdateSubresource(pConstantBuffer.Get(), 0u, NULL, &consts, 0u, 0u);
}

template<typename C>
void ConstantBuffer<C>::Update(Graphics& gfx, const C* consts)
{
	//INFOMAN(gfx);
	//GFX_THROW_INFO_ONLY();
	GetContext(gfx)->UpdateSubresource(pConstantBuffer.Get(), 0u, NULL, consts, 0u, 0u);
}

template<typename C>
ConstantBuffer<C>::ConstantBuffer(Graphics& gfx, const C& consts, TYPE_ConstBuffer type, const int slot)
	:Type{ type }
{
	//INFOMAN(gfx);

	if (slot == CONSTANT_BUFFER_DEFAULT_SLOT) {
		if (type == VERTEX_CONSTANT_BUFFER_TYPE)
			Slot = 1u;
		if (type == PIXEL_CONSTANT_BUFFER_TYPE)
			Slot = 0u;
	}
	else
		Slot = (UINT)slot;

	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.CPUAccessFlags = 0u;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(consts);
	cbd.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &consts;
	//GFX_THROW_INFO();
	GetDevice(gfx)->CreateBuffer(&cbd, &csd, &pConstantBuffer);
}

template<typename C>
ConstantBuffer<C>::ConstantBuffer(Graphics& gfx, TYPE_ConstBuffer type, const int slot)
	:Type{ type }
{
	//INFOMAN(gfx);
	if (slot == CONSTANT_BUFFER_DEFAULT_SLOT) {
		if (type == VERTEX_CONSTANT_BUFFER_TYPE)
			Slot = 1u;
		if (type == PIXEL_CONSTANT_BUFFER_TYPE)
			Slot = 0u;
	}
	else
		Slot = (UINT)slot;

	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.CPUAccessFlags = 0u;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(C);
	cbd.StructureByteStride = 0u;
	//GFX_THROW_INFO();
	GetDevice(gfx)->CreateBuffer(&cbd, NULL, &pConstantBuffer);
}

template<typename C>
void ConstantBuffer<C>::Bind(Graphics& gfx)
{
	//INFOMAN(gfx);
	if (Type == VERTEX_CONSTANT_BUFFER_TYPE) {
		//GFX_THROW_INFO_ONLY();
		GetContext(gfx)->VSSetConstantBuffers(Slot, 1u, pConstantBuffer.GetAddressOf());
	}
	else if (Type == PIXEL_CONSTANT_BUFFER_TYPE) {
		//GFX_THROW_INFO_ONLY();
		GetContext(gfx)->PSSetConstantBuffers(Slot, 1u, pConstantBuffer.GetAddressOf());
	}
}
