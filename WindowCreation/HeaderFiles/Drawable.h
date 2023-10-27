#pragma once
#include "Graphics.h"

#include "Bindable/BindableBase.h"

class Drawable
{
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	void Draw(Graphics& gfx) const;

protected:
	void* AddBind(std::unique_ptr<Bindable> bind);
	void* changeBind(std::unique_ptr<Bindable> bind, UINT N);
private:
	const IndexBuffer* pIndexBuffer;
	std::vector<std::unique_ptr<Bindable>> binds;
};