#include "Graphics.h"
#include <sstream>
#include <d3dcompiler.h>

//	Graphics exception macros

#define GFX_EXCEPT_NOINFO(hr)			Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_NOINFO(hrcall)		if( FAILED( hr = (hrcall) ) ) throw Graphics::HrException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define GFX_EXCEPT(hr)					Graphics::HrException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall)			infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr)	Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO_ONLY(call)		infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw Graphics::InfoException( __LINE__,__FILE__,v);}}
#else
#define GFX_EXCEPT(hr)					Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall)			GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr)	Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO_ONLY(call)		(call)
#endif

//	Graphics stuff

Graphics::Graphics(HWND hWnd)
{
	create(hWnd);
}

void Graphics::create(HWND hWnd)
{
	HWnd = hWnd;
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width						= 0u;
	sd.BufferDesc.Height					= 0u;
	sd.BufferDesc.Format					= DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator		= 0u;
	sd.BufferDesc.RefreshRate.Denominator	= 0u;
	sd.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count						= 1u;
	sd.SampleDesc.Quality					= 0u;
	sd.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount							= 1u;
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
	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
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

	pCom<ID3D11Resource> pBackBuffer;
	GFX_THROW_INFO(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
	GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget));
}

bool Graphics::isInitialized()
{
	if (pDevice.Get())
		return true;
	return false;
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

void Graphics::clearBuffer(float R, float G, float B, float A)
{
	const float color[] = { R,G,B,A };
	GFX_THROW_INFO_ONLY(pContext->ClearRenderTargetView(pTarget.Get(), color));
}

void Graphics::clearBuffer(Color color)
{
	const float col[] = { color.R / 255.f,color.G / 255.f,color.B / 255.f,color.A / 255.f };
	GFX_THROW_INFO_ONLY(pContext->ClearRenderTargetView(pTarget.Get(), col));
}

Vector2f Graphics::PixeltoR2(Vector2i MousePos)
{
	return Vector2f(2.f * MousePos.x / WindowDim.x - 1.f, -2.f * MousePos.y / WindowDim.y + 1.f);
}

void Graphics::setWindowDimensions(Vector2i& Dim)
{
	WindowDim = Dim;

	create(HWnd);
	initSettings();
	initTestTriangle();
	bindTestTrinagle();


}

void Graphics::initSettings()
{
	HRESULT hr;

	//	Create and bind depth stencil state

	pCom<ID3D11DepthStencilState> pDSState;
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	GFX_THROW_INFO(pDevice->CreateDepthStencilState(&dsDesc, &pDSState));

	GFX_THROW_INFO_ONLY(pContext->OMSetDepthStencilState(pDSState.Get(), 1u));

	//	Create and bind depth stencil texture

	pCom<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = (UINT)WindowDim.x;
	descDepth.Height = (UINT)WindowDim.y;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	GFX_THROW_INFO(pDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil));

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	GFX_THROW_INFO(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pDSV));

	//	Bind render target

	GFX_THROW_INFO_ONLY(pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get()));

	//	Configure viewport

	CD3D11_VIEWPORT vp;
	vp.Width = (float)WindowDim.x,
	vp.Height = (float)WindowDim.y,
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	GFX_THROW_INFO_ONLY(pContext->RSSetViewports(1u, &vp));
}

void Graphics::initTestTriangle()
{
	HRESULT hr;

	//	Create vertex buffer

	Triangle.vertexs.clear();
	Triangle.vertexs.push_back({ Vector3f(0.f, 0.f, 0.5f), Color(90,90,90,255) });
	Triangle.vertexs.push_back({ 0.5f * Vector3f(cosf(2 * 3.14159f * 0 / 6) ,-sinf(2 * 3.14159f * 0 / 6), 0.f) , Color(90,90,90,255) });
	Triangle.vertexs.push_back({ 0.5f * Vector3f(cosf(2 * 3.14159f * 1 / 6) ,-sinf(2 * 3.14159f * 1 / 6), 0.f) , Color(90,90,90,255) });
	Triangle.vertexs.push_back({ 0.5f * Vector3f(cosf(2 * 3.14159f * 2 / 6) ,-sinf(2 * 3.14159f * 2 / 6), 0.f) , Color(90,90,90,255) });
	Triangle.vertexs.push_back({ 0.5f * Vector3f(cosf(2 * 3.14159f * 3 / 6) ,-sinf(2 * 3.14159f * 3 / 6), 0.f) , Color(90,90,90,255) });
	Triangle.vertexs.push_back({ 0.5f * Vector3f(cosf(2 * 3.14159f * 4 / 6) ,-sinf(2 * 3.14159f * 4 / 6), 0.f) , Color(90,90,90,255) });
	Triangle.vertexs.push_back({ 0.5f * Vector3f(cosf(2 * 3.14159f * 5 / 6) ,-sinf(2 * 3.14159f * 5 / 6), 0.f) , Color(90,90,90,255) });
	Triangle.vertexs.push_back({ Vector3f(0.f, 0.f,-0.5f), Color(90,90,90,255) });

	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = (UINT)Triangle.vertexs.size()*sizeof(Vertex);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = &Triangle.vertexs[0];
	GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, &Triangle.pVertexBuffer));

	//	Create index buffer

	Triangle.Indexs.clear();
	for (int i = 0; i < 6; i++) {
		Triangle.Indexs.push_back(0);
		Triangle.Indexs.push_back(i + 1);
		Triangle.Indexs.push_back((i + 1) % 6 + 1);
	}
	for (int i = 0; i < 6; i++) {
		Triangle.Indexs.push_back(7);
		Triangle.Indexs.push_back((i + 1) % 6 + 1);
		Triangle.Indexs.push_back(i + 1);
	}

	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = (UINT)Triangle.Indexs.size() * sizeof(unsigned short);
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = &Triangle.Indexs[0];
	GFX_THROW_INFO(pDevice->CreateBuffer(&ibd, &isd, &Triangle.pIndexBuffer));
	Triangle.NumIndexes = (UINT)Triangle.Indexs.size();

	//	Save the normal vector of the triangles for pixel shaders

	for (UINT i = 0; i < Triangle.NumIndexes / 3; i++)
		Triangle.norms.push_back(
			((Triangle.vertexs[Triangle.Indexs[3 * i + 1]].vector - Triangle.vertexs[Triangle.Indexs[3 * i]].vector) *
			(Triangle.vertexs[Triangle.Indexs[3 * i + 2]].vector - Triangle.vertexs[Triangle.Indexs[3 * i]].vector)).normalize());

	//	(Matrix transformations) Vertex shader constant buffer

	D3D11_BUFFER_DESC vscbd = {};
	vscbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vscbd.Usage = D3D11_USAGE_DEFAULT;
	vscbd.CPUAccessFlags = 0u;
	vscbd.MiscFlags = 0u;
	vscbd.ByteWidth = sizeof(Triangle.vscBuff);
	vscbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA vscsd = {};
	vscsd.pSysMem = &Triangle.vscBuff;
	GFX_THROW_INFO(pDevice->CreateBuffer(&vscbd, &vscsd, &Triangle.pVSconstBuffer));

	//	(light management) Pixel shader constant buffer

	D3D11_BUFFER_DESC pscbd = {};
	pscbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pscbd.Usage = D3D11_USAGE_DEFAULT;
	pscbd.CPUAccessFlags = 0u;
	pscbd.MiscFlags = 0u;
	pscbd.ByteWidth = sizeof(Triangle.pscBuff);
	pscbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA pscsd = {};
	pscsd.pSysMem = &Triangle.pscBuff;
	GFX_THROW_INFO(pDevice->CreateBuffer(&pscbd, &pscsd, &Triangle.pPSconstBuffer));

	//	Create pixel shader

	pCom<ID3DBlob> pBlob;
	GFX_THROW_INFO(D3DReadFileToBlob(L"Shaders/PixelShader.cso", &pBlob));
	GFX_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &Triangle.pPixelShader));

	//	Create vertex shader

	GFX_THROW_INFO(D3DReadFileToBlob(L"Shaders/VertexShader.cso", &pBlob));
	GFX_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &Triangle.pVertexShader));

	//	 Create input (vertex) layout (2d position only)

	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{ "Color",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	GFX_THROW_INFO(pDevice->CreateInputLayout(
		ied, (UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&Triangle.pInputLayout
	));
}

void Graphics::bindTestTrinagle()
{
	//	Bind vertex shader constant buffer
	
	GFX_THROW_INFO_ONLY(pContext->VSSetConstantBuffers(0u, 1u, Triangle.pVSconstBuffer.GetAddressOf()));

	//	Bind pixel shader constant buffer

	GFX_THROW_INFO_ONLY(pContext->PSSetConstantBuffers(0u, 1u, Triangle.pPSconstBuffer.GetAddressOf()));

	//	Bind vertex buffer

	GFX_THROW_INFO_ONLY(pContext->IASetVertexBuffers(0u, 1u, Triangle.pVertexBuffer.GetAddressOf(), &Vertex::stride, &Vertex::offset));

	//	Bind index buffer

	GFX_THROW_INFO_ONLY(pContext->IASetIndexBuffer(Triangle.pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u));

	//	Bind pixel shader

	GFX_THROW_INFO_ONLY(pContext->PSSetShader(Triangle.pPixelShader.Get(), nullptr, 0u));

	//	Bind vertex shader

	GFX_THROW_INFO_ONLY(pContext->VSSetShader(Triangle.pVertexShader.Get(), nullptr, 0u));

	//	Bind input layout

	GFX_THROW_INFO_ONLY(pContext->IASetInputLayout(Triangle.pInputLayout.Get()));

	//	Set primitive topology

	GFX_THROW_INFO_ONLY(pContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
}

void Graphics::drawTestTriangle(float angle, Vector2i MousePos, float scale)
{
	//	Update vertex shader constant buffer

	Vector2f pos = PixeltoR2(MousePos);
	Vector3f Translation = Vector3f();
	Matrix Rotations = ZRotationMatrix(-pos.x * 3.14159f) * XRotationMatrix(-pos.y * 3.14159f / 2.f);

	Matrix Projections = ProjectionMatrix(Vector3f(0.f,-1.f,0.f)) * ScalingMatrix(1.f / WindowDim.x, 1.f / WindowDim.y, 1.f) * scale;
		

	Triangle.vscBuff = {
		Translation.getVector4(),
		Rotations.transpose().getMatrix4(),
		Projections.transpose().getMatrix4()
	};

	GFX_THROW_INFO_ONLY(pContext->UpdateSubresource(Triangle.pVSconstBuffer.Get(), 0u, nullptr, &Triangle.vscBuff, 0u, 0u));

	//	Update pixel shader constant buffer

	for (UINT i = 0; i < 12u; i++)
		Triangle.pscBuff.norm4[i] = (Rotations * Triangle.norms[i]).getVector4();

	GFX_THROW_INFO_ONLY(pContext->UpdateSubresource(Triangle.pPSconstBuffer.Get(), 0u, nullptr, &Triangle.pscBuff, 0u, 0u));

	//	Draw test triangle

	GFX_THROW_INFO_ONLY(pContext->DrawIndexed(Triangle.NumIndexes,0u, 0u));
}
