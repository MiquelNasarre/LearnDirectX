#include "Bindable/Topology.h"
#include "ExceptionMacros.h"

Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY type)
	: type(type)
{
}

void Topology::Bind(Graphics& gfx)
{
	INFOMAN(gfx);
	GFX_THROW_INFO_ONLY(GetContext(gfx)->IASetPrimitiveTopology(type));
}
