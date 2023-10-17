#include "Graphics.h"
#include <sstream>
#include <d3dcompiler.h>

#include "Math/Vectors.h"
#include "Math/Complex.h"
#include "Math/Color.h"

using namespace Microsoft::WRL;

// Graphics exception stuff

#define GFX_THROW_NOINFO(hrcall)		if( FAILED( hr = (hrcall) ) ) throw Graphics::HrException( __LINE__,__FILE__,hr )
#define GFX_EXCEPT(hr)					Graphics::HrException( __LINE__,__FILE__,hr )
#define GFX_DEVICE_REMOVED_EXCEPT(hr)	Graphics::DeviceRemovedException( __LINE__,__FILE__,hr )
#define GFX_THROW_INFO_ONLY(call)		(call)

Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
	:ExceptionClass(line, file),
	hr(hr)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	}
}

const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << hr
		<< std::dec << " (" << (unsigned long)hr << ")" << std::endl
		<< "[Error String] " << "Jaja jodete" << std::endl
		<< "[Description] " << "No vas trobar el header tonto" << std::endl;
	if (!info.empty())
	{
		oss << "\n[Error Info]\n" << info << std::endl << std::endl;
	}
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept
{
	return "Graphics Exception";
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}

//	Graphics stuff

Graphics::Graphics(HWND hWnd)
{
	create(hWnd);
}

void Graphics::create(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width						= 0;
	sd.BufferDesc.Height					= 0;
	sd.BufferDesc.Format					= DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator		= 0;
	sd.BufferDesc.RefreshRate.Denominator	= 0;
	sd.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count						= 1;
	sd.SampleDesc.Quality					= 0;
	sd.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount							= 1;
	sd.OutputWindow							= hWnd;
	sd.Windowed								= TRUE;
	sd.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags								= 0;

	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//	Create device & front/back buffers & swap chain & rendering context

	HRESULT hr;
	GFX_THROW_NOINFO(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	));

	//	Gain access to render target through shinnanigins

	ComPtr<ID3D11Resource> pBackBuffer;
	GFX_THROW_NOINFO(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
	GFX_THROW_NOINFO(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget));

}

void Graphics::pushFrame()
{
	HRESULT hr;

	if (FAILED(hr = pSwap->Present(1u, 0u))) {
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
			throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
		else
			throw GFX_EXCEPT(hr);
	}
}

void Graphics::clearBuffer(float R, float G, float B, float A) noexcept
{
	const float color[] = { R,G,B,A };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
}

void Graphics::setWindowDimensions(Vector2i& Dim)
{
	WindowDimensions = Dim;
}

void Graphics::drawTestTriangle()
{
	HRESULT hr;

	//	Create vertex buffer

	struct Vertex{
		Vector2f vector;
		Color color;
	};

	const Vertex vertices[] =
	{
		{Vector2f(),Color(255,255,255,255)},
		{ 0.5f * Vector2f(cosf(2 * Pi * 0 / 6) ,-sinf(2 * Pi * 0 / 6)) , Color(255,  0,  0,255) },
		{ 0.5f * Vector2f(cosf(2 * Pi * 1 / 6) ,-sinf(2 * Pi * 1 / 6)) , Color(255,255,  0,255) },
		{ 0.5f * Vector2f(cosf(2 * Pi * 2 / 6) ,-sinf(2 * Pi * 2 / 6)) , Color(  0,255,  0,255) },
		{ 0.5f * Vector2f(cosf(2 * Pi * 3 / 6) ,-sinf(2 * Pi * 3 / 6)) , Color(  0,255,255,255) },
		{ 0.5f * Vector2f(cosf(2 * Pi * 4 / 6) ,-sinf(2 * Pi * 4 / 6)) , Color(  0,  0,255,255) },
		{ 0.5f * Vector2f(cosf(2 * Pi * 5 / 6) ,-sinf(2 * Pi * 5 / 6)) , Color(255,  0,255,255) },
	};

	//	Create vertex buffer

	pCom<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	GFX_THROW_NOINFO(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

	//	Bind vertex buffer to pipeline

	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	//	Create index buffer

	const unsigned short indexes[] =
	{
		0,1,2,
		0,2,3,
		0,3,4,
		0,4,5,
		0,5,6,
		0,6,1,
	};

	pCom<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indexes);
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indexes;
	GFX_THROW_NOINFO(pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));

	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);


	//	Create pixel shader & bind

	pCom<ID3DBlob> pBlob;
	pCom<ID3D11PixelShader> pPixelShader;
	GFX_THROW_NOINFO(D3DReadFileToBlob(L"Shaders/PixelShader.cso", &pBlob));
	GFX_THROW_NOINFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	//	Create vertex shader & bind

	pCom<ID3D11VertexShader> pVertexShader;
	GFX_THROW_NOINFO(D3DReadFileToBlob(L"Shaders/VertexShader.cso", &pBlob));
	GFX_THROW_NOINFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	//	Input (vertex) layout (2d position only)

	pCom<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "Position",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{ "Color",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	GFX_THROW_NOINFO(pDevice->CreateInputLayout(
		ied, (UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	));

	pContext->IASetInputLayout(pInputLayout.Get());

	//	Bind render target

	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), NULL);

	//	Set primitive topology

	pContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//	Configure viewport

	CD3D11_VIEWPORT vp;
	vp.Width = 640.f * 640.f / WindowDimensions.x;
	vp.Height = 480.f * 480.f / WindowDimensions.y;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u, &vp);


	GFX_THROW_INFO_ONLY(pContext->DrawIndexed((UINT)std::size(indexes),0u, 0u));
}
