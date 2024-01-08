#include "Drawable/Curve.h"

//	Constructors

Curve::Curve(Graphics& gfx, Vector3f F(float), Vector2f rangeT, UINT Npoints, Color color)
{
	std::vector<Vertex> vertexs;

	for (UINT i = 0; i <= Npoints; i++)
		vertexs.push_back(Vertex( F(rangeT.x + float(i) / Npoints * (rangeT.y - rangeT.x)).getVector4() , color.getColor4() ));

	std::vector<unsigned short> indexs;

	for (UINT i = 0; i <= Npoints; i++)
		indexs.push_back(i);

	AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));
	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));

	addDefaultBinds(gfx);
}

Curve::Curve(Graphics& gfx, Vector3f F(float), Vector2f rangeT, UINT Npoints, std::vector<Color> colors)
{
	if (!colors.size())
		throw std::exception("The color vector for a curve must have at least one color!!");

	std::vector<Vertex> vertexs;

	for (UINT i = 0; i <= Npoints; i++) {
		float c = (colors.size() - 1) * float(i) / Npoints;
		UINT C0 = UINT(c);
		UINT C1 = (C0 + 1) % colors.size();
		c -= C0;

		vertexs.push_back(Vertex(
			F(rangeT.x + float(i) / Npoints * (rangeT.y - rangeT.x)).getVector4(),
			(colors[C0] * (1 - c) + colors[C1] * c).getColor4()
		));
	}


	std::vector<unsigned short> indexs;

	for (UINT i = 0; i <= Npoints; i++)
		indexs.push_back(i);

	AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));
	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));

	addDefaultBinds(gfx);
}

Curve::Curve(Graphics& gfx, float X(float), float Y(float), float Z(float), Vector2f rangeT, UINT Npoints, Color color)
{
	std::vector<Vertex> vertexs;

	for (UINT i = 0; i <= Npoints; i++)
		vertexs.push_back(Vertex(
			Vector3f(
				X(rangeT.x + float(i) / Npoints * (rangeT.y - rangeT.x)), 
				Y(rangeT.x + float(i) / Npoints * (rangeT.y - rangeT.x)), 
				Z(rangeT.x + float(i) / Npoints * (rangeT.y - rangeT.x))
			).getVector4(),

			color.getColor4()
		));

	std::vector<unsigned short> indexs;

	for (UINT i = 0; i <= Npoints; i++)
		indexs.push_back(i);

	AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));
	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));

	addDefaultBinds(gfx);
}

Curve::Curve(Graphics& gfx, float X(float), float Y(float), float Z(float), Vector2f rangeT, UINT Npoints, std::vector<Color> colors)
{
	if (!colors.size())
		throw std::exception("The color vector for a curve must have at least one color!!");

	std::vector<Vertex> vertexs;

	for (UINT i = 0; i <= Npoints; i++) {
		float c = (colors.size() - 1) * float(i) / Npoints;
		UINT C0 = UINT(c);
		UINT C1 = (C0 + 1) % colors.size();
		c -= C0;

		vertexs.push_back(Vertex(
			Vector3f(
				X(rangeT.x + float(i) / Npoints * (rangeT.y - rangeT.x)),
				Y(rangeT.x + float(i) / Npoints * (rangeT.y - rangeT.x)),
				Z(rangeT.x + float(i) / Npoints * (rangeT.y - rangeT.x))
			).getVector4(),

			(colors[C0] * (1 - c) + colors[C1] * c).getColor4()
		));
	}

	std::vector<unsigned short> indexs;

	for (UINT i = 0; i <= Npoints; i++)
		indexs.push_back(i);

	AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));
	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));

	addDefaultBinds(gfx);
}

//	Public

void Curve::updateRotation(Graphics& gfx, float rotationX, float rotationY, float rotationZ)
{
	vscBuff.rotation = rotationQuaternion({ 1,0,0 }, rotationX) * rotationQuaternion({ 0,1,0 }, rotationY) * rotationQuaternion({ 0,0,1 }, rotationZ);

	pVSCB->Update(gfx, vscBuff);
}

void Curve::updateRotation(Graphics& gfx, Vector3f axis, float angle, bool multiplicative)
{
	if (!multiplicative)
		vscBuff.rotation = rotationQuaternion(axis, angle);
	else
		vscBuff.rotation *= rotationQuaternion(axis, angle);

	vscBuff.rotation.normalize();
	pVSCB->Update(gfx, vscBuff);
}

void Curve::updatePosition(Graphics& gfx, Vector3f position, bool additive)
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

Quaternion Curve::getRotation()
{
	return vscBuff.rotation;
}

Vector3f Curve::getPosition()
{
	return Vector3f(vscBuff.translation.x, vscBuff.translation.y, vscBuff.translation.z);
}

//	Private

void Curve::addDefaultBinds(Graphics& gfx)
{
	auto pvs = (VertexShader*)AddBind(std::move(std::make_unique<VertexShader>(gfx, SHADERS_DIR + std::wstring(L"CurveVS.cso"))));
	AddBind(std::make_unique<PixelShader>(gfx, SHADERS_DIR + std::wstring(L"CurvePS.cso")));

	std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Color",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	AddBind(std::make_unique<InputLayout>(gfx, ied, pvs->GetBytecode()));
	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP));

	pVSCB = (ConstantBuffer<VSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<VSconstBuffer>>(gfx, vscBuff, VERTEX_CONSTANT_BUFFER_TYPE));
}
