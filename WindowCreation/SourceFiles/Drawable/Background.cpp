#include "Drawable/Background.h"

Background::Background(Graphics& gfx, std::string filename)
{
	AddBind(std::make_unique<Texture>(gfx, filename));

	AddBind(std::make_unique<Sampler>(gfx, D3D11_FILTER_MIN_MAG_MIP_LINEAR));

	std::vector<_float4vector> vertexs;
	vertexs.push_back({ -1.f,-1.f,0.f,0.f });
	vertexs.push_back({  1.f,-1.f,1.f,0.f });
	vertexs.push_back({ -1.f, 1.f,0.f,1.f });
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

	auto pvs = (VertexShader*)AddBind(std::move(std::make_unique<VertexShader>(gfx, L"Shaders/BackgroundVS.cso")));

	AddBind(std::make_unique<PixelShader>(gfx, L"Shaders/BackgroundPS.cso"));

	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	std::vector< D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	AddBind(std::make_unique<InputLayout>(gfx, ied, pvs->GetBytecode()));
}

void Background::updateTexture(Graphics& gfx, std::string filename)
{
	changeBind(std::make_unique<Texture>(gfx, filename), 0u);
}
