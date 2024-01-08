#include "Drawable/Triangle.h"
#include "Bindable/BindableBase.h"

Triangle::Triangle(Graphics& gfx, Vector3f* vertexs, Vector3i* triangles, UINT numT, Color* colors, bool vertexColor)
{

	create(gfx, vertexs, triangles, numT, colors, vertexColor);

}

void Triangle::create(Graphics& gfx, Vector3f* vertexs, Vector3i* triangles, UINT numT, Color* colors, bool vertexColor)
{
	if (pVSCB)
		throw std::exception("You cannot create a polihedron over one that is already initialized");

	struct Vertex {
		Vector3f vector;
		Vector3f norm;
		Color color;
	};

	Vertex* V = (Vertex*)calloc(6 * numT, sizeof(Vertex));

	for (UINT i = 0; i < numT; i++)
	{

		V[6 * i].vector = vertexs[triangles[i].x];
		V[6 * i + 1].vector = vertexs[triangles[i].y];
		V[6 * i + 2].vector = vertexs[triangles[i].z];

		V[6 * i + 3].vector = vertexs[triangles[i].y];
		V[6 * i + 4].vector = vertexs[triangles[i].x];
		V[6 * i + 5].vector = vertexs[triangles[i].z];

		if (vertexColor && colors)
		{
			V[6 * i].color = colors[triangles[i].x];
			V[6 * i + 1].color = colors[triangles[i].y];
			V[6 * i + 2].color = colors[triangles[i].z];

			V[6 * i + 3].color = colors[triangles[i].y];
			V[6 * i + 4].color = colors[triangles[i].x];
			V[6 * i + 5].color = colors[triangles[i].z];
		}

		else if (colors)
		{
			V[6 * i].color = colors[i];
			V[6 * i + 1].color = colors[i];
			V[6 * i + 2].color = colors[i];

			V[6 * i + 3].color = colors[i];
			V[6 * i + 4].color = colors[i];
			V[6 * i + 5].color = colors[i];
		}

		else
		{
			V[6 * i].color = Color::White;
			V[6 * i + 1].color = Color::White;
			V[6 * i + 2].color = Color::White;

			V[6 * i + 3].color = Color::White;
			V[6 * i + 4].color = Color::White;
			V[6 * i + 5].color = Color::White;
		}

		Vector3f norm = ((V[6 * i + 1].vector - V[6 * i].vector) * (V[6 * i + 2].vector - V[6 * i].vector)).normalize();
		V[6 * i].norm = norm;
		V[6 * i + 1].norm = norm;
		V[6 * i + 2].norm = norm;

		V[6 * i + 3].norm = -norm;
		V[6 * i + 4].norm = -norm;
		V[6 * i + 5].norm = -norm;
	}

	AddBind(std::make_unique<VertexBuffer>(gfx, V, 6 * numT));


	unsigned short* indexs = (unsigned short*)calloc(6 * numT, sizeof(unsigned short));

	for (UINT i = 0; i < 6 * numT; i++)
		indexs[i] = i;

	AddBind(std::make_unique<IndexBuffer>(gfx, indexs, 6 * numT));


	auto pvs = (VertexShader*)AddBind(std::move(std::make_unique<VertexShader>(gfx, SHADERS_DIR + std::wstring(L"TriangleVS.cso"))));

	AddBind(std::make_unique<PixelShader>(gfx, SHADERS_DIR + std::wstring(L"TrianglePS.cso")));

	std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Color",0,DXGI_FORMAT_B8G8R8A8_UNORM,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	AddBind(std::make_unique<InputLayout>(gfx, ied, pvs->GetBytecode()));

	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	pVSCB = (ConstantBuffer<VSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<VSconstBuffer>>(gfx, VERTEX_CONSTANT_BUFFER_TYPE));

	float unused = 0.f;
	pscBuff = {
		60.f,10.f,unused,unused,1.0f, 0.2f, 0.2f, 1.f , 0.f, 8.f, 8.f,unused,
		60.f,10.f,unused,unused,0.0f, 1.0f, 0.0f, 1.f , 0.f,-8.f, 8.f,unused,
		60.f,10.f,unused,unused,0.5f, 0.0f, 1.0f, 1.f ,-8.f, 0.f,-8.f,unused,
		60.f,10.f,unused,unused,1.0f, 1.0f, 0.0f, 1.f , 8.f, 0.f, 8.f,unused,
	};

	pPSCB = (ConstantBuffer<PSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<PSconstBuffer>>(gfx, pscBuff, PIXEL_CONSTANT_BUFFER_TYPE));
}

void Triangle::updateShape(Graphics& gfx, Vector3f* vertexs, Vector3i* triangles, UINT numT, Color* colors, bool vertexColor)
{
	struct Vertex {
		Vector3f vector;
		Vector3f norm;
		Color color;
	};

	Vertex* V = (Vertex*)calloc(6 * numT, sizeof(Vertex));

	for (UINT i = 0; i < numT; i++)
	{

		V[6 * i].vector = vertexs[triangles[i].x];
		V[6 * i + 1].vector = vertexs[triangles[i].y];
		V[6 * i + 2].vector = vertexs[triangles[i].z];

		V[6 * i + 3].vector = vertexs[triangles[i].y];
		V[6 * i + 4].vector = vertexs[triangles[i].x];
		V[6 * i + 5].vector = vertexs[triangles[i].z];

		if (vertexColor && colors)
		{
			V[6 * i].color = colors[triangles[i].x];
			V[6 * i + 1].color = colors[triangles[i].y];
			V[6 * i + 2].color = colors[triangles[i].z];

			V[6 * i + 3].color = colors[triangles[i].y];
			V[6 * i + 4].color = colors[triangles[i].x];
			V[6 * i + 5].color = colors[triangles[i].z];
		}

		else if (colors)
		{
			V[6 * i].color = colors[i];
			V[6 * i + 1].color = colors[i];
			V[6 * i + 2].color = colors[i];

			V[6 * i + 3].color = colors[i];
			V[6 * i + 4].color = colors[i];
			V[6 * i + 5].color = colors[i];
		}

		else
		{
			V[6 * i].color = Color::White;
			V[6 * i + 1].color = Color::White;
			V[6 * i + 2].color = Color::White;

			V[6 * i + 3].color = Color::White;
			V[6 * i + 4].color = Color::White;
			V[6 * i + 5].color = Color::White;
		}

		Vector3f norm = ((V[6 * i + 1].vector - V[6 * i].vector) * (V[6 * i + 2].vector - V[6 * i].vector)).normalize();
		V[6 * i].norm = norm;
		V[6 * i + 1].norm = norm;
		V[6 * i + 2].norm = norm;

		V[6 * i + 3].norm = -norm;
		V[6 * i + 4].norm = -norm;
		V[6 * i + 5].norm = -norm;
	}

	changeBind(std::make_unique<VertexBuffer>(gfx, V, 6 * numT), 0u);


	unsigned short* indexs = (unsigned short*)calloc(6 * numT, sizeof(unsigned short));

	for (UINT i = 0; i < 6 * numT; i++)
		indexs[i] = i;

	changeBind(std::make_unique<IndexBuffer>(gfx, indexs, 6 * numT), 1u);
}

void Triangle::updateRotation(Graphics& gfx, float rotationX, float rotationY, float rotationZ)
{
	vscBuff.rotation = rotationQuaternion({ 1,0,0 }, rotationX) * rotationQuaternion({ 0,1,0 }, rotationY) * rotationQuaternion({ 0,0,1 }, rotationZ);

	pVSCB->Update(gfx, vscBuff);
}

void Triangle::updateRotation(Graphics& gfx, Vector3f axis, float angle, bool multiplicative)
{
	if (!multiplicative)
		vscBuff.rotation = rotationQuaternion(axis, angle);
	else
		vscBuff.rotation *= rotationQuaternion(axis, angle);

	vscBuff.rotation.normalize();
	pVSCB->Update(gfx, vscBuff);
}

void Triangle::updatePosition(Graphics& gfx, Vector3f position, bool additive)
{
	if (!additive)
		vscBuff.translation = position.getVector4();
	else
	{
		vscBuff.translation.x += position.x;
		vscBuff.translation.y += position.y;
		vscBuff.translation.z += position.z;
	}

	pVSCB->Update(gfx, vscBuff);
}

void Triangle::updateLight(Graphics& gfx, UINT id, Vector2f intensity, Color color, Vector3f position)
{
	pscBuff.lightsource[id] = { intensity.getVector4() , color.getColor4() , position.getVector4() };
	pPSCB->Update(gfx, pscBuff);
}

void Triangle::updateLight(Graphics& gfx, UINT id, _float4vector intensity, _float4color color, _float4vector position)
{
	pscBuff.lightsource[id] = { intensity , color , position };
	pPSCB->Update(gfx, pscBuff);
}

void Triangle::clearLights(Graphics& gfx)
{
	pscBuff = {};
	pPSCB->Update(gfx, pscBuff);
}

Quaternion Triangle::getRotation()
{
	return vscBuff.rotation;
}

Vector3f Triangle::getPosition()
{
	return Vector3f(vscBuff.translation.x, vscBuff.translation.y, vscBuff.translation.z);
}

void Triangle::Draw(Graphics& gfx)
{
	if (!pVSCB)
		throw std::exception("You cannot draw a polihedron that hasn't yet been created");

	_draw(gfx);
}
