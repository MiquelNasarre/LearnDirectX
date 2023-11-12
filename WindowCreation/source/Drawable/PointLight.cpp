#include "Drawable/PointLight.h"

PointLight::PointLight(Graphics& gfx, Color color, Vector3f Position, float Radius, UINT circlePoints)
{
	std::vector<Vertex> vertexs;
	vertexs.push_back(Vertex({ 0 , 0 }, 1));
	for (UINT i = 0; i < circlePoints; i++)
		vertexs.push_back(Vertex({ cosf(2.f * pi * float(i) / circlePoints) , sinf(2.f * pi * float(i) / circlePoints) }, 0));

	std::vector<unsigned short> indexs;
	for (UINT i = 1; i <= circlePoints; i++) {
		indexs.push_back(0);
		indexs.push_back(i % circlePoints + 1);
		indexs.push_back(i);
	}

	AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));
	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));

	auto pvs = (VertexShader*)AddBind(std::move(std::make_unique<VertexShader>(gfx, SHADERS_DIR + std::wstring(L"PointLightVS.cso"))));
	AddBind(std::make_unique<PixelShader>(gfx, SHADERS_DIR + std::wstring(L"PointLightPS.cso")));

	std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Reference",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Intensity",0,DXGI_FORMAT_R32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	AddBind(std::make_unique<InputLayout>(gfx, ied, pvs->GetBytecode()));
	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	vscBuff = { Position.getVector4(), gfx.getObserver().getVector4(), { Radius, 0.f, 0.f, 0.f } };
	pscBuff = { color.getColor4() };

	pVSCB = (ConstantBuffer<VSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<VSconstBuffer>>(gfx, vscBuff, VERTEX_CONSTANT_BUFFER_TYPE));
	pPSCB = (ConstantBuffer<PSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<PSconstBuffer>>(gfx, pscBuff, PIXEL_CONSTANT_BUFFER_TYPE));
}

void PointLight::updateRadius(Graphics& gfx, float radius)
{
	vscBuff.radius = { radius, 0.f, 0.f, 0.f };

	pVSCB->Update(gfx, vscBuff);
}

void PointLight::updatePosition(Graphics& gfx, Vector3f position)
{
	vscBuff.Position = position.getVector4();

	pVSCB->Update(gfx, vscBuff);
}

void PointLight::updateColor(Graphics& gfx, Color color)
{
	pscBuff.color = color.getColor4();

	pPSCB->Update(gfx, pscBuff);
}

void PointLight::Draw(Graphics& gfx)
{
	vscBuff.observer = gfx.getObserver().getVector4();
	pVSCB->Update(gfx, vscBuff);

	_draw(gfx);
}
