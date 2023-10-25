#include "Drawable/TexSurface.h"

TexSurface::TexSurface(Graphics& gfx, std::string filename)
{
	UINT depth = 5;

	struct Vertex {
		Vector3f vector;
		Vector2f texCoor;
	};

	std::vector<Vertex> vertexs;
	std::vector<unsigned short> indexs;
	
	for (UINT i = 0; i < 101; i++) {
		for (UINT j = 0; j < 101; j++) {
			float theta = i * 2 * pi / 100.f;
			float phi = -pi / 2 + j * pi / 100.f;
			vertexs.push_back({ { cosf(theta) * cosf(phi),sinf(theta) * cosf(phi),sinf(phi) } , { i / 100.f, j / 100.f } });
		}
	}

	for (UINT i = 0; i < 100; i++) {
		for (UINT j = 0; j < 100; j++) {
			indexs.push_back(i * (100 + 1) + j);
			indexs.push_back(i * (100 + 1) + j + 1);
			indexs.push_back((i + 1) * (100 + 1) + j + 1);

			indexs.push_back(i * (100 + 1) + j);
			indexs.push_back((i + 1) * (100 + 1) + j + 1);
			indexs.push_back((i + 1) * (100 + 1) + j);
		}
	}
	if (filename.size()) {
		for (auto& v : vertexs)
			v.vector *= 0.3f;
	}
	AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));

	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));

	if (!filename.size()) {
		AddBind(std::make_unique<Texture>(gfx, "Resources/EarthTexture.jpg"));
		AddBind(std::make_unique<Texture>(gfx, "Resources/nightEarthTexture.jpg", 1u));
	}
	else {
		AddBind(std::make_unique<Texture>(gfx, filename));
		AddBind(std::make_unique<Texture>(gfx, filename, 1u));
	}
	

	AddBind(std::make_unique<Sampler>(gfx, D3D11_FILTER_MIN_MAG_MIP_LINEAR));

	auto pvs = (VertexShader*)AddBind(std::move(std::make_unique<VertexShader>(gfx, L"Shaders/TexSurfaceVS.cso")));

	AddBind(std::make_unique<PixelShader>(gfx, L"Shaders/TexSurfacePS.cso"));

	std::vector< D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	AddBind(std::make_unique<InputLayout>(gfx, ied, pvs->GetBytecode()));

	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	pVSCB = (ConstantBuffer<VSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<VSconstBuffer>>(gfx, vscBuff, VERTEX_CONSTANT_BUFFER_TYPE));
}

void TexSurface::updateRotation(Graphics& gfx, float rotationZ, float rotationX, Vector3f position)
{
	vscBuff.rotattion = (ZRotationMatrix(rotationZ) * XRotationMatrix(rotationX)).transpose().getMatrix4();
	vscBuff.traslation = position.getVector4();
	pVSCB->Update(gfx, vscBuff);
}