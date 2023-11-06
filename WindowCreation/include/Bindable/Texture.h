#pragma once
#include "Bindable.h"

class Texture : public Bindable
{
public:
	Texture(Graphics& gfx, std::string filename, UINT slot = 0u);
	void Bind(Graphics& gfx) override;
	void setSlot(UINT slot);
private:
	UINT Slot;
	pCom<ID3D11ShaderResourceView> pTextureView;
};
