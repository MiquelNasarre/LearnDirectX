#include "Drawable/Drawable.h"
#include "ExceptionMacros.h"
#include "Bindable/IndexBuffer.h"
#include <cassert>
#include <typeinfo>

void Drawable::Draw(Graphics& gfx) const
{
	for (auto& b : binds)
		b->Bind(gfx);

	gfx.drawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind)
{
	if(typeid(*bind) == typeid(IndexBuffer))
		pIndexBuffer = reinterpret_cast<IndexBuffer*>(bind.get());
	binds.push_back(std::move(bind));
}
