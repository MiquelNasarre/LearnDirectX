#include "Drawable/Triangle.h"
#include "Bindable/BindableBase.h"

Triangle::Triangle(Graphics& gfx, Color color)
{
	Position = Vector3f(rand() / 4096.f - 4.f, rand() / 4096.f - 4.f, rand() / 4096.f - 4.f);
	Velocity = Vector3f(rand() / 4096.f - 4.f, rand() / 4096.f - 4.f, rand() / 4096.f - 4.f);

	struct Vertex {
		Vector3f vector;
		Color color;
	};
	std::vector<Vertex> model;

	// set vertex colors for mesh

	model.push_back({ Vector3f(0.f, 0.f, 1.f), color });
	model.push_back({ Vector3f(cosf(2 * 3.14159f * 0 / 6) ,-sinf(2 * 3.14159f * 0 / 6), 0.f) , color });
	model.push_back({ Vector3f(cosf(2 * 3.14159f * 1 / 6) ,-sinf(2 * 3.14159f * 1 / 6), 0.f) , color });
	model.push_back({ Vector3f(cosf(2 * 3.14159f * 2 / 6) ,-sinf(2 * 3.14159f * 2 / 6), 0.f) , color });
	model.push_back({ Vector3f(cosf(2 * 3.14159f * 3 / 6) ,-sinf(2 * 3.14159f * 3 / 6), 0.f) , color });
	model.push_back({ Vector3f(cosf(2 * 3.14159f * 4 / 6) ,-sinf(2 * 3.14159f * 4 / 6), 0.f) , color });
	model.push_back({ Vector3f(cosf(2 * 3.14159f * 5 / 6) ,-sinf(2 * 3.14159f * 5 / 6), 0.f) , color });
	model.push_back({ Vector3f(0.f, 0.f,-1.f), color });

	AddBind(std::make_unique<VertexBuffer>(gfx, model));

	auto pvs = (VertexShader*)AddBind(std::move(std::make_unique<VertexShader>(gfx, SHADERS_DIR + std::wstring(L"TriangleVS.cso"))));

	AddBind(std::make_unique<PixelShader>(gfx, SHADERS_DIR + std::wstring(L"TrianglePS.cso")));

	std::vector<unsigned short> Indexs;

	for (int i = 0; i < 6; i++) {
		Indexs.push_back(0);
		Indexs.push_back(i + 1);
		Indexs.push_back((i + 1) % 6 + 1);
	}
	for (int i = 0; i < 6; i++) {
		Indexs.push_back(7);
		Indexs.push_back((i + 1) % 6 + 1);
		Indexs.push_back(i + 1);
	}

	for (UINT i = 0; i < Indexs.size() / 3; i++)
		norms.push_back(
			((model[Indexs[3 * i + 1]].vector - model[Indexs[3 * i]].vector) *
			(model[Indexs[3 * i + 2]].vector - model[Indexs[3 * i]].vector)).normalize());

	AddBind(std::make_unique<IndexBuffer>(gfx, Indexs));

	std::vector< D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Color",0,DXGI_FORMAT_B8G8R8A8_UNORM,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	AddBind(std::make_unique<InputLayout>(gfx, ied, pvs->GetBytecode()));

	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	pVSCB = (ConstantBuffer<VSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<VSconstBuffer>>(gfx, VERTEX_CONSTANT_BUFFER_TYPE));

	pPSCB = (ConstantBuffer<PSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<PSconstBuffer>>(gfx, PIXEL_CONSTANT_BUFFER_TYPE));
}

void Triangle::Update(Graphics& gfx, Vector2f ZXrotation, Vector3f position)
{
	Vector3f Translation = position;
	Matrix Rotations = ZRotationMatrix(ZXrotation.x) * XRotationMatrix(ZXrotation.y);
	
	vscBuff = {
		Translation.getVector4(),
		Rotations.transpose().getMatrix4(),
	};

	pVSCB->Update(gfx, vscBuff);

	for (UINT i = 0; i < norms.size(); i++)
		pscBuff.norm4[i] = (Rotations * norms[i]).getVector4();

	pPSCB->Update(gfx, pscBuff);
}

void Triangle::Update(Graphics& gfx, Vector2f ZXrotation, float dt)
{
	Position += Velocity * dt;
	Velocity += Acceleration * dt;
	Acceleration += dA * dt;
	dA = -Position - Velocity - Acceleration + Vector3f(rand() / 1024.f - 16.f, rand() / 1024.f - 16.f, rand() / 1024.f - 16.f);

	Vector3f Translation = Position;
	Matrix Rotations = ZRotationMatrix(ZXrotation.x) * XRotationMatrix(ZXrotation.y);

	vscBuff = {
		Translation.getVector4(),
		Rotations.transpose().getMatrix4(),
	};

	pVSCB->Update(gfx, vscBuff);

	for (UINT i = 0; i < norms.size(); i++)
		pscBuff.norm4[i] = (Rotations * norms[i]).getVector4();

	pPSCB->Update(gfx, pscBuff);
}
