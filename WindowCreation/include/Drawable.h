#pragma once
#include "Bindable/BindableBase.h"

class Drawable
{
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual void Draw(Graphics& gfx);

protected:
	bool isInit = false;

	void _draw(Graphics& gfx) const;

	void* AddBind(std::unique_ptr<Bindable> bind);
	void* changeBind(std::unique_ptr<Bindable> bind, UINT N);
private:
	const IndexBuffer* pIndexBuffer;
	std::vector<std::unique_ptr<Bindable>> binds;
};