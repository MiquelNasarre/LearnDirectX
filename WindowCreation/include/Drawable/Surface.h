#pragma once
#include "Drawable.h"

#define error_epsilon	0.001f

enum SURFACE_TYPE {
	_EXPLICIT,
	_EXPLICIT_SPHERICAL,
	_EXPLICIT_ICOSPHERE,
	_PARAMETRIC,
	_PARAMETRIC_SPHERICAL,
	_IMPLICIT,
	_IMPLICIT_SPHERICAL
};

struct SURFACE_SHAPE
{
	SURFACE_TYPE Type;

	//	Explicit

	float (*Explicit)(float, float) = NULL;

	//	Parametric

	float (*Parametric_0)(float, float) = NULL;
	float (*Parametric_1)(float, float) = NULL;
	float (*Parametric_2)(float, float) = NULL;

	Vector3f(*Parametric_V)(float, float) = NULL;

	//	Implicit

	float (*Implicit)(float, float, float) = NULL;

	//	Coordinates

	Vector2f minRect, maxRect;
	UINT numU, numV;

	UINT ICOSPHERE_DEPHT;

	Vector3f minCube, maxCube;

	//	Constructors

	SURFACE_SHAPE(SURFACE_TYPE Type, float explicit_(float, float), bool defaultValues = true, Vector2f minRect = {}, Vector2f maxRect = {}, UINT numX = 100u, UINT numY = 100u);
	SURFACE_SHAPE(SURFACE_TYPE Type, float radius(float, float), UINT ICOSPHERE_DEPTH);
	SURFACE_SHAPE(SURFACE_TYPE Type, float parametric_0(float, float), float parametric_1(float, float), float parametric_2(float, float), bool defaultValues = true, Vector2f minRect = {}, Vector2f maxRect = {}, UINT numU = 100u, UINT numV = 100u);
	SURFACE_SHAPE(SURFACE_TYPE Type, Vector3f parametric_v(float, float), bool defaultValues = true, Vector2f minRect = {}, Vector2f maxRect = {}, UINT numU = 100u, UINT numV = 100u);
	SURFACE_SHAPE(SURFACE_TYPE Type, float implicit(float, float, float), Vector3f minCube = { -2.f, -2.f, -2.f }, Vector3f maxCube = { 2.f, 2.f, 2.f });

	SURFACE_SHAPE(const SURFACE_SHAPE& other);
};

template<typename C>
struct PARAM_SURFACE_SHAPE
{
	C param;

	SURFACE_TYPE Type;

	//	Explicit

	float (*Explicit)(float, float, const C&) = NULL;

	//	Parametric

	float (*Parametric_0)(float, float, const C&) = NULL;
	float (*Parametric_1)(float, float, const C&) = NULL;
	float (*Parametric_2)(float, float, const C&) = NULL;

	Vector3f(*Parametric_V)(float, float, const C&) = NULL;

	//	Implicit

	float (*Implicit)(float, float, float, const C&) = NULL;

	//	Coordinates

	Vector2f minRect, maxRect;
	UINT numU, numV;

	UINT ICOSPHERE_DEPHT;

	Vector3f minCube, maxCube;

	PARAM_SURFACE_SHAPE(SURFACE_TYPE Type, float explicit_(float, float, const C&), const C& param, bool defaultValues = true, Vector2f min = {}, Vector2f max = {}, UINT numX = 100u, UINT numY = 100u)
		:Type{ Type }, param{ param }, Explicit{ explicit_ }, minRect{ min }, maxRect{ max }, numU{ numX }, numV{ numY }
	{
		switch (Type)
		{
		case _EXPLICIT:
			if (defaultValues) {
				minRect = { -1.f,-1.f };
				maxRect = { 1.f, 1.f };
			}
			Explicit = explicit_;
			break;
		case _EXPLICIT_SPHERICAL:
			if (defaultValues) {
				minRect = { 0.f, pi / 2.f };
				maxRect = { 2.f * pi, -pi / 2.f };
			}
			break;
		case _IMPLICIT:
			throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create an Implicit surface you must provide just one function that takes three arguments");
		case _PARAMETRIC:
			throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create a Parametric surface you must provide three functions that take two arguments");
		case _EXPLICIT_ICOSPHERE:
			throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create a Radial Icosphere surface you must specify the depth of the icosphere\n(depth 5 recomended)");
		case _PARAMETRIC_SPHERICAL:
			throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create a Parametric Spherical surface you must provide three functions that take two arguments");
		case _IMPLICIT_SPHERICAL:
			throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create an Implicit Spherical surface you must provide just one function that takes three arguments");
		default:
			throw std::exception("The surface type specified is not supported");
		}
	}

	PARAM_SURFACE_SHAPE(SURFACE_TYPE Type, float radius(float, float, const C&), const C& param, UINT depth)
		:Type{ Type }, param{ param }, Explicit{ radius }, ICOSPHERE_DEPHT{ depth }
	{
		switch (Type)
		{
		case _EXPLICIT_ICOSPHERE:
			break;
		case _EXPLICIT:
			throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create an Explicit surface you must remove the last value that provides a depth for the icosphere");
		case _EXPLICIT_SPHERICAL:
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
	}

	PARAM_SURFACE_SHAPE(SURFACE_TYPE Type, float parametric_0(float, float, const C&), float parametric_1(float, float, const C&), float parametric_2(float, float, const C&), const C& param, bool defaultValues = true, Vector2f min = {}, Vector2f max = {}, UINT U = 100u, UINT V = 100u)
		:Type{ Type }, param{ param }, Parametric_0{ parametric_0 }, Parametric_1{ parametric_1 }, Parametric_2{ parametric_2 }, minRect{ min }, maxRect{ max }, numU{ U }, numV{ V }
	{
		switch (Type)
		{
		case _PARAMETRIC:
			if (defaultValues) {
				minRect = { -1.f,-1.f };
				maxRect = { 1.f, 1.f };
			}
			break;
		case _PARAMETRIC_SPHERICAL:
			if (defaultValues) {
				minRect = { 0.f, pi / 2.f };
				maxRect = { 2.f * pi, -pi / 2.f };
			}
			break;
		case _EXPLICIT_ICOSPHERE:
			throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create a Radial Icosphere surface you must provide a single function that take two arguments and a depth value");
		case _EXPLICIT:
			throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create an Explicit surface you must provide a single function that take two arguments");
		case _EXPLICIT_SPHERICAL:
			throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create a Radial Spherical surface you must provide a single function that take two arguments");
		case _IMPLICIT:
			throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create an Implicit surface you must provide just one function that takes three arguments");
		case _IMPLICIT_SPHERICAL:
			throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create an Implicit Spherical surface you must provide just one function that takes three arguments");
		default:
			throw std::exception("The surface type specified is not supported");
		}
	}

	PARAM_SURFACE_SHAPE(SURFACE_TYPE Type, Vector3f parametric_v(float, float, const C&), const C& param, bool defaultValues = true, Vector2f min = {}, Vector2f max = {}, UINT U = 100u, UINT V = 100u)
		:Type{ Type }, param{ param }, Parametric_V{ parametric_v }, minRect{ min }, maxRect{ max }, numU{ U }, numV{ V }
	{
		switch (Type)
		{
		case _PARAMETRIC:
			if (defaultValues) {
				minRect = { -1.f,-1.f };
				maxRect = { 1.f, 1.f };
			}
			break;
		case _PARAMETRIC_SPHERICAL:
			if (defaultValues) {
				minRect = { 0.f, pi / 2.f };
				maxRect = { 2.f * pi, -pi / 2.f };
			}
			break;
		case _EXPLICIT_ICOSPHERE:
			throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create a Radial Icosphere surface you must provide a single function that take two arguments and a depth value");
		case _EXPLICIT:
			throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create an Explicit surface you must provide a single function that take two arguments");
		case _EXPLICIT_SPHERICAL:
			throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create a Radial Spherical surface you must provide a single function that take two arguments");
		case _IMPLICIT:
			throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create an Implicit surface you must provide just one function that takes three arguments");
		case _IMPLICIT_SPHERICAL:
			throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create an Implicit Spherical surface you must provide just one function that takes three arguments");
		default:
			throw std::exception("The surface type specified is not supported");
		}
	}

	PARAM_SURFACE_SHAPE(SURFACE_TYPE Type, float implicit(float, float, float, const C&), const C& param, Vector3f min = { -2.f, -2.f, -2.f }, Vector3f max = { 2.f, 2.f, 2.f })
		:Type{ Type }, param{ param }, Implicit{ implicit }, minCube{ min }, maxCube{ max }
	{
		switch (Type)
		{
		case _IMPLICIT:
		case _IMPLICIT_SPHERICAL:
			break;
		case _PARAMETRIC:
			throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create a Parametric surface you must provide three functions that take two arguments");
		case _PARAMETRIC_SPHERICAL:
			throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create a Parametric Spherical surface you must provide three functions that take two arguments");
		case _EXPLICIT_ICOSPHERE:
			throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create a Radial Icosphere surface you must provide a single function that take two arguments and a depth value");
		case _EXPLICIT:
			throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create an Explicit surface you must provide a single function that take two arguments");
		case _EXPLICIT_SPHERICAL:
			throw std::exception("The surface type specified is iconrrect for the given constructor arguments\nTo create a Radial Spherical surface you must provide a single function that take two arguments");
		default:
			throw std::exception("The surface type specified is not supported");
		}
	}

	PARAM_SURFACE_SHAPE(const PARAM_SURFACE_SHAPE& other) :
		param			{ other.param },
		Type			{ other.Type },
		Explicit		{ other.Explicit },
		Parametric_0	{ other.Parametric_0 },
		Parametric_1	{ other.Parametric_1 },
		Parametric_2	{ other.Parametric_2 },
		Parametric_V	{ other.Parametric_V },
		Implicit		{ other.Implicit },
		minRect			{ other.minRect },
		maxRect			{ other.maxRect },
		ICOSPHERE_DEPHT	{ other.ICOSPHERE_DEPHT },
		numU			{ other.numU },
		numV			{ other.numV },
		minCube			{ other.minCube },
		maxCube			{ other.maxCube }
	{
	}

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
		_float4vector intensity = { 0.f,0.f,0.f,0.f };
		_float4color  color = { 1.f,1.f,1.f,1.f };
		_float4vector position = { 0.f,0.f,0.f,0.f };
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

	SURFACE_COLORING(const SURFACE_COLORING& other) :
		Textured		{ other.Textured },
		texture			{ other.texture },
		textureNight	{ other.textureNight },
		color			{ other.color },
		Lighted			{ other.Lighted },
		DefaultInitialLights	{ other.DefaultInitialLights }
	{
		if (!Lighted || DefaultInitialLights)
			return;

		lightsource[0] = other.lightsource[0];
		lightsource[1] = other.lightsource[1];
		lightsource[2] = other.lightsource[2];
		lightsource[3] = other.lightsource[3];
		lightsource[4] = other.lightsource[4];
		lightsource[5] = other.lightsource[5];
		lightsource[6] = other.lightsource[6];
		lightsource[7] = other.lightsource[7];
	}

	SURFACE_COLORING* ptr()
	{
		return this;
	}
};

class Surface : public Drawable
{
public:
	//	Two to rule them all!!

	Surface(Graphics& gfx, SURFACE_SHAPE ss, SURFACE_COLORING* psc = NULL);

	template<typename C>
	Surface(Graphics& gfx, PARAM_SURFACE_SHAPE<C> ss, SURFACE_COLORING* psc = NULL);

	//	Public functions

	void updateRotation(Graphics& gfx, float rotationX, float rotationY, float rotationZ);
	void updateRotation(Graphics& gfx, Vector3f axis, float angle, bool multiplicative = false);
	void updatePosition(Graphics& gfx, Vector3f position, bool additive = false);
	void updateTexture(Graphics& gfx, UINT id, std::string texture);
	void updateTexture(Graphics& gfx, UINT id, Texture texture);
	void updateTextures(Graphics& gfx, Texture texture0, Texture texture1);
	void updateTextures(Graphics& gfx, std::string texture0, std::string texture1);
	void updateLight(Graphics& gfx, UINT id, Vector2f intensity, Color color, Vector3f position);
	void updateLight(Graphics& gfx, UINT id, _float4vector intensity, _float4color color, _float4vector position);
	void clearLights(Graphics& gfx);
	void updateShape(Graphics& gfx, SURFACE_SHAPE ss);

	template<typename C>
	void updateShape(Graphics& gfx, PARAM_SURFACE_SHAPE<C> ss);

	Quaternion getRotation();
	Vector3f getPosition();

private:

	void addOtherBinds(Graphics& gfx);

	struct VSconstBuffer {
		_float4vector translation = { 0.f, 0.f, 0.f, 0.f };
		Quaternion rotation = 1.f;
	}vscBuff;

	struct PSconstBuffer {
		SURFACE_COLORING::LIGHTSOURCE lightsource[8];
	}pscBuff;

	ConstantBuffer<VSconstBuffer>* pVSCB = NULL;
	ConstantBuffer<PSconstBuffer>* pPSCB = NULL;
	SURFACE_COLORING sc;

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

	struct VertexArr
	{
		Vertex* data = NULL;
		UINT counter = 0;
		//UINT maxcount = 0;

		VertexArr(UINT max)
		{
			data = (Vertex*)calloc(max, sizeof(Vertex));
			//if (!data)
			//	throw std::exception(("not able to allocate space for " + std::to_string(max) + " vertexs").c_str());

			//maxcount = max;
		}

		void push_back(Vector3f v, Vector3f norm, Color col)
		{
			//if (counter == maxcount)
			//	throw std::exception("you reached the limit of vector space");

			data[counter].vector = v;
			data[counter].norm = norm;
			data[counter].color = col;
			++counter;
		}

		~VertexArr()
		{
			free(data);
		}
	};

	struct TexVertexArr
	{
		TexVertex* data = NULL;
		UINT counter = 0;
		//UINT maxcount = 0;

		TexVertexArr(UINT max)
		{
			data = (TexVertex*)calloc(max, sizeof(TexVertex));
			//if (!data)
			//	throw std::exception(("not able to allocate space for " + std::to_string(max) + " vertexs").c_str());

			//maxcount = max;
		}

		void push_back(Vector3f v, Vector3f norm, Vector2f tex)
		{
			//if (counter == maxcount)
			//	throw std::exception("you reached the limit of vector space");

			data[counter].vector = v;
			data[counter].norm = norm;
			data[counter].texCoor = tex;
			++counter;
		}

		~TexVertexArr()
		{
			free(data);
		}
	};

	struct IndexArr
	{
		unsigned short* data = NULL;
		UINT counter = 0;
		//UINT maxcount = 0;

		IndexArr(UINT max)
		{
			data = (unsigned short*)calloc(max, sizeof(unsigned short));
			//if (!data)
			//	throw std::exception(("not able to allocate space for " + std::to_string(max) + " indexs").c_str());

			//maxcount = max;
		}

		void push_back(unsigned short i)
		{
			//if (counter == maxcount)
			//	throw std::exception("you reached the limit of vector space");

			data[counter] = i;
			++counter;
		}

		~IndexArr()
		{
			free(data);
		}
	};

	Vector3f evalPolar(float r(float, float), float theta, float phi);
	Vector3f makePolar(Vector3f other);
	void addVertexsCube(_float4vector cube[8], std::vector<Vertex>& vertexs, std::vector<unsigned short>& indexs, bool polar = false);

};

//	Templated stuff

template<typename C>
Surface::Surface(Graphics& gfx, PARAM_SURFACE_SHAPE<C> ss, SURFACE_COLORING* psc)
{
	if (psc != NULL)
		sc = *psc;
	else
		sc = {};

	switch (ss.Type)
	{
	case _EXPLICIT:

		if (ss.Explicit)
		{
			float epsilon = error_epsilon;
			auto numX = ss.numU;
			auto numY = ss.numV;
			auto minRect = ss.minRect;
			auto maxRect = ss.maxRect;
			auto F = ss.Explicit;

			if (!sc.Textured) {
				std::vector<Vertex> vertexs;

				for (UINT i = 0; i < numX + 1; i++) {
					for (UINT j = 0; j < numY + 1; j++) {
						float x = ((numX - i) * minRect.x + i * maxRect.x) / numX;
						float y = ((numY - j) * minRect.y + j * maxRect.y) / numY;
						vertexs.push_back({ Vector3f(x, y, F(x,y, ss.param)) ,
							-((Vector3f(x + epsilon, y, F(x + epsilon,y, ss.param)) - Vector3f(x - epsilon, y, F(x - epsilon,y, ss.param))) *
							(Vector3f(x, y + epsilon, F(x,y + epsilon, ss.param)) - Vector3f(x, y - epsilon, F(x,y - epsilon, ss.param)))).normalize(),
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
						vertexs.push_back({ Vector3f(x, y, F(x,y, ss.param)) ,
							-((Vector3f(x + epsilon, y, F(x + epsilon,y, ss.param)) - Vector3f(x - epsilon, y, F(x - epsilon,y, ss.param))) *
							(Vector3f(x, y + epsilon, F(x,y + epsilon, ss.param)) - Vector3f(x, y - epsilon, F(x,y - epsilon, ss.param)))).normalize() ,
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

		else throw std::exception("Found nullptr while trying to read Explicit function");
		break;
	case _EXPLICIT_SPHERICAL:

		if (ss.Explicit)
		{
			float epsilon = error_epsilon;
			auto numX = ss.numU;
			auto numY = ss.numV;
			auto minRect = ss.minRect;
			auto maxRect = ss.maxRect;
			auto r = ss.Explicit;

			if (!sc.Textured) {
				std::vector<Vertex> vertexs;

				for (UINT i = 0; i < numX + 1; i++) {
					for (UINT j = 0; j < numY + 1; j++) {
						float theta = ((numX - i) * minRect.x + i * maxRect.x) / numX;
						float phi = ((numY - j) * minRect.y + j * maxRect.y) / numY;
						vertexs.push_back({ makePolar({theta,phi, r(theta,phi,ss.param)}) ,

							-((makePolar({theta + epsilon,phi, r(theta + epsilon,phi,ss.param)}) - makePolar({theta - epsilon,phi, r(theta - epsilon,phi,ss.param)})) *
							(makePolar({theta,phi + epsilon, r(theta,phi + epsilon,ss.param)}) - makePolar({theta,phi - epsilon, r(theta,phi - epsilon,ss.param)})))
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
						vertexs.push_back({ makePolar({theta,phi, r(theta,phi,ss.param)}) ,

							-((makePolar({theta + epsilon,phi, r(theta + epsilon,phi,ss.param)}) - makePolar({theta - epsilon,phi, r(theta - epsilon,phi,ss.param)})) *
							(makePolar({theta,phi + epsilon, r(theta,phi + epsilon,ss.param)}) - makePolar({theta,phi - epsilon, r(theta,phi - epsilon,ss.param)})))
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

		else throw std::exception("Found nullptr while trying to read Explicit function");
		break;
	case _EXPLICIT_ICOSPHERE:

		if (ss.Explicit)
		{
			float epsilon = error_epsilon;
			auto depth = ss.ICOSPHERE_DEPHT;
			auto r = ss.Explicit;

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
					-((makePolar({ theta + epsilon, phi, r(theta + epsilon, phi, ss.param) }) - makePolar({ theta - epsilon, phi, r(theta - epsilon, phi, ss.param) })) *
						(makePolar({ theta, phi + epsilon, r(theta, phi + epsilon, ss.param) }) - makePolar({ theta, phi - epsilon, r(theta, phi - epsilon, ss.param) })))
					.normalize();

				v.vector *= r(theta, phi, ss.param);
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

		else throw std::exception("Found nullptr while trying to read Explicit function");
		break;
	case _IMPLICIT:

		if (ss.Implicit)
		{
			auto H = ss.Implicit;
			auto regionBegin = ss.minCube;
			auto regionEnd = ss.maxCube;

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
						h[i][j][k] = { x , y , z , H(x, y, z, ss.param) };
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

						addVertexsCube(cube, vertexs, indexs);
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

		else throw std::exception("Found nullptr while trying to read Implicit function");
		break;
	case _IMPLICIT_SPHERICAL:

		if (ss.Implicit)
		{
			auto H = ss.Implicit;
			auto regionBegin = ss.minCube;
			auto regionEnd = ss.maxCube;

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
						h[i][j][k] = { x , y , z , H(x, y, z, ss.param) };
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

						addVertexsCube(cube, vertexs, indexs, true);
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

		else throw std::exception("Found nullptr while trying to read Implicit function");
		break;
	case _PARAMETRIC:

		if (ss.Parametric_V)
		{
			float epsilon = error_epsilon;
			auto numU = ss.numU;
			auto numV = ss.numV;
			auto minRect = ss.minRect;
			auto maxRect = ss.maxRect;
			auto P = ss.Parametric_V;

			if (!sc.Textured) {
				std::vector<Vertex> vertexs;

				for (UINT i = 0; i < numU + 1; i++) {
					for (UINT j = 0; j < numV + 1; j++) {
						float u = ((numU - i) * minRect.x + i * maxRect.x) / numU;
						float v = ((numV - j) * minRect.y + j * maxRect.y) / numV;
						vertexs.push_back({ P(u,v, ss.param) ,
							-((P(u + epsilon,v, ss.param) - P(u - epsilon,v, ss.param)) *
							(P(u,v + epsilon, ss.param) - P(u,v - epsilon, ss.param)))
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
						vertexs.push_back({ P(u,v, ss.param) ,
							-((P(u + epsilon,v, ss.param) - P(u - epsilon,v, ss.param)) *
							(P(u,v + epsilon, ss.param) - P(u,v - epsilon, ss.param)))
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

		else if (ss.Parametric_0 && ss.Parametric_1 && ss.Parametric_2)
		{
			float epsilon = error_epsilon;
			auto numU = ss.numU;
			auto numV = ss.numV;
			auto minRect = ss.minRect;
			auto maxRect = ss.maxRect;
			auto x = ss.Parametric_0;
			auto y = ss.Parametric_1;
			auto z = ss.Parametric_2;

			if (!sc.Textured) {
				std::vector<Vertex> vertexs;

				for (UINT i = 0; i < numU + 1; i++) {
					for (UINT j = 0; j < numV + 1; j++) {
						float u = ((numU - i) * minRect.x + i * maxRect.x) / numU;
						float v = ((numV - j) * minRect.y + j * maxRect.y) / numV;
						vertexs.push_back({ Vector3f(x(u,v,ss.param), y(u,v,ss.param), z(u,v,ss.param)) ,
							-((Vector3f(x(u + epsilon,v,ss.param), y(u + epsilon,v,ss.param), z(u + epsilon,v,ss.param)) - Vector3f(x(u - epsilon,v,ss.param), y(u - epsilon,v,ss.param), z(u - epsilon,v,ss.param))) *
							(Vector3f(x(u,v + epsilon,ss.param), y(u,v + epsilon,ss.param), z(u,v + epsilon,ss.param)) - Vector3f(x(u,v - epsilon,ss.param), y(u,v - epsilon,ss.param), z(u,v - epsilon,ss.param))))
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
						vertexs.push_back({ Vector3f(x(u,v,ss.param), y(u,v,ss.param), z(u,v,ss.param)) ,
							-((Vector3f(x(u + epsilon,v,ss.param), y(u + epsilon,v,ss.param), z(u + epsilon,v,ss.param)) - Vector3f(x(u - epsilon,v,ss.param), y(u - epsilon,v,ss.param), z(u - epsilon,v,ss.param))) *
							(Vector3f(x(u,v + epsilon,ss.param), y(u,v + epsilon,ss.param), z(u,v + epsilon,ss.param)) - Vector3f(x(u,v - epsilon,ss.param), y(u,v - epsilon,ss.param), z(u,v - epsilon,ss.param))))
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

		else throw std::exception("Found nullptr while trying to read Parametric function");
		break;
	case _PARAMETRIC_SPHERICAL:

		if (ss.Parametric_V)
		{
			float epsilon = error_epsilon;
			auto numU = ss.numU;
			auto numV = ss.numV;
			auto minRect = ss.minRect;
			auto maxRect = ss.maxRect;
			auto P = ss.Parametric_V;

			if (!sc.Textured) {
				std::vector<Vertex> vertexs;

				for (UINT i = 0; i < numU + 1; i++) {
					for (UINT j = 0; j < numV + 1; j++) {
						float u = ((numU - i) * minRect.x + i * maxRect.x) / numU;
						float v = ((numV - j) * minRect.y + j * maxRect.y) / numV;
						vertexs.push_back({ makePolar(P(u,v,ss.param)) ,

							-((makePolar(P(u + epsilon,v,ss.param)) - makePolar(P(u - epsilon,v,ss.param))) *
							(makePolar(P(u,v + epsilon,ss.param)) - makePolar(P(u,v - epsilon,ss.param))))
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
						vertexs.push_back({ makePolar(P(u,v,ss.param)) ,

							-((makePolar(P(u + epsilon,v,ss.param)) - makePolar(P(u - epsilon,v,ss.param))) *
							(makePolar(P(u,v + epsilon,ss.param)) - makePolar(P(u,v - epsilon,ss.param))))
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

		else if (ss.Parametric_0 && ss.Parametric_1 && ss.Parametric_2)
		{
			float epsilon = error_epsilon;
			auto numU = ss.numU;
			auto numV = ss.numV;
			auto minRect = ss.minRect;
			auto maxRect = ss.maxRect;
			auto theta = ss.Parametric_0;
			auto phi = ss.Parametric_1;
			auto rad = ss.Parametric_2;

			if (!sc.Textured) {
				std::vector<Vertex> vertexs;

				for (UINT i = 0; i < numU + 1; i++) {
					for (UINT j = 0; j < numV + 1; j++) {
						float u = ((numU - i) * minRect.x + i * maxRect.x) / numU;
						float v = ((numV - j) * minRect.y + j * maxRect.y) / numV;
						vertexs.push_back({ makePolar({theta(u,v,ss.param),phi(u,v,ss.param),rad(u,v,ss.param)}) ,

							-((makePolar({theta(u + epsilon,v,ss.param),phi(u + epsilon,v,ss.param),rad(u + epsilon,v,ss.param)}) -
							makePolar({theta(u - epsilon,v,ss.param),phi(u - epsilon,v,ss.param),rad(u - epsilon,v,ss.param)})) *
							((makePolar({theta(u,v + epsilon,ss.param),phi(u,v + epsilon,ss.param),rad(u,v + epsilon,ss.param)}) -
							makePolar({theta(u,v - epsilon,ss.param),phi(u,v - epsilon,ss.param),rad(u,v - epsilon,ss.param)}))))
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
						vertexs.push_back({ makePolar({theta(u,v,ss.param),phi(u,v,ss.param),rad(u,v,ss.param)}) ,

							-((makePolar({theta(u + epsilon,v,ss.param),phi(u + epsilon,v,ss.param),rad(u + epsilon,v,ss.param)}) -
							makePolar({theta(u - epsilon,v,ss.param),phi(u - epsilon,v,ss.param),rad(u - epsilon,v,ss.param)})) *
							((makePolar({theta(u,v + epsilon,ss.param),phi(u,v + epsilon,ss.param),rad(u,v + epsilon,ss.param)}) -
							makePolar({theta(u,v - epsilon,ss.param),phi(u,v - epsilon,ss.param),rad(u,v - epsilon,ss.param)}))))
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

		else throw std::exception("Found nullptr while trying to read Parametric function");
		break;
	default:
		throw std::exception("The surface type specified is not supported");
	}
	addOtherBinds(gfx);
}

template<typename C>
void Surface::updateShape(Graphics& gfx, PARAM_SURFACE_SHAPE<C> ss)
{
	switch (ss.Type)
	{
	case _EXPLICIT:

		if (ss.Explicit)
		{
			float epsilon = error_epsilon;
			auto numX = ss.numU;
			auto numY = ss.numV;
			auto minRect = ss.minRect;
			auto maxRect = ss.maxRect;
			auto F = ss.Explicit;

			if (!sc.Textured) {
				VertexArr vertexs((numX + 1) * (numY + 1));

				for (UINT i = 0; i < numX + 1; i++) {
					for (UINT j = 0; j < numY + 1; j++) {
						float x = ((numX - i) * minRect.x + i * maxRect.x) / numX;
						float y = ((numY - j) * minRect.y + j * maxRect.y) / numY;
						vertexs.push_back( Vector3f(x, y, F(x,y, ss.param)) ,
							-((Vector3f(x + epsilon, y, F(x + epsilon,y, ss.param)) - Vector3f(x - epsilon, y, F(x - epsilon,y, ss.param))) *
							(Vector3f(x, y + epsilon, F(x,y + epsilon, ss.param)) - Vector3f(x, y - epsilon, F(x,y - epsilon, ss.param)))).normalize(),
							sc.color );
					}
				}
				changeBind(std::make_unique<VertexBuffer>(gfx, vertexs.data, vertexs.counter), 0u);
			}

			else {
				TexVertexArr vertexs((numX + 1) * (numY + 1));

				for (UINT i = 0; i < numX + 1; i++) {
					for (UINT j = 0; j < numY + 1; j++) {
						float x = ((numX - i) * minRect.x + i * maxRect.x) / numX;
						float y = ((numY - j) * minRect.y + j * maxRect.y) / numY;
						vertexs.push_back( Vector3f(x, y, F(x,y, ss.param)) ,
							-((Vector3f(x + epsilon, y, F(x + epsilon,y, ss.param)) - Vector3f(x - epsilon, y, F(x - epsilon,y, ss.param))) *
							(Vector3f(x, y + epsilon, F(x,y + epsilon, ss.param)) - Vector3f(x, y - epsilon, F(x,y - epsilon, ss.param)))).normalize() ,
							Vector2f((float)i / numX,(float)j / numY) );
					}
				}
				changeBind(std::make_unique<VertexBuffer>(gfx, vertexs.data, vertexs.counter), 0u);
			}

			IndexArr indexs(12 * numX * numY);

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

			changeBind(std::make_unique<IndexBuffer>(gfx, indexs.data, indexs.counter), 1u);
		}

		else throw std::exception("Found nullptr while trying to read Explicit function");
		break;
	case _EXPLICIT_SPHERICAL:

		if (ss.Explicit)
		{
			float epsilon = error_epsilon;
			auto numX = ss.numU;
			auto numY = ss.numV;
			auto minRect = ss.minRect;
			auto maxRect = ss.maxRect;
			auto r = ss.Explicit;

			if (!sc.Textured) {
				VertexArr vertexs((numX + 1) * (numY + 1));

				for (UINT i = 0; i < numX + 1; i++) {
					for (UINT j = 0; j < numY + 1; j++) {
						float theta = ((numX - i) * minRect.x + i * maxRect.x) / numX;
						float phi = ((numY - j) * minRect.y + j * maxRect.y) / numY;
						vertexs.push_back( makePolar({theta,phi, r(theta,phi,ss.param)}) ,

							-((makePolar({theta + epsilon,phi, r(theta + epsilon,phi,ss.param)}) - makePolar({theta - epsilon,phi, r(theta - epsilon,phi,ss.param)})) *
							(makePolar({theta,phi + epsilon, r(theta,phi + epsilon,ss.param)}) - makePolar({theta,phi - epsilon, r(theta,phi - epsilon,ss.param)})))
							.normalize(),
							sc.color );
						if (!j || j == numY)
							vertexs.data[vertexs.counter - 1].norm = Vector3f(0.f, 0.f, 2.f * phi / pi);
					}
				}
				changeBind(std::make_unique<VertexBuffer>(gfx, vertexs.data, vertexs.counter), 0u);
			}
			else {
				TexVertexArr vertexs((numX + 1) * (numY + 1));

				for (UINT i = 0; i < numX + 1; i++) {
					for (UINT j = 0; j < numY + 1; j++) {
						float theta = ((numX - i) * minRect.x + i * maxRect.x) / numX;
						float phi = ((numY - j) * minRect.y + j * maxRect.y) / numY;
						vertexs.push_back( makePolar({theta,phi, r(theta,phi,ss.param)}) ,

							-((makePolar({theta + epsilon,phi, r(theta + epsilon,phi,ss.param)}) - makePolar({theta - epsilon,phi, r(theta - epsilon,phi,ss.param)})) *
							(makePolar({theta,phi + epsilon, r(theta,phi + epsilon,ss.param)}) - makePolar({theta,phi - epsilon, r(theta,phi - epsilon,ss.param)})))
							.normalize() ,

							Vector2f((float)i / numX,(float)j / numY) );
						if (!j || j == numY)
							vertexs.data[vertexs.counter - 1].norm = Vector3f(0.f, 0.f, 2.f * phi / pi);
					}
				}
				changeBind(std::make_unique<VertexBuffer>(gfx, vertexs.data, vertexs.counter), 0u);
			}


			IndexArr indexs(12 * numX * numY);

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

			changeBind(std::make_unique<IndexBuffer>(gfx, indexs.data, indexs.counter), 1u);
		}

		else throw std::exception("Found nullptr while trying to read Explicit function");
		break;
	case _EXPLICIT_ICOSPHERE:

		if (ss.Explicit)
		{
			float epsilon = error_epsilon;
			auto depth = ss.ICOSPHERE_DEPHT;
			auto r = ss.Explicit;

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
					-((makePolar({ theta + epsilon, phi, r(theta + epsilon, phi, ss.param) }) - makePolar({ theta - epsilon, phi, r(theta - epsilon, phi, ss.param) })) *
						(makePolar({ theta, phi + epsilon, r(theta, phi + epsilon, ss.param) }) - makePolar({ theta, phi - epsilon, r(theta, phi - epsilon, ss.param) })))
					.normalize();

				v.vector *= r(theta, phi, ss.param);
				texCoord.push_back({ theta / 2.f / pi,(pi / 2.f - phi) / pi });
			}

			if (sc.Textured) {
				std::vector<TexVertex> texvertexs;
				for (UINT i = 0; i < vertexs.size(); i++)
					texvertexs.push_back({ vertexs[i].vector,vertexs[i].norm,texCoord[i] });
				changeBind(std::make_unique<VertexBuffer>(gfx, texvertexs), 0u);
			}
			else
				changeBind(std::make_unique<VertexBuffer>(gfx, vertexs), 0u);

			changeBind(std::make_unique<IndexBuffer>(gfx, indexs), 1u);
		}

		else throw std::exception("Found nullptr while trying to read Explicit function");
		break;
	case _IMPLICIT:

		if (ss.Implicit)
		{
			auto H = ss.Implicit;
			auto regionBegin = ss.minCube;
			auto regionEnd = ss.maxCube;

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
						h[i][j][k] = { x , y , z , H(x, y, z, ss.param) };
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

						addVertexsCube(cube, vertexs, indexs);
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

			changeBind(std::make_unique<VertexBuffer>(gfx, vertexs), 0u);
			changeBind(std::make_unique<IndexBuffer>(gfx, indexs), 1u);
		}

		else throw std::exception("Found nullptr while trying to read Implicit function");
		break;
	case _IMPLICIT_SPHERICAL:

		if (ss.Implicit)
		{
			auto H = ss.Implicit;
			auto regionBegin = ss.minCube;
			auto regionEnd = ss.maxCube;

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
						h[i][j][k] = { x , y , z , H(x, y, z, ss.param) };
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

						addVertexsCube(cube, vertexs, indexs, true);
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

			changeBind(std::make_unique<VertexBuffer>(gfx, vertexs), 0u);
			changeBind(std::make_unique<IndexBuffer>(gfx, indexs), 1u);
		}

		else throw std::exception("Found nullptr while trying to read Implicit function");
		break;
	case _PARAMETRIC:

		if (ss.Parametric_V)
		{
			float epsilon = error_epsilon;
			auto numU = ss.numU;
			auto numV = ss.numV;
			auto minRect = ss.minRect;
			auto maxRect = ss.maxRect;
			auto P = ss.Parametric_V;

			if (!sc.Textured) {
				VertexArr vertexs((numU + 1)* (numV + 1));

				for (UINT i = 0; i < numU + 1; i++) {
					for (UINT j = 0; j < numV + 1; j++) {
						float u = ((numU - i) * minRect.x + i * maxRect.x) / numU;
						float v = ((numV - j) * minRect.y + j * maxRect.y) / numV;
						vertexs.push_back( P(u,v, ss.param) ,
							-((P(u + epsilon,v, ss.param) - P(u - epsilon,v, ss.param)) *
							(P(u,v + epsilon, ss.param) - P(u,v - epsilon, ss.param)))
							.normalize(),
							sc.color );
					}
				}
				changeBind(std::make_unique<VertexBuffer>(gfx, vertexs.data, vertexs.counter), 0u);
			}
			else {
				TexVertexArr vertexs((numU + 1) * (numV + 1));

				for (UINT i = 0; i < numU + 1; i++) {
					for (UINT j = 0; j < numV + 1; j++) {
						float u = ((numU - i) * minRect.x + i * maxRect.x) / numU;
						float v = ((numV - j) * minRect.y + j * maxRect.y) / numV;
						vertexs.push_back( P(u,v, ss.param) ,
							-((P(u + epsilon,v, ss.param) - P(u - epsilon,v, ss.param)) *
							(P(u,v + epsilon, ss.param) - P(u,v - epsilon, ss.param)))
							.normalize(),

							Vector2f((float)i / numU,(float)j / numV) );
					}
				}
				changeBind(std::make_unique<VertexBuffer>(gfx, vertexs.data, vertexs.counter), 0u);
			}

			IndexArr indexs(12 * numU * numV);

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

			changeBind(std::make_unique<IndexBuffer>(gfx, indexs.data, indexs.counter), 1u);
		}

		else if (ss.Parametric_0 && ss.Parametric_1 && ss.Parametric_2)
		{
			float epsilon = error_epsilon;
			auto numU = ss.numU;
			auto numV = ss.numV;
			auto minRect = ss.minRect;
			auto maxRect = ss.maxRect;
			auto x = ss.Parametric_0;
			auto y = ss.Parametric_1;
			auto z = ss.Parametric_2;

			if (!sc.Textured) {
				VertexArr vertexs((numU + 1) * (numV + 1));

				for (UINT i = 0; i < numU + 1; i++) {
					for (UINT j = 0; j < numV + 1; j++) {
						float u = ((numU - i) * minRect.x + i * maxRect.x) / numU;
						float v = ((numV - j) * minRect.y + j * maxRect.y) / numV;
						vertexs.push_back( Vector3f(x(u,v,ss.param), y(u,v,ss.param), z(u,v,ss.param)) ,
							-((Vector3f(x(u + epsilon,v,ss.param), y(u + epsilon,v,ss.param), z(u + epsilon,v,ss.param)) - Vector3f(x(u - epsilon,v,ss.param), y(u - epsilon,v,ss.param), z(u - epsilon,v,ss.param))) *
							(Vector3f(x(u,v + epsilon,ss.param), y(u,v + epsilon,ss.param), z(u,v + epsilon,ss.param)) - Vector3f(x(u,v - epsilon,ss.param), y(u,v - epsilon,ss.param), z(u,v - epsilon,ss.param))))
							.normalize(),
							sc.color );
					}
				}
				changeBind(std::make_unique<VertexBuffer>(gfx, vertexs.data, vertexs.counter), 0u);
			}
			else {
				TexVertexArr vertexs((numU + 1) * (numV + 1));

				for (UINT i = 0; i < numU + 1; i++) {
					for (UINT j = 0; j < numV + 1; j++) {
						float u = ((numU - i) * minRect.x + i * maxRect.x) / numU;
						float v = ((numV - j) * minRect.y + j * maxRect.y) / numV;
						vertexs.push_back( Vector3f(x(u,v,ss.param), y(u,v,ss.param), z(u,v,ss.param)) ,
							-((Vector3f(x(u + epsilon,v,ss.param), y(u + epsilon,v,ss.param), z(u + epsilon,v,ss.param)) - Vector3f(x(u - epsilon,v,ss.param), y(u - epsilon,v,ss.param), z(u - epsilon,v,ss.param))) *
							(Vector3f(x(u,v + epsilon,ss.param), y(u,v + epsilon,ss.param), z(u,v + epsilon,ss.param)) - Vector3f(x(u,v - epsilon,ss.param), y(u,v - epsilon,ss.param), z(u,v - epsilon,ss.param))))
							.normalize(),

							Vector2f((float)i / numU,(float)j / numV) );
					}
				}
				changeBind(std::make_unique<VertexBuffer>(gfx, vertexs.data, vertexs.counter), 0u);
			}

			IndexArr indexs(12 * numU * numV);

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

			changeBind(std::make_unique<IndexBuffer>(gfx, indexs.data, indexs.counter), 1u);
		}

		else throw std::exception("Found nullptr while trying to read Parametric function");
		break;
	case _PARAMETRIC_SPHERICAL:

		if (ss.Parametric_V)
		{
			float epsilon = error_epsilon;
			auto numU = ss.numU;
			auto numV = ss.numV;
			auto minRect = ss.minRect;
			auto maxRect = ss.maxRect;
			auto P = ss.Parametric_V;

			if (!sc.Textured) {
				VertexArr vertexs((numU + 1)* (numV + 1));

				for (UINT i = 0; i < numU + 1; i++) {
					for (UINT j = 0; j < numV + 1; j++) {
						float u = ((numU - i) * minRect.x + i * maxRect.x) / numU;
						float v = ((numV - j) * minRect.y + j * maxRect.y) / numV;
						vertexs.push_back( makePolar(P(u,v,ss.param)) ,

							-((makePolar(P(u + epsilon,v,ss.param)) - makePolar(P(u - epsilon,v,ss.param))) *
							(makePolar(P(u,v + epsilon,ss.param)) - makePolar(P(u,v - epsilon,ss.param))))
							.normalize(),
							sc.color );
					}
				}
				changeBind(std::make_unique<VertexBuffer>(gfx, vertexs.data, vertexs.counter), 0u);
			}
			else {
				TexVertexArr vertexs((numU + 1) * (numV + 1));

				for (UINT i = 0; i < numU + 1; i++) {
					for (UINT j = 0; j < numV + 1; j++) {
						float u = ((numU - i) * minRect.x + i * maxRect.x) / numU;
						float v = ((numV - j) * minRect.y + j * maxRect.y) / numV;
						vertexs.push_back( makePolar(P(u,v,ss.param)) ,

							-((makePolar(P(u + epsilon,v,ss.param)) - makePolar(P(u - epsilon,v,ss.param))) *
							(makePolar(P(u,v + epsilon,ss.param)) - makePolar(P(u,v - epsilon,ss.param))))
							.normalize(),

							Vector2f((float)i / numU,(float)j / numV) );
					}
				}
				changeBind(std::make_unique<VertexBuffer>(gfx, vertexs.data, vertexs.counter), 0u);
			}


			IndexArr indexs(12 * numU * numV);

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

			changeBind(std::make_unique<IndexBuffer>(gfx, indexs.data, indexs.counter), 1u);
		}

		else if (ss.Parametric_0 && ss.Parametric_1 && ss.Parametric_2)
		{
			float epsilon = error_epsilon;
			auto numU = ss.numU;
			auto numV = ss.numV;
			auto minRect = ss.minRect;
			auto maxRect = ss.maxRect;
			auto theta = ss.Parametric_0;
			auto phi = ss.Parametric_1;
			auto rad = ss.Parametric_2;

			if (!sc.Textured) {
				VertexArr vertexs((numU + 1) * (numV + 1));

				for (UINT i = 0; i < numU + 1; i++) {
					for (UINT j = 0; j < numV + 1; j++) {
						float u = ((numU - i) * minRect.x + i * maxRect.x) / numU;
						float v = ((numV - j) * minRect.y + j * maxRect.y) / numV;
						vertexs.push_back( makePolar({theta(u,v,ss.param),phi(u,v,ss.param),rad(u,v,ss.param)}) ,

							-((makePolar({theta(u + epsilon,v,ss.param),phi(u + epsilon,v,ss.param),rad(u + epsilon,v,ss.param)}) -
							makePolar({theta(u - epsilon,v,ss.param),phi(u - epsilon,v,ss.param),rad(u - epsilon,v,ss.param)})) *
							((makePolar({theta(u,v + epsilon,ss.param),phi(u,v + epsilon,ss.param),rad(u,v + epsilon,ss.param)}) -
							makePolar({theta(u,v - epsilon,ss.param),phi(u,v - epsilon,ss.param),rad(u,v - epsilon,ss.param)}))))
							.normalize(),
							sc.color );
					}
				}
				changeBind(std::make_unique<VertexBuffer>(gfx, vertexs.data, vertexs.counter), 0u);
			}
			else {
				TexVertexArr vertexs((numU + 1)* (numV + 1));

				for (UINT i = 0; i < numU + 1; i++) {
					for (UINT j = 0; j < numV + 1; j++) {
						float u = ((numU - i) * minRect.x + i * maxRect.x) / numU;
						float v = ((numV - j) * minRect.y + j * maxRect.y) / numV;
						vertexs.push_back( makePolar({theta(u,v,ss.param),phi(u,v,ss.param),rad(u,v,ss.param)}) ,

							-((makePolar({theta(u + epsilon,v,ss.param),phi(u + epsilon,v,ss.param),rad(u + epsilon,v,ss.param)}) -
							makePolar({theta(u - epsilon,v,ss.param),phi(u - epsilon,v,ss.param),rad(u - epsilon,v,ss.param)})) *
							((makePolar({theta(u,v + epsilon,ss.param),phi(u,v + epsilon,ss.param),rad(u,v + epsilon,ss.param)}) -
							makePolar({theta(u,v - epsilon,ss.param),phi(u,v - epsilon,ss.param),rad(u,v - epsilon,ss.param)}))))
							.normalize() ,

							Vector2f((float)i / numU,(float)j / numV) );
					}
				}
				changeBind(std::make_unique<VertexBuffer>(gfx, vertexs.data, vertexs.counter), 0u);
			}


			IndexArr indexs(12 * numU * numV);

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

			changeBind(std::make_unique<IndexBuffer>(gfx, indexs.data, indexs.counter), 1u);
		}

		else throw std::exception("Found nullptr while trying to read Parametric function");
		break;
	default:
		throw std::exception("The surface type specified is not supported");
	}
}
