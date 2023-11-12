#include "Drawable/Point.h"

Point::Point(Graphics& gfx, Vector3f position, float radius, Color color)
{
	UINT depth = 4;

	std::vector<Vertex> vertexs;

	struct triangle {
		unsigned short v0;
		unsigned short v1;
		unsigned short v2;
	};

	float gold = (1.f + sqrtf(5) / 2.f);

	vertexs.push_back(Vertex({ 0.f, 1.f, gold })); //0
	vertexs.push_back(Vertex({ 0.f, 1.f,-gold })); //1
	vertexs.push_back(Vertex({ 0.f,-1.f, gold })); //2
	vertexs.push_back(Vertex({ 0.f,-1.f,-gold })); //3
	vertexs.push_back(Vertex({ 1.f, gold, 0.f })); //4
	vertexs.push_back(Vertex({ 1.f,-gold, 0.f })); //5
	vertexs.push_back(Vertex({-1.f, gold, 0.f })); //6
	vertexs.push_back(Vertex({-1.f,-gold, 0.f })); //7
	vertexs.push_back(Vertex({ gold, 0.f, 1.f })); //8
	vertexs.push_back(Vertex({-gold, 0.f, 1.f })); //9
	vertexs.push_back(Vertex({ gold, 0.f,-1.f })); //10
	vertexs.push_back(Vertex({-gold, 0.f,-1.f })); //11

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
			vertexs.push_back({ (vertexs[t.v0].Normal + vertexs[t.v1].Normal) / 2.f });
			vertexs.push_back({ (vertexs[t.v1].Normal + vertexs[t.v2].Normal) / 2.f });
			vertexs.push_back({ (vertexs[t.v2].Normal + vertexs[t.v0].Normal) / 2.f });
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
		v.Normal.normalize();

	AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));
	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));

	auto pvs = (VertexShader*)AddBind(std::move(std::make_unique<VertexShader>(gfx, SHADERS_DIR + std::wstring(L"PointVS.cso"))));
	AddBind(std::make_unique<PixelShader>(gfx, SHADERS_DIR + std::wstring(L"PointPS.cso")));

	std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Normal",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	AddBind(std::make_unique<InputLayout>(gfx, ied, pvs->GetBytecode()));
	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	vscBuff = { position.getVector4(), radius, gfx.getScale() };
	pscBuff = { color.getColor4() };

	pVSCB = (ConstantBuffer<VSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<VSconstBuffer>>(gfx, vscBuff, VERTEX_CONSTANT_BUFFER_TYPE));
	pPSCB = (ConstantBuffer<PSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<PSconstBuffer>>(gfx, pscBuff, PIXEL_CONSTANT_BUFFER_TYPE));
}

void Point::updatePosition(Graphics& gfx, Vector3f position)
{
	vscBuff.position = position.getVector4();
	pVSCB->Update(gfx, &vscBuff);
}

void Point::updateRadius(Graphics& gfx, float radius)
{
	vscBuff.radius = radius;
	pVSCB->Update(gfx, &vscBuff);
}

void Point::updateColor(Graphics& gfx, Color col)
{
	pscBuff.color = col.getColor4();
	pPSCB->Update(gfx, &pscBuff);
}

void Point::Draw(Graphics& gfx)
{
	vscBuff.scale = gfx.getScale();
	pVSCB->Update(gfx, &vscBuff);

	_draw(gfx);
}
