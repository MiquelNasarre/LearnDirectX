#include "Drawable/Curve.h"

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

	auto pvs = (VertexShader*)AddBind(std::move(std::make_unique<VertexShader>(gfx, L"Shaders/CurveVS.cso")));
	AddBind(std::make_unique<PixelShader>(gfx, L"Shaders/CurvePS.cso"));

	std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Color",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	AddBind(std::make_unique<InputLayout>(gfx, ied, pvs->GetBytecode()));
	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP));

	vscBuff = {
		Matrix::Identity.getMatrix4(),
		Vector3f().getVector4()
	};

	pVSCB = (ConstantBuffer<VSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<VSconstBuffer>>(gfx, vscBuff, VERTEX_CONSTANT_BUFFER_TYPE));
}

void Curve::updateRotation(Graphics& gfx, float rotationZ, float rotationX, Vector3f position)
{
	vscBuff.rotattion = (ZRotationMatrix(rotationZ) * XRotationMatrix(rotationX)).transpose().getMatrix4();
	vscBuff.traslation = position.getVector4();
	pVSCB->Update(gfx, vscBuff);
}
