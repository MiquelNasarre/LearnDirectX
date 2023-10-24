#include "Drawable/Surface.h"

Surface::Surface(Graphics& gfx, Type0 type, float F(float, float), Vector2f minRect, Vector2f maxRect, UINT numX, UINT numY)
{
	generateExplicit(gfx, F, minRect, maxRect, numX, numY);
	addDefaultBinds(gfx);
	
}

Surface::Surface(Graphics& gfx, Type1 TYPE_FLAG, float F(float, float), UINT numX, UINT numY, Vector2f minRect, Vector2f maxRect)
{
	generatePolarNormal(gfx, F, minRect, maxRect, numX, numY);
	addDefaultBinds(gfx);
}

Surface::Surface(Graphics& gfx, Type2 TYPE_FLAG, float r(float, float), UINT depth)
{
	generatePolarIcosphere(gfx, r, depth);
	addDefaultBinds(gfx);
}

Surface::Surface(Graphics& gfx, Type3 TYPE_FLAG, float x(float, float), float y(float, float), float z(float, float), Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV)
{
	generateParametric(gfx, x, y, z, minRect, maxRect, numU, numV);
	addDefaultBinds(gfx);
}

Surface::Surface(Graphics& gfx, Type4 TYPE_FLAG, float theta(float, float), float phi(float, float), float rad(float, float), Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV)
{
	generatePolarParametric(gfx, theta, phi, rad, minRect, maxRect, numU, numV);
	addDefaultBinds(gfx);
}

void Surface::generateExplicit(Graphics& gfx, float F(float, float), Vector2f minRect, Vector2f maxRect, UINT numX, UINT numY)
{
	float epsilon = error_epsilon;

	std::vector<Vertex> vertexs;
	std::vector<unsigned short> indexs;

	for (UINT i = 0; i < numX + 1; i++) {
		for (UINT j = 0; j < numY + 1; j++) {
			float x = ((numX - i) * minRect.x + i * maxRect.x) / numX;
			float y = ((numY - j) * minRect.y + j * maxRect.y) / numY;
			vertexs.push_back({ Vector3f(x, y, F(x,y)) ,
				-((Vector3f(x + epsilon, y, F(x + epsilon,y)) - Vector3f(x - epsilon, y, F(x - epsilon,y))) * 
				(Vector3f(x, y + epsilon, F(x,y + epsilon)) - Vector3f(x, y - epsilon, F(x,y - epsilon)))).normalize() });
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

	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));
}

void Surface::generatePolarNormal(Graphics& gfx, float r(float, float), Vector2f minRect, Vector2f maxRect, UINT numX, UINT numY)
{
	float epsilon = error_epsilon;

	std::vector<Vertex> vertexs;
	std::vector<unsigned short> indexs;

	for (UINT i = 0; i < numX + 1; i++) {
		for (UINT j = 0; j < numY + 1; j++) {
			float theta = ((numX - i) * minRect.x + i * maxRect.x) / numX;
			float phi = ((numY - j) * minRect.y + j * maxRect.y) / numY;
			vertexs.push_back({ evalPolar(r,theta,phi) ,

				-((evalPolar(r,theta + epsilon,phi) - evalPolar(r,theta - epsilon,phi)) *
				((evalPolar(r,theta,phi + epsilon) - evalPolar(r,theta,phi - epsilon))))
				.normalize() });
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

	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));
	
}

void Surface::generatePolarIcosphere(Graphics& gfx, float r(float, float), UINT depth)
{
	float epsilon = error_epsilon;
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
	vertexs.push_back(Vertex({ -1.f, gold, 0.f })); //6
	vertexs.push_back(Vertex({ -1.f,-gold, 0.f })); //7
	vertexs.push_back(Vertex({ gold, 0.f, 1.f })); //8
	vertexs.push_back(Vertex({ -gold, 0.f, 1.f })); //9
	vertexs.push_back(Vertex({ gold, 0.f,-1.f })); //10
	vertexs.push_back(Vertex({ -gold, 0.f,-1.f })); //11

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
			vertexs.push_back({ (vertexs[t.v0].vector + vertexs[t.v1].vector) / 2.f });
			vertexs.push_back({ (vertexs[t.v1].vector + vertexs[t.v2].vector) / 2.f });
			vertexs.push_back({ (vertexs[t.v2].vector + vertexs[t.v0].vector) / 2.f });
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
	for (Vertex& v : vertexs) {
		v.vector.normalize();
		float phi = asinf(v.vector.z);
		float theta = 0.f;
		if (v.vector.x || v.vector.y) {
			Vector2f temp = Vector2f(v.vector.x, v.vector.y).normalize();
			theta = acosf(temp.x);
			if (temp.y < 0)
				theta = 2 * pi - theta;
		}
		v.vector *= r(theta, phi);
		v.norm =
			-((evalPolar(r, theta + epsilon, phi) - evalPolar(r, theta - epsilon, phi)) *
			((evalPolar(r, theta, phi + epsilon) - evalPolar(r, theta, phi - epsilon))))
			.normalize();
	}
		
	AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));

	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));

}

void Surface::generateParametric(Graphics& gfx, float x(float, float), float y(float, float), float z(float, float), Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV)
{
	float epsilon = error_epsilon;

	std::vector<Vertex> vertexs;
	std::vector<unsigned short> indexs;

	for (UINT i = 0; i < numU + 1; i++) {
		for (UINT j = 0; j < numV + 1; j++) {
			float u = ((numU - i) * minRect.x + i * maxRect.x) / numU;
			float v = ((numV - j) * minRect.y + j * maxRect.y) / numV;
			vertexs.push_back({ Vector3f(x(u,v), y(u,v), z(u,v)) ,
				-((Vector3f(x(u + epsilon,v), y(u + epsilon,v), z(u + epsilon,v)) - Vector3f(x(u - epsilon,v), y(u - epsilon,v), z(u - epsilon,v))) *
				(Vector3f(x(u,v + epsilon), y(u,v + epsilon), z(u,v + epsilon)) - Vector3f(x(u,v - epsilon), y(u,v - epsilon), z(u,v - epsilon))))
				.normalize() });
		}
	}

	for (UINT i = 0; i < numU; i++) {
		for (UINT j = 0; j < numV; j++) {
			indexs.push_back(i * (numV + 1) + j);
			indexs.push_back(i * (numV + 1) + j + 1);
			indexs.push_back((i + 1) * (numV + 1) + j + 1);

			indexs.push_back(i * (numV + 1) + j);
			indexs.push_back((i + 1) * (numV + 1) + j + 1);
			indexs.push_back((i + 1) * (numV + 1) + j);

			indexs.push_back(i * (numV + 1) + j);
			indexs.push_back((i + 1) * (numV + 1) + j + 1);
			indexs.push_back(i * (numV + 1) + j + 1);

			indexs.push_back(i * (numV + 1) + j);
			indexs.push_back((i + 1) * (numV + 1) + j);
			indexs.push_back((i + 1) * (numV + 1) + j + 1);
		}
	}

	AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));

	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));
}

void Surface::generatePolarParametric(Graphics& gfx, float theta(float, float), float phi(float, float), float rad(float, float), Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV)
{
	float epsilon = error_epsilon;

	std::vector<Vertex> vertexs;
	std::vector<unsigned short> indexs;

	for (UINT i = 0; i < numU + 1; i++) {
		for (UINT j = 0; j < numV + 1; j++) {
			float u = ((numU - i) * minRect.x + i * maxRect.x) / numU;
			float v = ((numV - j) * minRect.y + j * maxRect.y) / numV;
			vertexs.push_back({ rad(u,v) * Vector3f(cosf(theta(u,v)) * cosf(phi(u,v)), sinf(theta(u,v)) * cosf(phi(u,v)), sinf(phi(u,v))) ,

				-((rad(u + epsilon,v) * Vector3f(cosf(theta(u + epsilon,v)) * cosf(phi(u + epsilon,v)), sinf(theta(u + epsilon,v)) * cosf(phi(u + epsilon,v)), sinf(phi(u + epsilon,v))) -
				rad(u + epsilon,v) * Vector3f(cosf(theta(u - epsilon,v)) * cosf(phi(u - epsilon,v)), sinf(theta(u - epsilon,v)) * cosf(phi(u - epsilon,v)), sinf(phi(u - epsilon,v)))) *
				((rad(u,v + epsilon) * Vector3f(cosf(theta(u,v + epsilon)) * cosf(phi(u,v + epsilon)), sinf(theta(u,v + epsilon)) * cosf(phi(u,v + epsilon)), sinf(phi(u,v + epsilon))) -
				rad(u,v + epsilon) * Vector3f(cosf(theta(u,v - epsilon)) * cosf(phi(u,v - epsilon)), sinf(theta(u,v - epsilon)) * cosf(phi(u,v - epsilon)), sinf(phi(u,v - epsilon))))))
				.normalize() });
		}
	}

	for (UINT i = 0; i < numU; i++) {
		for (UINT j = 0; j < numV; j++) {
			indexs.push_back(i * (numV + 1) + j);
			indexs.push_back(i * (numV + 1) + j + 1);
			indexs.push_back((i + 1) * (numV + 1) + j + 1);

			indexs.push_back(i * (numV + 1) + j);
			indexs.push_back((i + 1) * (numV + 1) + j + 1);
			indexs.push_back((i + 1) * (numV + 1) + j);

			indexs.push_back(i * (numV + 1) + j);
			indexs.push_back((i + 1) * (numV + 1) + j + 1);
			indexs.push_back(i * (numV + 1) + j + 1);

			indexs.push_back(i * (numV + 1) + j);
			indexs.push_back((i + 1) * (numV + 1) + j);
			indexs.push_back((i + 1) * (numV + 1) + j + 1);
		}
	}

	AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));

	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));
}

void Surface::addDefaultBinds(Graphics& gfx)
{
	auto pvs = (VertexShader*)AddBind(std::move(std::make_unique<VertexShader>(gfx, L"Shaders/SurfaceVS.cso")));

	AddBind(std::make_unique<PixelShader>(gfx, L"Shaders/SurfacePS.cso"));


	std::vector< D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	AddBind(std::make_unique<InputLayout>(gfx, ied, pvs->GetBytecode()));

	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	pVSCB = (ConstantBuffer<_float4matrix>*)AddBind(std::make_unique<ConstantBuffer<_float4matrix>>(gfx, Matrix::Identity.getMatrix4(), VERTEX_CONSTANT_BUFFER_TYPE));
}

void Surface::updateRotation(Graphics& gfx, float rotationZ, float rotationX)
{
	pVSCB->Update(gfx, (ZRotationMatrix(rotationZ) * XRotationMatrix(rotationX)).transpose().getMatrix4());
}

Vector3f Surface::evalPolar(float r(float, float), float theta, float phi)
{
	return r(theta, phi) * Vector3f(cosf(theta) * cosf(phi), sinf(theta) * cosf(phi), sinf(phi));
}
