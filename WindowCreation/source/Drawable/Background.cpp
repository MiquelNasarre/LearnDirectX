#include "Drawable/Background.h"

Background::Background(Graphics& gfx, std::string filename, bool MakeDynamic, PROJECTION_TYPES ProjectionType)
{
	AddBind(std::make_unique<Texture>(gfx, filename));

	AddBind(std::make_unique<Sampler>(gfx, D3D11_FILTER_MIN_MAG_MIP_LINEAR));

	std::vector<_float4vector> vertexs;
	vertexs.push_back({ -1.f,-1.f,1.f,1.f });
	vertexs.push_back({  1.f,-1.f,1.f,1.f });
	vertexs.push_back({ -1.f, 1.f,1.f,1.f });
	vertexs.push_back({  1.f, 1.f,1.f,1.f });

	AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));

	std::vector<unsigned short> indexs;
	indexs.push_back(0u);
	indexs.push_back(2u);
	indexs.push_back(1u);
	indexs.push_back(1u);
	indexs.push_back(2u);
	indexs.push_back(3u);

	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));

	VertexShader* pvs;

	if (!MakeDynamic) {
		pvs = (VertexShader*)AddBind(std::move(std::make_unique<VertexShader>(gfx, SHADERS_DIR + std::wstring(L"BackgroundVS.cso"))));
		AddBind(std::make_unique<PixelShader>(gfx, SHADERS_DIR + std::wstring(L"BackgroundPS.cso")));

		vscBuff = (ConstantBuffer<_float4vector>*)AddBind(std::make_unique<ConstantBuffer<_float4vector>>(gfx, _float4vector{ 0.f,0.f,1.f,1.f }, VERTEX_CONSTANT_BUFFER_TYPE));
	}
	else {
		pvs = (VertexShader*)AddBind(std::move(std::make_unique<VertexShader>(gfx, SHADERS_DIR + std::wstring(L"DynamicBgVS.cso"))));

		if (ProjectionType == PT_MERCATOR)
			AddBind(std::make_unique<PixelShader>(gfx, SHADERS_DIR + std::wstring(L"DyBgMercatorPS.cso")));
		else if (ProjectionType == PT_AZIMUTHAL)
			AddBind(std::make_unique<PixelShader>(gfx, SHADERS_DIR + std::wstring(L"DyBgAzimuthPS.cso")));
		else
			throw std::exception("this Projection Type is not suported by the dynamic bacground");

		pscBuff0 = (ConstantBuffer<PSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<PSconstBuffer>>(gfx, PIXEL_CONSTANT_BUFFER_TYPE));
		pscBuff1 = (ConstantBuffer<_float4vector>*)AddBind(std::make_unique<ConstantBuffer<_float4vector>>(gfx, PIXEL_CONSTANT_BUFFER_TYPE, 1u));
	}

	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	std::vector< D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	AddBind(std::make_unique<InputLayout>(gfx, ied, pvs->GetBytecode()));
}

Background::Background(Graphics& gfx, Texture texture, bool MakeDynamic, PROJECTION_TYPES ProjectionType)
{
	((Texture*)AddBind(std::make_unique<Texture>(texture)))->setSlot(0u);

	AddBind(std::make_unique<Sampler>(gfx, D3D11_FILTER_MIN_MAG_MIP_LINEAR));

	std::vector<_float4vector> vertexs;
	vertexs.push_back({ -1.f,-1.f,1.f,1.f });
	vertexs.push_back({ 1.f,-1.f,1.f,1.f });
	vertexs.push_back({ -1.f, 1.f,1.f,1.f });
	vertexs.push_back({ 1.f, 1.f,1.f,1.f });

	AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));

	std::vector<unsigned short> indexs;
	indexs.push_back(0u);
	indexs.push_back(2u);
	indexs.push_back(1u);
	indexs.push_back(1u);
	indexs.push_back(2u);
	indexs.push_back(3u);

	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));

	VertexShader* pvs;

	if (!MakeDynamic) {
		pvs = (VertexShader*)AddBind(std::move(std::make_unique<VertexShader>(gfx, SHADERS_DIR + std::wstring(L"BackgroundVS.cso"))));
		AddBind(std::make_unique<PixelShader>(gfx, SHADERS_DIR + std::wstring(L"BackgroundPS.cso")));

		vscBuff = (ConstantBuffer<_float4vector>*)AddBind(std::make_unique<ConstantBuffer<_float4vector>>(gfx, _float4vector{ 0.f,0.f,1.f,1.f } , VERTEX_CONSTANT_BUFFER_TYPE));
	}
	else {
		pvs = (VertexShader*)AddBind(std::move(std::make_unique<VertexShader>(gfx, SHADERS_DIR + std::wstring(L"DynamicBgVS.cso"))));

		if (ProjectionType == PT_MERCATOR)
			AddBind(std::make_unique<PixelShader>(gfx, SHADERS_DIR + std::wstring(L"DyBgMercatorPS.cso")));
		else if (ProjectionType == PT_AZIMUTHAL)
			AddBind(std::make_unique<PixelShader>(gfx, SHADERS_DIR + std::wstring(L"DyBgAzimuthPS.cso")));
		else
			throw std::exception("this Projection Type is not suported by the dynamic background");

		pscBuff0 = (ConstantBuffer<PSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<PSconstBuffer>>(gfx, PIXEL_CONSTANT_BUFFER_TYPE));
		pscBuff1 = (ConstantBuffer<_float4vector>*)AddBind(std::make_unique<ConstantBuffer<_float4vector>>(gfx, PIXEL_CONSTANT_BUFFER_TYPE, 1u));
	}

	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	std::vector< D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	AddBind(std::make_unique<InputLayout>(gfx, ied, pvs->GetBytecode()));
}

void Background::updateTexture(Graphics& gfx, std::string filename)
{
	changeBind(std::make_unique<Texture>(gfx, filename), 0u);
}

void Background::updateTexture(Graphics& gfx, Texture texture)
{
	((Texture*)changeBind(std::make_unique<Texture>(texture), 0u))->setSlot(0u);
}

void Background::updateObserver(Graphics& gfx, Vector3f obs)
{
	if (!pscBuff0)
		throw std::exception("Not possible to call updateObserver() on a non dynamic background!!");

	cBuff0.obs = obs.normalize().getVector4();
	cBuff0.ei = Vector3f(obs.y, -obs.x, 0.f).normalize().getVector4();
	cBuff0.zp = (obs * Vector3f(obs.y, -obs.x, 0.f)).normalize().getVector4();
	pscBuff0->Update(gfx, cBuff0);
}

void Background::updateWideness(Graphics& gfx, float FOV, Vector2f WindowDimensions)
{
	if (!pscBuff1)
		throw std::exception("Not possible to call updateWideness() on a non dynamic background!!");

	pscBuff1->Update(gfx, Vector3f(FOV, WindowDimensions.x, WindowDimensions.y).getVector4());
}

void Background::updateRectangle(Graphics& gfx, _float4vector rectangle)
{
	if (!vscBuff)
		throw std::exception("Not possible to call updateRectangle() on a dynamic background!!");

	vscBuff->Update(gfx, rectangle);
}
