#include "Bindable/VertexBuffer.h"
#include "ExceptionMacros.h"

void VertexBuffer::Bind(Graphics& gfx)
{
	INFOMAN(gfx);

	const UINT offset = 0u;
	GFX_THROW_INFO_ONLY(GetContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset));
}