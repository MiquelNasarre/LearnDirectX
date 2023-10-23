#include "Drawable/Triangle.h"
#include "Bindable/BindableBase.h"
#include "ExceptionMacros.h"


Triangle::Triangle(Graphics& gfx)
{
	struct Vertex {
		Vector3f vector;
		Color color;

		const UINT stride = sizeof(Vector3f) + sizeof(Color);
		const UINT offset = 0u;
	};
	std::vector<Vertex> model;

	// set vertex colors for mesh

	model.push_back(Vertex{ Vector3f(-1.5f, 0.f, 1.f), Color::Yellow });
	model.push_back({ Vector3f(-1.5f + cosf(2 * 3.14159f * 0 / 6) ,-sinf(2 * 3.14159f * 0 / 6), 0.f) , Color::Yellow });
	model.push_back({ Vector3f(-1.5f + cosf(2 * 3.14159f * 1 / 6) ,-sinf(2 * 3.14159f * 1 / 6), 0.f) , Color::Yellow });
	model.push_back({ Vector3f(-1.5f + cosf(2 * 3.14159f * 2 / 6) ,-sinf(2 * 3.14159f * 2 / 6), 0.f) , Color::Yellow });
	model.push_back({ Vector3f(-1.5f + cosf(2 * 3.14159f * 3 / 6) ,-sinf(2 * 3.14159f * 3 / 6), 0.f) , Color::Yellow });
	model.push_back({ Vector3f(-1.5f + cosf(2 * 3.14159f * 4 / 6) ,-sinf(2 * 3.14159f * 4 / 6), 0.f) , Color::Yellow });
	model.push_back({ Vector3f(-1.5f + cosf(2 * 3.14159f * 5 / 6) ,-sinf(2 * 3.14159f * 5 / 6), 0.f) , Color::Yellow });
	model.push_back({ Vector3f(-1.5f, 0.f,-1.f), Color::Yellow });

	model.push_back({ Vector3f(1.5f, 0.f, 1.f), Color::Blue });
	model.push_back({ Vector3f(1.5f + cosf(2 * 3.14159f * 0 / 6) ,-sinf(2 * 3.14159f * 0 / 6), 0.f) , Color::Blue });
	model.push_back({ Vector3f(1.5f + cosf(2 * 3.14159f * 1 / 6) ,-sinf(2 * 3.14159f * 1 / 6), 0.f) , Color::Blue });
	model.push_back({ Vector3f(1.5f + cosf(2 * 3.14159f * 2 / 6) ,-sinf(2 * 3.14159f * 2 / 6), 0.f) , Color::Blue });
	model.push_back({ Vector3f(1.5f + cosf(2 * 3.14159f * 3 / 6) ,-sinf(2 * 3.14159f * 3 / 6), 0.f) , Color::Blue });
	model.push_back({ Vector3f(1.5f + cosf(2 * 3.14159f * 4 / 6) ,-sinf(2 * 3.14159f * 4 / 6), 0.f) , Color::Blue });
	model.push_back({ Vector3f(1.5f + cosf(2 * 3.14159f * 5 / 6) ,-sinf(2 * 3.14159f * 5 / 6), 0.f) , Color::Blue });
	model.push_back({ Vector3f(1.5f, 0.f,-1.f), Color::Blue });

	AddBind(std::make_unique<VertexBuffer>(gfx, model));

	auto pvs = std::make_unique<VertexShader>(gfx, L"Shaders/TriangleVS.cso");
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(std::make_unique<PixelShader>(gfx, L"Shaders/TrianglePS.cso"));

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

	for (int i = 0; i < 6; i++) {
		Indexs.push_back(8);
		Indexs.push_back(i + 9);
		Indexs.push_back((i + 1) % 6 + 9);
	}
	for (int i = 0; i < 6; i++) {
		Indexs.push_back(15);
		Indexs.push_back((i + 1) % 6 + 9);
		Indexs.push_back(i + 9);
	}

	AddBind(std::make_unique<IndexBuffer>(gfx, Indexs));

	std::vector< D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Color",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	//	(Matrix transformations) Vertex shader constant buffer
	
	Vector2f pos(0,0);
	Vector3f Translation = Vector3f();
	Matrix Rotations = ZRotationMatrix(-pos.x * 3.14159f * 2.f) * XRotationMatrix(-pos.y * 3.14159f);

	Matrix Projections = ProjectionMatrix(Vector3f(0, -1, 0)) * ScalingMatrix(1.f / 640, 1.f / 480, 1.f) * 200;


	vscBuff = {
		Translation.getVector4(),
		Rotations.transpose().getMatrix4(),
		Projections.transpose().getMatrix4()
	};


	AddBind(std::make_unique<ConstantBuffer<VSconstBuffer>>(gfx, vscBuff, VERTEX_CONSTANT_BUFFER_TYPE));

	//	(light management) Pixel shader constant buffer



	AddBind(std::make_unique<ConstantBuffer<PSconstBuffer>>(gfx, pscBuff, PIXEL_CONSTANT_BUFFER_TYPE));
}

void Triangle::Update( float dt ) noexcept
{

}
