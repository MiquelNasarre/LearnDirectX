#include "Bindable/ConstantBuffers.h"

template<typename C>
void ConstantBuffer<C>::Update(Graphics& gfx, const C& consts)
{
	INFOMAN(gfx);
	GFX_THROW_INFO_ONLY(GetContext(gfx)->UpdateSubresource(pConstantBuffer.Get(), 0u, NULL, &consts, 0u, 0u));
}


