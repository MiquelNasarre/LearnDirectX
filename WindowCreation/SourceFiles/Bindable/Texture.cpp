#include "Bindable/Texture.h"
#include "ExceptionMacros.h"
#include "resource.h"

Texture::Texture(Graphics& gfx, std::string filename)
{
	INFOMAN(gfx);

	//	Here you find a fucking way to load your dumb picture into an array of colors, good fucking luck

	UINT width = 5400;
	UINT height = 2700;
	Color* pBuffer = (Color*)calloc(sizeof(Color), width * height);
	srand(1234);
	for (UINT i = 0; i < height; i++) {
		for (UINT j = 0; j < width; j++) {
			pBuffer[i * width + j].R = rand() / 128;
			pBuffer[i * width + j].G = rand() / 128;
			pBuffer[i * width + j].B = rand() / 128;
			pBuffer[i * width + j].A = rand() / 128;
		}
	}

	//	Create texture resource

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1u;
	textureDesc.ArraySize = 1u;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1u;
	textureDesc.SampleDesc.Quality = 0u;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0u;
	textureDesc.MiscFlags = 0u;
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = pBuffer;
	sd.SysMemPitch = width * sizeof(Color);

	pCom<ID3D11Texture2D> pTexture;
	GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(&textureDesc, &sd, &pTexture));

	//	Create the resource view on the texture

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0u;
	srvDesc.Texture2D.MipLevels = 1u;

	GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(pTexture.Get(), &srvDesc, &pTextureView));
}

void Texture::Bind(Graphics& gfx)
{
	INFOMAN(gfx);
	GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetShaderResources(0u, 1u, pTextureView.GetAddressOf()));
}
