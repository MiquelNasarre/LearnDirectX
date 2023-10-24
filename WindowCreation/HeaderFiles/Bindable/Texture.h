#pragma once
#include "Bindable.h"

class Texture : public Bindable
{
public:
	Texture(Graphics& gfx, std::string filename);
	void Bind(Graphics& gfx) override;
private:
	pCom<ID3D11ShaderResourceView> pTextureView;
};
