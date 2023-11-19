#pragma once
#include "Bindable.h"

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices);
	IndexBuffer(Graphics& gfx, unsigned short* indices, UINT size);
	void Bind(Graphics& gfx) override;
	UINT GetCount() const noexcept;
private:
	UINT count;
	pCom<ID3D11Buffer> pIndexBuffer;
};