#include "Drawable.h"
#include <typeinfo>

void Drawable::Draw(Graphics& gfx)
{
	if (!isInit)
		throw std::exception("You cannot issue a draw call if the drawable has not been initialized");

	_draw(gfx);
}

void Drawable::_draw(Graphics& gfx) const
{
	for (auto& b : binds)
		b->Bind(gfx);

	gfx.drawIndexed(pIndexBuffer->GetCount());
}

void* Drawable::AddBind(std::unique_ptr<Bindable> bind)
{
	if(typeid(*bind) == typeid(IndexBuffer))
		pIndexBuffer = reinterpret_cast<IndexBuffer*>(bind.get());
	binds.push_back(std::move(bind));
	return (void*)binds[binds.size() - 1].get();
}

void* Drawable::changeBind(std::unique_ptr<Bindable> bind, UINT N)
{
	binds[N].get()->Deletion();
	free(binds[N].get());
	binds[N].release();

	if (typeid(*bind) == typeid(IndexBuffer))
		pIndexBuffer = reinterpret_cast<IndexBuffer*>(bind.get());
	binds[N] = std::move(bind);
	return (void*)binds[N].get();
}
