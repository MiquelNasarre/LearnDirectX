#include "Drawable/Surface.h"

Surface::Surface(Graphics& gfx, float F(float, float), Vector2f minRect, Vector2f maxRect, UINT numX, UINT numY)
{
	float epsilon = 0.001f;

	std::vector<Vertex> vertexs;
	std::vector<unsigned short> indexs;

	for (UINT i = 0; i < numX + 1; i++) {
		for (UINT j = 0; j < numY + 1; j++) {
			float x = ((numX - i) * minRect.x + i * maxRect.x) / numX;
			float y = ((numY - j) * minRect.y + j * maxRect.y) / numY;
			vertexs.push_back({ Vector3f(x, y, F(x,y)) ,       ((Vector3f(x + epsilon, y, F(x + epsilon,y)) - Vector3f(x - epsilon, y, F(x - epsilon,y))) * (Vector3f(x, y + epsilon, F(x,y + epsilon)) - Vector3f(x, y - epsilon, F(x,y - epsilon)))).normalize()    });
		}
	}

	for (UINT i = 0; i < numX; i++) {
		for (UINT j = 0; j < numY; j++) {
			indexs.push_back(i * (numY + 1) + j);
			indexs.push_back(i * (numY + 1) + j + 1);
			indexs.push_back((i + 1) * (numY + 1) + j + 1);
			
			indexs.push_back(i * (numY + 1) + j);
			indexs.push_back((i + 1) * (numY + 1) + j + 1);
			indexs.push_back((i + 1) * (numY + 1) + j);

			indexs.push_back(i * (numY + 1) + j);
			indexs.push_back((i + 1) * (numY + 1) + j + 1);
			indexs.push_back(i * (numY + 1) + j + 1);

			indexs.push_back(i * (numY + 1) + j);
			indexs.push_back((i + 1) * (numY + 1) + j);
			indexs.push_back((i + 1) * (numY + 1) + j + 1);
		}
	}

	AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));

	auto pvs = (VertexShader*)AddBind(std::move(std::make_unique<VertexShader>(gfx, L"Shaders/SurfaceVS.cso")));

	AddBind(std::make_unique<PixelShader>(gfx, L"Shaders/SurfacePS.cso"));

	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));

	std::vector< D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	AddBind(std::make_unique<InputLayout>(gfx, ied, pvs->GetBytecode()));

	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
}
