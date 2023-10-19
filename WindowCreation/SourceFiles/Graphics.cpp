#include "Graphics.h"
#include <sstream>
#include <d3dcompiler.h>

#include "Math/Vectors.h"
#include "Math/Matrix.h"

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

	pCom<ID3D11Resource> pBackBuffer;
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

void Graphics::clearBuffer(Color color) noexcept
{
	const float col[] = { color.R / 255.f,color.G / 255.f,color.B / 255.f,color.A / 255.f };
	pContext->ClearRenderTargetView(pTarget.Get(), col);
}

Vector2f Graphics::PixeltoR2(Vector2i MousePos)
{
	return Vector2f(2.f * MousePos.x / WindowDimensions.x - 1.f, -2.f * MousePos.y / WindowDimensions.y + 1.f);
}

void Graphics::setWindowDimensions(Vector2i& Dim)
{
	WindowDimensions = Dim;
}

void Graphics::initSettings()
{
	//	Bind render target

	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), NULL);

	//	Configure viewport

	CD3D11_VIEWPORT vp;
	vp.Width = 640.f,
	vp.Height = 480.f,
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u, &vp);
}

void Graphics::initTestTriangle()
{
	HRESULT hr;

	//	Create vertex buffer

	TestTriangle.vertexs.push_back({Vector3f(0.f,0.f,0.5f),Color::White});
	TestTriangle.vertexs.push_back({ 0.5f * Vector3f(cosf(2 * 3.14159f * 0 / 6) ,-sinf(2 * 3.14159f * 0 / 6), 0.f) , Color::Red		});
	TestTriangle.vertexs.push_back({ 0.5f * Vector3f(cosf(2 * 3.14159f * 1 / 6) ,-sinf(2 * 3.14159f * 1 / 6), 0.f) , Color::Yellow	});
	TestTriangle.vertexs.push_back({ 0.5f * Vector3f(cosf(2 * 3.14159f * 2 / 6) ,-sinf(2 * 3.14159f * 2 / 6), 0.f) , Color::Green	});
	TestTriangle.vertexs.push_back({ 0.5f * Vector3f(cosf(2 * 3.14159f * 3 / 6) ,-sinf(2 * 3.14159f * 3 / 6), 0.f) , Color::Cyan	});
	TestTriangle.vertexs.push_back({ 0.5f * Vector3f(cosf(2 * 3.14159f * 4 / 6) ,-sinf(2 * 3.14159f * 4 / 6), 0.f) , Color::Blue	});
	TestTriangle.vertexs.push_back({ 0.5f * Vector3f(cosf(2 * 3.14159f * 5 / 6) ,-sinf(2 * 3.14159f * 5 / 6), 0.f) , Color::Purple	});
	TestTriangle.vertexs.push_back({ Vector3f(0.f,0.f,-0.5f),Color::Black });

	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = (UINT)TestTriangle.vertexs.size()*sizeof(Vertex);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = &TestTriangle.vertexs[0];
	GFX_THROW_NOINFO(pDevice->CreateBuffer(&bd, &sd, &TestTriangle.pVertexBuffer));

	//	Create index buffer

	for (int i = 0; i < 6; i++) {
		TestTriangle.Indexs.push_back(0);
		TestTriangle.Indexs.push_back(i + 1);
		TestTriangle.Indexs.push_back((i + 1) % 6 + 1);
	}
	for (int i = 0; i < 6; i++) {
		TestTriangle.Indexs.push_back(7);
		TestTriangle.Indexs.push_back((i + 1) % 6 + 1);
		TestTriangle.Indexs.push_back(i + 1);
	}

	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = (UINT)TestTriangle.Indexs.size() * sizeof(unsigned short);
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = &TestTriangle.Indexs[0];
	GFX_THROW_NOINFO(pDevice->CreateBuffer(&ibd, &isd, &TestTriangle.pIndexBuffer));
	TestTriangle.NumIndexes = (UINT)TestTriangle.Indexs.size();

	//	Save the normal vector of the triangles for pixel shaders

	for (UINT i = 0; i < TestTriangle.NumIndexes / 3; i++)
		TestTriangle.norms.push_back(
			((TestTriangle.vertexs[TestTriangle.Indexs[3 * i + 1]].vector - TestTriangle.vertexs[TestTriangle.Indexs[3 * i]].vector) *
			(TestTriangle.vertexs[TestTriangle.Indexs[3 * i + 2]].vector - TestTriangle.vertexs[TestTriangle.Indexs[3 * i]].vector)).normalize());

	//	Create pixel shader

	pCom<ID3DBlob> pBlob;
	GFX_THROW_NOINFO(D3DReadFileToBlob(L"Shaders/PixelShader.cso", &pBlob));
	GFX_THROW_NOINFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &TestTriangle.pPixelShader));

	//	Create vertex shader & bind

	GFX_THROW_NOINFO(D3DReadFileToBlob(L"Shaders/VertexShader.cso", &pBlob));
	GFX_THROW_NOINFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &TestTriangle.pVertexShader));

	//	 Create input (vertex) layout (2d position only)

	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{ "Color",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	GFX_THROW_NOINFO(pDevice->CreateInputLayout(
		ied, (UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&TestTriangle.pInputLayout
	));
}

void Graphics::drawTestTriangle(float angle, Vector2i MousePos)
{
	HRESULT hr;

	//	(Matrix transformations) Vertex shader constant buffer

	struct VSconstBuffer{
		_float4matrix movement;
		_float4matrix perspective;
	};
	
	Vector2f pos = PixeltoR2(MousePos);
	Matrix Rotations = ZRotationMatrix(-pos.x * 3.14159f) * XRotationMatrix(-pos.y * 3.14159f / 2.f);
	Matrix Projections = ProjectionMatrix(Vector3f(-1.f, 0.f, 0.f)) * ScalingMatrix((float)WindowDimensions.y / (float)WindowDimensions.x, 1.f, 1.f);
	const VSconstBuffer cBuff = {
		Rotations.transpose().getMatrix4(),
		Projections.transpose().getMatrix4()
	};

	pCom<ID3D11Buffer> pConstBuffer;

	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cBuff);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cBuff;
	GFX_THROW_NOINFO(pDevice->CreateBuffer(&cbd, &csd, &pConstBuffer));

	pContext->VSSetConstantBuffers(0u, 1u, pConstBuffer.GetAddressOf());

	//	(light management) Pixel shader constant buffer

	struct PSconstBuffer {
		_float4vector norm4[12];
	};

	PSconstBuffer pscBuff;
	for (UINT i = 0; i < 12u; i++)
		pscBuff.norm4[i] = { 1.f,0.f,0.f,0.f };//(Rotations * TestTriangle.norms[i]).getVector4();

	D3D11_BUFFER_DESC pscbd = {};
	pscbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pscbd.Usage = D3D11_USAGE_DYNAMIC;
	pscbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pscbd.MiscFlags = 0u;
	pscbd.ByteWidth = sizeof(pscBuff);
	pscbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA pscsd = {};
	pscsd.pSysMem = &cBuff;
	GFX_THROW_NOINFO(pDevice->CreateBuffer(&pscbd, &pscsd, &TestTriangle.pPSConstantBuffer));

	pContext->PSSetConstantBuffers(0u, 1u, TestTriangle.pPSConstantBuffer.GetAddressOf());

	//	Bind vertex buffer

	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, TestTriangle.pVertexBuffer.GetAddressOf(), &stride, &offset);

	//	Bind index buffer

	pContext->IASetIndexBuffer(TestTriangle.pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	//	Bind pixel shader

	pContext->PSSetShader(TestTriangle.pPixelShader.Get(), nullptr, 0u);

	//	Bind vertex shader

	pContext->VSSetShader(TestTriangle.pVertexShader.Get(), nullptr, 0u);

	//	Bind input layout

	pContext->IASetInputLayout(TestTriangle.pInputLayout.Get());

	//	Set primitive topology

	pContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//	Draw test triangle

	GFX_THROW_INFO_ONLY(pContext->DrawIndexed(TestTriangle.NumIndexes,0u, 0u));
}
