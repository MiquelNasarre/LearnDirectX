#pragma once
#include "Graphics.h"

#include "Bindable/BindableBase.h"

class Drawable
{
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	void Draw(Graphics& gfx) const;
	void Bind(Graphics& gfx) const;

protected:
	void* AddBind(std::unique_ptr<Bindable> bind);
private:
	const IndexBuffer* pIndexBuffer;
	std::vector<std::unique_ptr<Bindable>> binds;
};