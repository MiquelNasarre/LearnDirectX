#include "Bindable/VertexBuffer.h"
#include "Exception/ExceptionMacros.h"

void VertexBuffer::Bind(Graphics& gfx)
{
	INFOMAN(gfx);

	const UINT offset = 0u;
	GFX_THROW_INFO_ONLY(GetContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset));
}

void VertexBuffer::Deletion()
{
	pVertexBuffer->Release();
}
