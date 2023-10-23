#include "Drawable/Sphere.h"

Sphere::Sphere(Graphics& gfx, Color color, UINT depth)
{

	Model model = generateIcosphere(depth, color);

	AddBind(std::make_unique<VertexBuffer>(gfx, model.vertexs));

	AddBind(std::make_unique<IndexBuffer>(gfx, model.indexs));

	auto pvs = (VertexShader*)AddBind(std::move(std::make_unique<VertexShader>(gfx, L"Shaders/SphereVS.cso")));

	AddBind(std::make_unique<PixelShader>(gfx, L"Shaders/SpherePS.cso"));

	std::vector< D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Color",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied, pvs->GetBytecode()));

	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	pVSCB = (ConstantBuffer<VSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<VSconstBuffer>>(gfx, VERTEX_CONSTANT_BUFFER_TYPE));

	pPSCB = (ConstantBuffer<_float4vector>*)AddBind(std::make_unique<ConstantBuffer<_float4vector>>(gfx, PIXEL_CONSTANT_BUFFER_TYPE));

}

void Sphere::Update(Graphics& gfx, Vector3f position)
{
	Vector3f Translation = position;

	vscBuff = {
		Translation.getVector4(),
	};

	pVSCB->Update(gfx, vscBuff);

	pPSCB->Update(gfx, vscBuff.translation);
}

Sphere::Model Sphere::generateIcosphere(UINT depth, Color color)
{
	std::vector<Vertex> vertexs;

	struct triangle {
		unsigned short v0;
		unsigned short v1;
		unsigned short v2;
	};

	float gold = (1.f + sqrtf(5) / 2.f);

	vertexs.push_back(Vertex({ 0.f, 1.f, gold }, color)); //0
	vertexs.push_back(Vertex({ 0.f, 1.f,-gold }, color)); //1
	vertexs.push_back(Vertex({ 0.f,-1.f, gold }, color)); //2
	vertexs.push_back(Vertex({ 0.f,-1.f,-gold }, color)); //3
	vertexs.push_back(Vertex({ 1.f, gold, 0.f }, color)); //4
	vertexs.push_back(Vertex({ 1.f,-gold, 0.f }, color)); //5
	vertexs.push_back(Vertex({ -1.f, gold, 0.f }, color)); //6
	vertexs.push_back(Vertex({ -1.f,-gold, 0.f }, color)); //7
	vertexs.push_back(Vertex({ gold, 0.f, 1.f }, color)); //8
	vertexs.push_back(Vertex({ -gold, 0.f, 1.f }, color)); //9
	vertexs.push_back(Vertex({ gold, 0.f,-1.f }, color)); //10
	vertexs.push_back(Vertex({ -gold, 0.f,-1.f }, color)); //11

	std::vector<triangle> triangles;
	std::vector<triangle> newTriangles;

	triangles.push_back({ 0,6,4 });
	triangles.push_back({ 1,4,6 });
	triangles.push_back({ 2,5,7 });
	triangles.push_back({ 3,7,5 });
	triangles.push_back({ 4,10,8 });
	triangles.push_back({ 5,8,10 });
	triangles.push_back({ 6,9,11 });
	triangles.push_back({ 7,11,9 });
	triangles.push_back({ 8,2,0 });
	triangles.push_back({ 9,0,2 });
	triangles.push_back({ 10,1,3 });
	triangles.push_back({ 11,3,1 });
	triangles.push_back({ 0,4,8 });
	triangles.push_back({ 0,9,6 });
	triangles.push_back({ 4,1,10 });
	triangles.push_back({ 6,11,1 });
	triangles.push_back({ 3,11,7 });
	triangles.push_back({ 3,5,10 });
	triangles.push_back({ 7,9,2 });
	triangles.push_back({ 5,2,8 });

	for (UINT i = 0; i < depth; i++) {
		newTriangles.clear();
		for (triangle& t : triangles) {
			vertexs.push_back({ (vertexs[t.v0].vector + vertexs[t.v1].vector) / 2.f , color });
			vertexs.push_back({ (vertexs[t.v1].vector + vertexs[t.v2].vector) / 2.f , color });
			vertexs.push_back({ (vertexs[t.v2].vector + vertexs[t.v0].vector) / 2.f , color });
			unsigned short s0 = (unsigned short)vertexs.size() - 3;
			unsigned short s1 = s0 + 1;
			unsigned short s2 = s0 + 2;
			newTriangles.push_back({ t.v0 , s0 , s2 });
			newTriangles.push_back({ t.v1 , s1 , s0 });
			newTriangles.push_back({ t.v2 , s2 , s1 });
			newTriangles.push_back({ s0 , s1 , s2 });
		}
		triangles = newTriangles;
	}
	

	std::vector<unsigned short> indexs;
	for (triangle& t : triangles) {
		indexs.push_back(t.v0);
		indexs.push_back(t.v1);
		indexs.push_back(t.v2);
	}
	for (Vertex& v : vertexs)
		v.vector.normalize();
	return { vertexs,indexs };
}
