#pragma once
#include "Drawable.h"

#define error_epsilon	0.001f

enum SURFACE_TYPE {
	_EXPLICIT,
	_IMPLICIT,
	_PARAMETRIC,
	_RADIAL_ICOSPHERE,
	_RADIAL_SPHERICAL,
	_PARAMETRIC_SPHERICAL,
	_IMPLICIT_SPHERICAL
};

struct SURFACE_COLORING
{
	bool Textured = false;

	Texture* texture = NULL;
	Texture* textureNight = NULL;
	Color color = Color::White;

	bool Lighted = true;
	bool DefaultInitialLights = true;

	struct LIGHTSOURCE {
		_float4vector intensity	= { 0.f,0.f,0.f,0.f };
		_float4color  color		= { 1.f,1.f,1.f,1.f };
		_float4vector position	= { 0.f,0.f,0.f,0.f };
	}lightsource[8];

	SURFACE_COLORING() {}
	SURFACE_COLORING(Texture* ptexture, Texture* ptextureNight = NULL, bool lighted = true)
	{
		Lighted = lighted;
		Textured = true;
		texture = ptexture;
		if (ptextureNight)
			textureNight = ptextureNight;
	}
	SURFACE_COLORING(Texture& ptexture, Texture& ptextureNight, bool lighted = true)
	{
		Lighted = lighted;
		Textured = true;
		texture = &ptexture;
		textureNight = &ptextureNight;
	}
	SURFACE_COLORING(Texture& ptexture, bool lighted = true)
	{
		Lighted = lighted;
		Textured = true;
		texture = &ptexture;
	}
	SURFACE_COLORING(Color col, bool lighted = true)
	{
		Lighted = lighted;
		color = col;
	}
};

class Surface : public Drawable
{
public:
	//	Explicit & Radial spherical

	Surface(Graphics& gfx, SURFACE_TYPE Type, float F0(float, float), SURFACE_COLORING sc = {}, bool defaultValues = true, Vector2f minRect = {}, Vector2f maxRect = {}, UINT numX = 100u, UINT numY = 100u);

	template<typename C>
	Surface(Graphics& gfx, SURFACE_TYPE Type, float F0(float, float, const C&), const C& param, SURFACE_COLORING sc = {}, bool defaultValues = true, Vector2f minRect = {}, Vector2f maxRect = {}, UINT numX = 100u, UINT numY = 100u);

	//	Radial icosphere

	Surface(Graphics& gfx, SURFACE_TYPE Type, float rad(float, float), UINT ICOSPHERE_DEPTH, SURFACE_COLORING sc = {});

	template<typename C>
	Surface(Graphics& gfx, SURFACE_TYPE Type, float rad(float, float, const C&), const C& param, UINT ICOSPHERE_DEPTH, SURFACE_COLORING sc = {});

	//	Parametric & Parametric spherical

	Surface(Graphics& gfx, SURFACE_TYPE Type, float F0(float, float), float F1(float, float), float F2(float, float), SURFACE_COLORING sc = {}, bool defaultValues = true, Vector2f minRect = {}, Vector2f maxRect = {}, UINT numU = 100u, UINT numV = 100u);

	Surface(Graphics& gfx, SURFACE_TYPE Type, Vector3f P(float, float), SURFACE_COLORING sc = {}, bool defaultValues = true, Vector2f minRect = {}, Vector2f maxRect = {}, UINT numU = 100u, UINT numV = 100u);

	template<typename C>
	Surface(Graphics& gfx, SURFACE_TYPE Type, float F0(float, float, const C&), float F1(float, float, const C&), float F2(float, float, const C&),  const C& param, SURFACE_COLORING sc = {},bool defaultValues = true, Vector2f minRect = {}, Vector2f maxRect = {}, UINT numU = 100u, UINT numV = 100u);

	template<typename C>
	Surface(Graphics& gfx, SURFACE_TYPE Type, Vector3f P(float, float, const C&), const C& param, SURFACE_COLORING sc = {}, bool defaultValues = true, Vector2f minRect = {}, Vector2f maxRect = {}, UINT numU = 100u, UINT numV = 100u);

	//	Implicit & Implicit spherical

	Surface(Graphics& gfx, SURFACE_TYPE Type, float H(float, float, float), SURFACE_COLORING sc = {}, Vector3f regionBegin = { -2.f, -2.f, -2.f }, Vector3f regionEnd = { 2.f, 2.f, 2.f });

	template<typename C>
	Surface(Graphics& gfx, SURFACE_TYPE Type, float H(float, float, float, const C&), const C& param, SURFACE_COLORING sc = {}, Vector3f regionBegin = { -2.f, -2.f, -2.f }, Vector3f regionEnd = { 2.f, 2.f, 2.f });

	//	Public functions

	void updateRotation(Graphics& gfx, float rotationZ, float rotationX, Vector3f position = Vector3f());
	void updateTexture(Graphics& gfx, UINT id, std::string texture);
	void updateTexture(Graphics& gfx, UINT id, Texture texture);
	void updateTextures(Graphics& gfx, Texture texture0, Texture texture1);
	void updateTextures(Graphics& gfx, std::string texture0, std::string texture1);
	void updateLight(Graphics& gfx, UINT id, Vector2f intensity, Color color, Vector3f position);
	void updateLight(Graphics& gfx, UINT id, _float4vector intensity, _float4color color, _float4vector position);
	void clearLights(Graphics& gfx);

private:
	void generateExplicit(Graphics& gfx, float F(float, float), Vector2f minRect, Vector2f maxRect, UINT numX, UINT numY, SURFACE_COLORING sc);
	void generatePolarNormal(Graphics& gfx, float rad(float, float), Vector2f minRect, Vector2f maxRect, UINT numX, UINT numY, SURFACE_COLORING sc);
	void generatePolarIcosphere(Graphics& gfx, float rad(float, float), UINT depth, SURFACE_COLORING sc);
	void generateParametric(Graphics& gfx, float x(float, float), float y(float, float), float z(float, float), Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV, SURFACE_COLORING sc);
	void generateParametric(Graphics& gfx, Vector3f P(float, float), Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV, SURFACE_COLORING sc);
	void generatePolarParametric(Graphics& gfx, float theta(float, float), float phi(float, float), float rad(float, float), Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV, SURFACE_COLORING sc);
	void generatePolarParametric(Graphics& gfx, Vector3f P(float, float), Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV, SURFACE_COLORING sc);
	void generateImplicit(Graphics& gfx, float H(float, float, float), SURFACE_COLORING sc, Vector3f regionBegin, Vector3f regionEnd);
	void generateImplicitPolar(Graphics& gfx, float H(float, float, float), SURFACE_COLORING sc, Vector3f regionBegin, Vector3f regionEnd);

	template<typename C>
	void generateExplicit(Graphics& gfx, float F(float, float, const C&), const C& param, Vector2f minRect, Vector2f maxRect, UINT numX, UINT numY, SURFACE_COLORING sc);
	template<typename C>
	void generatePolarNormal(Graphics& gfx, float rad(float, float, const C&), const C& param, Vector2f minRect, Vector2f maxRect, UINT numX, UINT numY, SURFACE_COLORING sc);
	template<typename C>
	void generatePolarIcosphere(Graphics& gfx, float rad(float, float, const C&), const C& param, UINT depth, SURFACE_COLORING sc);
	template<typename C>
	void generateParametric(Graphics& gfx, float x(float, float, const C&), float y(float, float, const C&), float z(float, float, const C&), const C& param, Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV, SURFACE_COLORING sc);
	template<typename C>
	void generateParametric(Graphics& gfx, Vector3f P(float, float, const C&), const C& param, Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV, SURFACE_COLORING sc);
	template<typename C>
	void generatePolarParametric(Graphics& gfx, float theta(float, float, const C&), float phi(float, float, const C&), float rad(float, float, const C&), const C& param, Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV, SURFACE_COLORING sc);
	template<typename C>
	void generatePolarParametric(Graphics& gfx, Vector3f P(float, float, const C&), const C& param, Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV, SURFACE_COLORING sc);
	template<typename C>
	void generateImplicit(Graphics& gfx, float H(float, float, float, const C&), const C& param, SURFACE_COLORING sc, Vector3f regionBegin, Vector3f regionEnd);
	template<typename C>
	void generateImplicitPolar(Graphics& gfx, float H(float, float, float, const C&), const C& param, SURFACE_COLORING sc, Vector3f regionBegin, Vector3f regionEnd);

	void addOtherBinds(Graphics& gfx, SURFACE_COLORING sc);

	struct VSconstBuffer {
		_float4matrix rotattion;
		_float4vector traslation;
	}vscBuff;

	struct PSconstBuffer {
		SURFACE_COLORING::LIGHTSOURCE lightsource[8];
	}pscBuff;

	ConstantBuffer<VSconstBuffer>* pVSCB = NULL;
	ConstantBuffer<PSconstBuffer>* pPSCB = NULL;
	bool textured = false;


	struct Vertex {
		Vector3f vector;
		Vector3f norm;
		Color color;
	};

	struct TexVertex {
		Vector3f vector;
		Vector3f norm;
		Vector2f texCoor;
	};

	Vector3f evalPolar(float r(float, float), float theta, float phi);
	Vector3f makePolar(Vector3f other);
	void addVertexsCube(_float4vector cube[8], std::vector<Vertex>& vertexs, std::vector<unsigned short>& indexs, SURFACE_COLORING sc, bool polar = false);

};

//	Templated stuff

template<typename C>
Surface::Surface(Graphics& gfx, SURFACE_TYPE Type, float F0(float, float, const C&), const C& param, SURFACE_COLORING sc, bool defaultValues, Vector2f minRect, Vector2f maxRect, UINT numX, UINT numY)
{
	switch (Type)
	{
	case _EXPLICIT:
		if (defaultValues) {
			minRect = { -1.f,-1.f };
			maxRect = { 1.f, 1.f };
		}
		generateExplicit(gfx, F0, param, minRect, maxRect, numX, numY, sc);
		break;
	case _RADIAL_SPHERICAL:
		if (defaultValues) {
			minRect = { 0.f, pi / 2.f };
			maxRect = { 2.f * pi, -pi / 2.f };
		}
		generatePolarNormal(gfx, F0, param, minRect, maxRect, numX, numY, sc);
		break;
	case _IMPLICIT:
		throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create an Implicit surface you must provide just one function that takes three arguments");
	case _PARAMETRIC:
		throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create a Parametric surface you must provide three functions that take two arguments");
	case _RADIAL_ICOSPHERE:
		throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create a Radial Icosphere surface you must specify the depth of the icosphere\n(depth 5 recomended)");
	case _PARAMETRIC_SPHERICAL:
		throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create a Parametric Spherical surface you must provide three functions that take two arguments");
	case _IMPLICIT_SPHERICAL:
		throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create an Implicit Spherical surface you must provide just one function that takes three arguments");
	default:
		throw std::exception("The surface type specified is not supported");
	}
	addOtherBinds(gfx, sc);
}

template<typename C>
Surface::Surface(Graphics& gfx, SURFACE_TYPE Type, float rad(float, float, const C&), const C& param, UINT ICOSPHERE_DEPTH, SURFACE_COLORING sc)
{
	switch (Type)
	{
	case _RADIAL_ICOSPHERE:
		generatePolarIcosphere(gfx, rad, param, ICOSPHERE_DEPTH, sc);
		break;
	case _EXPLICIT:
		throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create an Explicit surface you must remove the last value that provides a depth for the icosphere");
	case _RADIAL_SPHERICAL:
		throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create a Radial Spherical surface you must remove the last value that provides a depth for the icosphere");
	case _IMPLICIT:
		throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create an Implicit surface you must provide just one function that takes three arguments");
	case _PARAMETRIC:
		throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create a Parametric surface you must provide three functions that take two arguments");
	case _PARAMETRIC_SPHERICAL:
		throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create a Parametric Spherical surface you must provide three functions that take two arguments");
	case _IMPLICIT_SPHERICAL:
		throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create an Implicit Spherical surface you must provide just one function that takes three arguments");
	default:
		throw std::exception("The surface type specified is not supported");
	}
	addOtherBinds(gfx, sc);
}

template<typename C>
Surface::Surface(Graphics& gfx, SURFACE_TYPE Type, float F0(float, float, const C&), float F1(float, float, const C&), float F2(float, float, const C&), const C& param, SURFACE_COLORING sc, bool defaultValues, Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV)
{
	switch (Type)
	{
	case _PARAMETRIC:
		if (defaultValues) {
			minRect = { -1.f,-1.f };
			maxRect = { 1.f, 1.f };
		}
		generateParametric(gfx, F0, F1, F2, param, minRect, maxRect, numU, numV, sc);
		break;
	case _PARAMETRIC_SPHERICAL:
		if (defaultValues) {
			minRect = { 0.f, pi / 2.f };
			maxRect = { 2.f * pi, -pi / 2.f };
		}
		generatePolarParametric(gfx, F0, F1, F2, param, minRect, maxRect, numU, numV, sc);
		break;
	case _RADIAL_ICOSPHERE:
		throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create a Radial Icosphere surface you must provide a single function that take two arguments and a depth value");
	case _EXPLICIT:
		throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create an Explicit surface you must provide a single function that take two arguments");
	case _RADIAL_SPHERICAL:
		throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create a Radial Spherical surface you must provide a single function that take two arguments");
	case _IMPLICIT:
		throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create an Implicit surface you must provide just one function that takes three arguments");
	case _IMPLICIT_SPHERICAL:
		throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create an Implicit Spherical surface you must provide just one function that takes three arguments");
	default:
		throw std::exception("The surface type specified is not supported");
	}
	addOtherBinds(gfx, sc);
}

template<typename C>
Surface::Surface(Graphics& gfx, SURFACE_TYPE Type, Vector3f P(float, float, const C&), const C& param, SURFACE_COLORING sc, bool defaultValues, Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV)
{
	switch (Type)
	{
	case _PARAMETRIC:
		if (defaultValues) {
			minRect = { -1.f,-1.f };
			maxRect = { 1.f, 1.f };
		}
		generateParametric(gfx, P, param, minRect, maxRect, numU, numV, sc);
		break;
	case _PARAMETRIC_SPHERICAL:
		if (defaultValues) {
			minRect = { 0.f, pi / 2.f };
			maxRect = { 2.f * pi, -pi / 2.f };
		}
		generatePolarParametric(gfx, P, param, minRect, maxRect, numU, numV, sc);
		break;
	case _RADIAL_ICOSPHERE:
		throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create a Radial Icosphere surface you must provide a single function that take two arguments and a depth value");
	case _EXPLICIT:
		throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create an Explicit surface you must provide a single function that take two arguments");
	case _RADIAL_SPHERICAL:
		throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create a Radial Spherical surface you must provide a single function that take two arguments");
	case _IMPLICIT:
		throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create an Implicit surface you must provide just one function that takes three arguments");
	case _IMPLICIT_SPHERICAL:
		throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create an Implicit Spherical surface you must provide just one function that takes three arguments");
	default:
		throw std::exception("The surface type specified is not supported");
	}
	addOtherBinds(gfx, sc);
}

template<typename C>
Surface::Surface(Graphics& gfx, SURFACE_TYPE Type, float H(float, float, float, const C&), const C& param, SURFACE_COLORING sc, Vector3f regionBegin, Vector3f regionEnd)
{
	switch (Type)
	{
	case _IMPLICIT:
		generateImplicit(gfx, H, param, sc, regionBegin, regionEnd);
		break;
	case _IMPLICIT_SPHERICAL:
		generateImplicitPolar(gfx, H, param, sc, regionBegin, regionEnd);
		break;
	case _PARAMETRIC:
		throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create a Parametric surface you must provide three functions that take two arguments");
	case _PARAMETRIC_SPHERICAL:
		throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create a Parametric Spherical surface you must provide three functions that take two arguments");
	case _RADIAL_ICOSPHERE:
		throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create a Radial Icosphere surface you must provide a single function that take two arguments and a depth value");
	case _EXPLICIT:
		throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create an Explicit surface you must provide a single function that take two arguments");
	case _RADIAL_SPHERICAL:
		throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create a Radial Spherical surface you must provide a single function that take two arguments");
	default:
		throw std::exception("The surface type specified is not supported");
	}
	addOtherBinds(gfx, sc);
}

//	Shape generators

template<typename C>
void Surface::generateExplicit(Graphics& gfx, float F(float, float, const C&), const C& param, Vector2f minRect, Vector2f maxRect, UINT numX, UINT numY, SURFACE_COLORING sc)
{
	float epsilon = error_epsilon;
	if (!sc.Textured) {
		std::vector<Vertex> vertexs;

		for (UINT i = 0; i < numX + 1; i++) {
			for (UINT j = 0; j < numY + 1; j++) {
				float x = ((numX - i) * minRect.x + i * maxRect.x) / numX;
				float y = ((numY - j) * minRect.y + j * maxRect.y) / numY;
				vertexs.push_back({ Vector3f(x, y, F(x,y, param)) ,
					-((Vector3f(x + epsilon, y, F(x + epsilon,y, param)) - Vector3f(x - epsilon, y, F(x - epsilon,y, param))) *
					(Vector3f(x, y + epsilon, F(x,y + epsilon, param)) - Vector3f(x, y - epsilon, F(x,y - epsilon, param)))).normalize(),
					sc.color });
			}
		}
		AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));
	}

	else {
		std::vector<TexVertex> vertexs;

		for (UINT i = 0; i < numX + 1; i++) {
			for (UINT j = 0; j < numY + 1; j++) {
				float x = ((numX - i) * minRect.x + i * maxRect.x) / numX;
				float y = ((numY - j) * minRect.y + j * maxRect.y) / numY;
				vertexs.push_back({ Vector3f(x, y, F(x,y, param)) ,
					-((Vector3f(x + epsilon, y, F(x + epsilon,y, param)) - Vector3f(x - epsilon, y, F(x - epsilon,y, param))) *
					(Vector3f(x, y + epsilon, F(x,y + epsilon, param)) - Vector3f(x, y - epsilon, F(x,y - epsilon, param)))).normalize() ,
					Vector2f((float)i / numX,(float)j / numY) });
			}
		}
		AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));
	}

	std::vector<unsigned short> indexs;

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

	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));
}

template<typename C>
void Surface::generatePolarNormal(Graphics& gfx, float r(float, float, const C&), const C& param, Vector2f minRect, Vector2f maxRect, UINT numX, UINT numY, SURFACE_COLORING sc)
{
	float epsilon = error_epsilon;

	if (!sc.Textured) {
		std::vector<Vertex> vertexs;

		for (UINT i = 0; i < numX + 1; i++) {
			for (UINT j = 0; j < numY + 1; j++) {
				float theta = ((numX - i) * minRect.x + i * maxRect.x) / numX;
				float phi = ((numY - j) * minRect.y + j * maxRect.y) / numY;
				vertexs.push_back({ makePolar({theta,phi, r(theta,phi,param)}) ,

					-((makePolar({theta + epsilon,phi, r(theta + epsilon,phi,param)}) - makePolar({theta - epsilon,phi, r(theta - epsilon,phi,param)})) *
					(makePolar({theta,phi + epsilon, r(theta,phi + epsilon,param)}) - makePolar({theta,phi - epsilon, r(theta,phi - epsilon,param)})))
					.normalize(),
					sc.color });
				if (!j || j == numY)
					vertexs[vertexs.size() - 1].norm = Vector3f(0.f, 0.f, 2.f * phi / pi);
			}
		}
		AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));
	}
	else {
		std::vector<TexVertex> vertexs;

		for (UINT i = 0; i < numX + 1; i++) {
			for (UINT j = 0; j < numY + 1; j++) {
				float theta = ((numX - i) * minRect.x + i * maxRect.x) / numX;
				float phi = ((numY - j) * minRect.y + j * maxRect.y) / numY;
				vertexs.push_back({ makePolar({theta,phi, r(theta,phi,param)}) ,

					-((makePolar({theta + epsilon,phi, r(theta + epsilon,phi,param)}) - makePolar({theta - epsilon,phi, r(theta - epsilon,phi,param)})) *
					(makePolar({theta,phi + epsilon, r(theta,phi + epsilon,param)}) - makePolar({theta,phi - epsilon, r(theta,phi - epsilon,param)})))
					.normalize() ,

					Vector2f((float)i / numX,(float)j / numY) });
				if (!j || j == numY)
					vertexs[vertexs.size() - 1].norm = Vector3f(0.f, 0.f, 2.f * phi / pi);
			}
		}
		AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));
	}


	std::vector<unsigned short> indexs;

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

	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));
}

template<typename C>
void Surface::generatePolarIcosphere(Graphics& gfx, float r(float, float, const C&), const C& param, UINT depth, SURFACE_COLORING sc)
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
	std::vector<Vector2f> texCoord;
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
		if (!v.vector.x && !v.vector.y)
			v.norm = v.vector;
		else
			v.norm =
			-((makePolar({ theta + epsilon, phi, r(theta + epsilon, phi, param )}) - makePolar({ theta - epsilon, phi, r(theta - epsilon, phi, param) })) *
				(makePolar({ theta, phi + epsilon, r(theta, phi + epsilon, param) }) - makePolar({ theta, phi - epsilon, r(theta, phi - epsilon, param) })))
			.normalize();

		v.vector *= r(theta, phi, param);
		texCoord.push_back({ theta / 2.f / pi,(pi / 2.f - phi) / pi });
	}

	if (sc.Textured) {
		std::vector<TexVertex> texvertexs;
		for (UINT i = 0; i < vertexs.size(); i++)
			texvertexs.push_back({ vertexs[i].vector,vertexs[i].norm,texCoord[i] });
		AddBind(std::make_unique<VertexBuffer>(gfx, texvertexs));
	}
	else
		AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));

	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));
}

template<typename C>
void Surface::generateParametric(Graphics& gfx, float x(float, float, const C&), float y(float, float, const C&), float z(float, float, const C&), const C& param, Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV, SURFACE_COLORING sc)
{
	float epsilon = error_epsilon;

	if (!sc.Textured) {
		std::vector<Vertex> vertexs;

		for (UINT i = 0; i < numU + 1; i++) {
			for (UINT j = 0; j < numV + 1; j++) {
				float u = ((numU - i) * minRect.x + i * maxRect.x) / numU;
				float v = ((numV - j) * minRect.y + j * maxRect.y) / numV;
				vertexs.push_back({ Vector3f(x(u,v,param), y(u,v,param), z(u,v,param)) ,
					-((Vector3f(x(u + epsilon,v,param), y(u + epsilon,v,param), z(u + epsilon,v,param)) - Vector3f(x(u - epsilon,v,param), y(u - epsilon,v,param), z(u - epsilon,v,param))) *
					(Vector3f(x(u,v + epsilon,param), y(u,v + epsilon,param), z(u,v + epsilon,param)) - Vector3f(x(u,v - epsilon,param), y(u,v - epsilon,param), z(u,v - epsilon,param))))
					.normalize(),
					sc.color });
			}
		}
		AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));
	}
	else {
		std::vector<TexVertex> vertexs;

		for (UINT i = 0; i < numU + 1; i++) {
			for (UINT j = 0; j < numV + 1; j++) {
				float u = ((numU - i) * minRect.x + i * maxRect.x) / numU;
				float v = ((numV - j) * minRect.y + j * maxRect.y) / numV;
				vertexs.push_back({ Vector3f(x(u,v,param), y(u,v,param), z(u,v,param)) ,
					-((Vector3f(x(u + epsilon,v,param), y(u + epsilon,v,param), z(u + epsilon,v,param)) - Vector3f(x(u - epsilon,v,param), y(u - epsilon,v,param), z(u - epsilon,v,param))) *
					(Vector3f(x(u,v + epsilon,param), y(u,v + epsilon,param), z(u,v + epsilon,param)) - Vector3f(x(u,v - epsilon,param), y(u,v - epsilon,param), z(u,v - epsilon,param))))
					.normalize(),

					Vector2f((float)i / numU,(float)j / numV) });
			}
		}
		AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));
	}

	std::vector<unsigned short> indexs;

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

	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));
}

template<typename C>
void Surface::generateParametric(Graphics& gfx, Vector3f P(float, float, const C&), const C& param, Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV, SURFACE_COLORING sc)
{
	float epsilon = error_epsilon;

	if (!sc.Textured) {
		std::vector<Vertex> vertexs;

		for (UINT i = 0; i < numU + 1; i++) {
			for (UINT j = 0; j < numV + 1; j++) {
				float u = ((numU - i) * minRect.x + i * maxRect.x) / numU;
				float v = ((numV - j) * minRect.y + j * maxRect.y) / numV;
				vertexs.push_back({ P(u,v, param) ,
					-((P(u + epsilon,v, param) - P(u - epsilon,v, param)) *
					(P(u,v + epsilon, param) - P(u,v - epsilon, param)))
					.normalize(),
					sc.color });
			}
		}
		AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));
	}
	else {
		std::vector<TexVertex> vertexs;

		for (UINT i = 0; i < numU + 1; i++) {
			for (UINT j = 0; j < numV + 1; j++) {
				float u = ((numU - i) * minRect.x + i * maxRect.x) / numU;
				float v = ((numV - j) * minRect.y + j * maxRect.y) / numV;
				vertexs.push_back({ P(u,v, param) ,
					-((P(u + epsilon,v, param) - P(u - epsilon,v, param)) *
					(P(u,v + epsilon, param) - P(u,v - epsilon, param)))
					.normalize(),

					Vector2f((float)i / numU,(float)j / numV) });
			}
		}
		AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));
	}

	std::vector<unsigned short> indexs;

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

	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));
}

template<typename C>
void Surface::generatePolarParametric(Graphics& gfx, float theta(float, float, const C&), float phi(float, float, const C&), float rad(float, float, const C&), const C& param, Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV, SURFACE_COLORING sc)
{
	float epsilon = error_epsilon;

	if (!sc.Textured) {
		std::vector<Vertex> vertexs;

		for (UINT i = 0; i < numU + 1; i++) {
			for (UINT j = 0; j < numV + 1; j++) {
				float u = ((numU - i) * minRect.x + i * maxRect.x) / numU;
				float v = ((numV - j) * minRect.y + j * maxRect.y) / numV;
				vertexs.push_back({ makePolar({theta(u,v,param),phi(u,v,param),rad(u,v,param)}) ,

					-((makePolar({theta(u+epsilon,v,param),phi(u + epsilon,v,param),rad(u + epsilon,v,param)}) -
					makePolar({theta(u - epsilon,v,param),phi(u - epsilon,v,param),rad(u - epsilon,v,param)})) *
					((makePolar({theta(u,v + epsilon,param),phi(u,v + epsilon,param),rad(u,v + epsilon,param)}) -
					makePolar({theta(u,v - epsilon,param),phi(u,v - epsilon,param),rad(u,v - epsilon,param)}))))
					.normalize(),
					sc.color });
			}
		}
		AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));
	}
	else {
		std::vector<TexVertex> vertexs;

		for (UINT i = 0; i < numU + 1; i++) {
			for (UINT j = 0; j < numV + 1; j++) {
				float u = ((numU - i) * minRect.x + i * maxRect.x) / numU;
				float v = ((numV - j) * minRect.y + j * maxRect.y) / numV;
				vertexs.push_back({ makePolar({theta(u,v,param),phi(u,v,param),rad(u,v,param)}) ,

					-((makePolar({theta(u + epsilon,v,param),phi(u + epsilon,v,param),rad(u + epsilon,v,param)}) -
					makePolar({theta(u - epsilon,v,param),phi(u - epsilon,v,param),rad(u - epsilon,v,param)})) *
					((makePolar({theta(u,v + epsilon,param),phi(u,v + epsilon,param),rad(u,v + epsilon,param)}) -
					makePolar({theta(u,v - epsilon,param),phi(u,v - epsilon,param),rad(u,v - epsilon,param)}))))
					.normalize() ,

					Vector2f((float)i / numU,(float)j / numV) });
			}
		}
		AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));
	}


	std::vector<unsigned short> indexs;

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

	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));
}

template<typename C>
void Surface::generatePolarParametric(Graphics& gfx, Vector3f P(float, float, const C&), const C& param, Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV, SURFACE_COLORING sc)
{
	float epsilon = error_epsilon;

	if (!sc.Textured) {
		std::vector<Vertex> vertexs;

		for (UINT i = 0; i < numU + 1; i++) {
			for (UINT j = 0; j < numV + 1; j++) {
				float u = ((numU - i) * minRect.x + i * maxRect.x) / numU;
				float v = ((numV - j) * minRect.y + j * maxRect.y) / numV;
				vertexs.push_back({ makePolar(P(u,v,param)) ,

					-((makePolar(P(u + epsilon,v,param)) - makePolar(P(u - epsilon,v,param))) *
					(makePolar(P(u,v + epsilon,param)) - makePolar(P(u,v - epsilon,param))))
					.normalize(),
					sc.color });
			}
		}
		AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));
	}
	else {
		std::vector<TexVertex> vertexs;

		for (UINT i = 0; i < numU + 1; i++) {
			for (UINT j = 0; j < numV + 1; j++) {
				float u = ((numU - i) * minRect.x + i * maxRect.x) / numU;
				float v = ((numV - j) * minRect.y + j * maxRect.y) / numV;
				vertexs.push_back({ makePolar(P(u,v,param)) ,

					-((makePolar(P(u + epsilon,v,param)) - makePolar(P(u - epsilon,v,param))) *
					(makePolar(P(u,v + epsilon,param)) - makePolar(P(u,v - epsilon,param))))
					.normalize(),

					Vector2f((float)i / numU,(float)j / numV) });
			}
		}
		AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));
	}


	std::vector<unsigned short> indexs;

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

	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));
}

template<typename C>
void Surface::generateImplicit(Graphics& gfx, float H(float, float, float, const C&), const C& param, SURFACE_COLORING sc, Vector3f regionBegin, Vector3f regionEnd)
{
	constexpr int cubes = 60;
	float minx = regionBegin.x;
	float maxx = regionEnd.x;
	float miny = regionBegin.y;
	float maxy = regionEnd.y;
	float minz = regionBegin.z;
	float maxz = regionEnd.z;

	_float4vector*** h = (_float4vector***)calloc(cubes + 1, sizeof(void**));

	for (int i = 0; i <= cubes; i++)
	{
		h[i] = (_float4vector**)calloc(cubes + 1, sizeof(void*));

		for (int j = 0; j <= cubes; j++)
		{
			h[i][j] = (_float4vector*)calloc(cubes + 1, sizeof(_float4vector));

			for (int k = 0; k <= cubes; k++)
			{
				float di = float(i) / cubes;
				float dj = float(j) / cubes;
				float dk = float(k) / cubes;

				float x = minx * (1 - di) + maxx * di;
				float y = miny * (1 - dj) + maxy * dj;
				float z = minz * (1 - dk) + maxz * dk;
				h[i][j][k] = { x , y , z , H(x, y, z, param) };
			}
		}
	}

	std::vector<Vertex> vertexs;
	std::vector<unsigned short> indexs;

	for (int i = 0; i < cubes; i++)
	{
		for (int j = 0; j < cubes; j++)
		{
			for (int k = 0; k < cubes; k++)
			{
				_float4vector cube[8] = { h[i][j][k] , h[i][j][k + 1] , h[i][j + 1][k + 1] , h[i][j + 1][k] , h[i + 1][j][k] , h[i + 1][j][k + 1] , h[i + 1][j + 1][k + 1] , h[i + 1][j + 1][k] };

				addVertexsCube(cube, vertexs, indexs, sc);
			}
		}
	}

	for (int i = 0; i <= cubes; i++)
	{
		for (int j = 0; j <= cubes; j++)
		{
			free(h[i][j]);
		}
		free(h[i]);
	}
	free(h);

	AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));
	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));
}

template<typename C>
void Surface::generateImplicitPolar(Graphics& gfx, float H(float, float, float, const C&), const C& param, SURFACE_COLORING sc, Vector3f regionBegin, Vector3f regionEnd)
{
	constexpr int cubes = 60;
	float minx = regionBegin.x;
	float maxx = regionEnd.x;
	float miny = regionBegin.y;
	float maxy = regionEnd.y;
	float minz = regionBegin.z;
	float maxz = regionEnd.z;

	_float4vector*** h = (_float4vector***)calloc(cubes + 1, sizeof(void**));

	for (int i = 0; i <= cubes; i++)
	{
		h[i] = (_float4vector**)calloc(cubes + 1, sizeof(void*));

		for (int j = 0; j <= cubes; j++)
		{
			h[i][j] = (_float4vector*)calloc(cubes + 1, sizeof(_float4vector));

			for (int k = 0; k <= cubes; k++)
			{
				float di = float(i) / cubes;
				float dj = float(j) / cubes;
				float dk = float(k) / cubes;

				float x = minx * (1 - di) + maxx * di;
				float y = miny * (1 - dj) + maxy * dj;
				float z = minz * (1 - dk) + maxz * dk;
				h[i][j][k] = { x , y , z , H(x, y, z, param) };
			}
		}
	}

	std::vector<Vertex> vertexs;
	std::vector<unsigned short> indexs;

	for (int i = 0; i < cubes; i++)
	{
		for (int j = 0; j < cubes; j++)
		{
			for (int k = 0; k < cubes; k++)
			{
				_float4vector cube[8] = { h[i][j][k] , h[i][j][k + 1] , h[i][j + 1][k + 1] , h[i][j + 1][k] , h[i + 1][j][k] , h[i + 1][j][k + 1] , h[i + 1][j + 1][k + 1] , h[i + 1][j + 1][k] };

				addVertexsCube(cube, vertexs, indexs, sc, true);
			}
		}
	}

	for (int i = 0; i <= cubes; i++)
	{
		for (int j = 0; j <= cubes; j++)
		{
			free(h[i][j]);
		}
		free(h[i]);
	}
	free(h);

	AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));
	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));
}
