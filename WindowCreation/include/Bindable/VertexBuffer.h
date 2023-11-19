#pragma once
#include "Bindable.h"

class VertexBuffer : public Bindable
{
public:
	template<typename V>
	VertexBuffer(Graphics& gfx, const std::vector<V>& vertices);

	template<typename V>
	VertexBuffer(Graphics& gfx, V* vertices, UINT size);

	void Bind(Graphics& gfx) override;

private:
	UINT stride;
	pCom<ID3D11Buffer> pVertexBuffer;
};

template<typename V>
VertexBuffer::VertexBuffer(Graphics& gfx, const std::vector<V>& vertices)
	:stride(sizeof(V))
{
	//INFOMAN(gfx);

	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = UINT(sizeof(V) * vertices.size());
	bd.StructureByteStride = sizeof(V);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices.data();
	//GFX_THROW_INFO();
	GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer);
}

template<typename V>
inline VertexBuffer::VertexBuffer(Graphics& gfx, V* vertices, UINT size)
	:stride(sizeof(V))
{
	//INFOMAN(gfx);

	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = UINT(sizeof(V) * size);
	bd.StructureByteStride = sizeof(V);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	//GFX_THROW_INFO();
	GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer);
}
