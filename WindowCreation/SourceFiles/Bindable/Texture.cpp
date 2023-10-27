#include "Bindable/Texture.h"
#include "ExceptionMacros.h"
#include "resource.h"
#include "FreeImage.h"

Texture::Texture(Graphics& gfx, std::string filename, UINT slot) : Slot{ slot }
{
	INFOMAN(gfx);

	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename.c_str());
	if (format == FIF_UNKNOWN)      format = FreeImage_GetFIFFromFilename(filename.c_str());
	if (format == FIF_UNKNOWN)      throw(std::runtime_error("File format not supported"));

	FIBITMAP* bitmap = FreeImage_Load(format, filename.c_str());
	FIBITMAP* bitmap2 = FreeImage_ConvertTo32Bits(bitmap);
	FreeImage_Unload(bitmap);

	UINT width = FreeImage_GetWidth(bitmap2);
	UINT height = FreeImage_GetHeight(bitmap2);

	if (!width && !height)
		throw std::exception(std::string("ERROR: The image file [" + filename + "] could not be correctly loaded or found.").c_str());

	void* pBuffer = calloc(sizeof(Color), width * height);
	FreeImage_ConvertToRawBits((BYTE*)pBuffer, bitmap2, FreeImage_GetWidth(bitmap2) * 4, 32, FI_RGBA_BLUE_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_RED_MASK, true);
	FreeImage_Unload(bitmap2);

	//	Create texture resource

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1u;
	textureDesc.ArraySize = 1u;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1u;
	textureDesc.SampleDesc.Quality = 0u;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0u;
	textureDesc.MiscFlags = 0u;
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = pBuffer;
	sd.SysMemPitch = width * 4;

	pCom<ID3D11Texture2D> pTexture;
	GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(&textureDesc, &sd, &pTexture));

	//	Create the resource view on the texture

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0u;
	srvDesc.Texture2D.MipLevels = 1u;

	GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(pTexture.Get(), &srvDesc, &pTextureView));
	free(pBuffer);
}

void Texture::Bind(Graphics& gfx)
{
	INFOMAN(gfx);
	GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetShaderResources(Slot, 1u, pTextureView.GetAddressOf()));
}
