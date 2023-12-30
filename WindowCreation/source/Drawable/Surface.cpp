#include "Drawable/Surface.h"

//	Surface shape

SURFACE_SHAPE::SURFACE_SHAPE(SURFACE_TYPE Type, float explicit_(float, float), bool defaultValues, Vector2f min, Vector2f max, UINT numX, UINT numY)
	:Type{ Type }, Explicit{ explicit_ }, minRect{ min }, maxRect{ max }, numU{ numX }, numV{ numY }
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

SURFACE_SHAPE::SURFACE_SHAPE(SURFACE_TYPE Type, float radius(float, float), UINT depth)
	:Type{ Type }, Explicit{ radius }, ICOSPHERE_DEPHT{ depth }
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

SURFACE_SHAPE::SURFACE_SHAPE(SURFACE_TYPE Type, float p0(float, float), float p1(float, float), float p2(float, float), bool defaultValues, Vector2f min, Vector2f max, UINT U, UINT V)
	:Type{ Type }, Parametric_0{ p0 }, Parametric_1{ p1 }, Parametric_2{ p2 }, minRect{ min }, maxRect{ max }, numU{ U }, numV{ V }
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

SURFACE_SHAPE::SURFACE_SHAPE(SURFACE_TYPE Type, Vector3f pv(float, float), bool defaultValues, Vector2f min, Vector2f max, UINT U, UINT V)
	:Type{ Type }, Parametric_V{ pv }, minRect{ min }, maxRect{ max }, numU{ U }, numV{ V }
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

SURFACE_SHAPE::SURFACE_SHAPE(SURFACE_TYPE Type, float imp(float, float, float), Vector3f min, Vector3f max)
	:Type{ Type }, Implicit{ imp }, minCube{ min }, maxCube{ max }
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

SURFACE_SHAPE::SURFACE_SHAPE(const SURFACE_SHAPE& other) :
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

//	Constructors

Surface::Surface(Graphics& gfx, SURFACE_SHAPE ss, SURFACE_COLORING* psc)
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
						vertexs.push_back({ Vector3f(x, y, F(x,y)) ,
							-((Vector3f(x + epsilon, y, F(x + epsilon,y)) - Vector3f(x - epsilon, y, F(x - epsilon,y))) *
							(Vector3f(x, y + epsilon, F(x,y + epsilon)) - Vector3f(x, y - epsilon, F(x,y - epsilon)))).normalize(),
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
						vertexs.push_back({ Vector3f(x, y, F(x,y)) ,
							-((Vector3f(x + epsilon, y, F(x + epsilon,y)) - Vector3f(x - epsilon, y, F(x - epsilon,y))) *
							(Vector3f(x, y + epsilon, F(x,y + epsilon)) - Vector3f(x, y - epsilon, F(x,y - epsilon)))).normalize() ,
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
						vertexs.push_back({ evalPolar(r,theta,phi) ,

							-((evalPolar(r,theta + epsilon,phi) - evalPolar(r,theta - epsilon,phi)) *
							((evalPolar(r,theta,phi + epsilon) - evalPolar(r,theta,phi - epsilon))))
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
						vertexs.push_back({ evalPolar(r,theta,phi) ,

							-((evalPolar(r,theta + epsilon,phi) - evalPolar(r,theta - epsilon,phi)) *
							((evalPolar(r,theta,phi + epsilon) - evalPolar(r,theta,phi - epsilon))))
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
					-((evalPolar(r, theta + epsilon, phi) - evalPolar(r, theta - epsilon, phi)) *
						((evalPolar(r, theta, phi + epsilon) - evalPolar(r, theta, phi - epsilon))))
					.normalize();

				v.vector *= r(theta, phi);
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
						h[i][j][k] = { x , y , z , H(x, y, z) };
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
						h[i][j][k] = { x , y , z , H(x, y, z) };
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
						vertexs.push_back({ P(u,v) ,
							-((P(u + epsilon,v) - P(u - epsilon,v)) *
							(P(u,v + epsilon) - P(u,v - epsilon)))
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
						vertexs.push_back({ P(u,v) ,
							-((P(u + epsilon,v) - P(u - epsilon,v)) *
							(P(u,v + epsilon) - P(u,v - epsilon)))
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
						vertexs.push_back({ Vector3f(x(u,v), y(u,v), z(u,v)) ,
							-((Vector3f(x(u + epsilon,v), y(u + epsilon,v), z(u + epsilon,v)) - Vector3f(x(u - epsilon,v), y(u - epsilon,v), z(u - epsilon,v))) *
							(Vector3f(x(u,v + epsilon), y(u,v + epsilon), z(u,v + epsilon)) - Vector3f(x(u,v - epsilon), y(u,v - epsilon), z(u,v - epsilon))))
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
						vertexs.push_back({ Vector3f(x(u,v), y(u,v), z(u,v)) ,
							-((Vector3f(x(u + epsilon,v), y(u + epsilon,v), z(u + epsilon,v)) - Vector3f(x(u - epsilon,v), y(u - epsilon,v), z(u - epsilon,v))) *
							(Vector3f(x(u,v + epsilon), y(u,v + epsilon), z(u,v + epsilon)) - Vector3f(x(u,v - epsilon), y(u,v - epsilon), z(u,v - epsilon))))
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
						vertexs.push_back({ makePolar(P(u,v)) ,

							-((makePolar(P(u + epsilon,v)) - makePolar(P(u - epsilon,v))) *
							(makePolar(P(u,v + epsilon)) - makePolar(P(u,v - epsilon))))
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
						vertexs.push_back({ makePolar(P(u,v)) ,

							-((makePolar(P(u + epsilon,v)) - makePolar(P(u - epsilon,v))) *
							(makePolar(P(u,v + epsilon)) - makePolar(P(u,v - epsilon))))
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
						vertexs.push_back({ makePolar({ theta(u,v), phi(u,v), rad(u,v) }),

							-((makePolar({ theta(u + epsilon,v), phi(u + epsilon,v), rad(u + epsilon,v) }) -
							makePolar({ theta(u - epsilon,v), phi(u - epsilon,v), rad(u - epsilon,v) })) *
							(makePolar({ theta(u,v + epsilon), phi(u,v + epsilon), rad(u,v + epsilon) }) -
							makePolar({ theta(u,v - epsilon), phi(u,v - epsilon), rad(u,v - epsilon) })))
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
						vertexs.push_back({ makePolar({ theta(u,v), phi(u,v), rad(u,v) }),

							-((makePolar({ theta(u + epsilon,v), phi(u + epsilon,v), rad(u + epsilon,v) }) -
							makePolar({ theta(u - epsilon,v), phi(u - epsilon,v), rad(u - epsilon,v) })) *
							(makePolar({ theta(u,v + epsilon), phi(u,v + epsilon), rad(u,v + epsilon) }) -
							makePolar({ theta(u,v - epsilon), phi(u,v - epsilon), rad(u,v - epsilon) })))
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
	default:
		throw std::exception("The surface type specified is not supported");
	}
	addOtherBinds(gfx);
}

//	Public

void Surface::updateRotation(Graphics& gfx, float rotationZ, float rotationX, Vector3f position)
{
	vscBuff.rotattion = (ZRotationMatrix(rotationZ) * XRotationMatrix(rotationX)).transpose().getMatrix4();
	vscBuff.traslation = position.getVector4();
	pVSCB->Update(gfx, vscBuff);
}

void Surface::updateTexture(Graphics& gfx, UINT id, std::string texture)
{
	if (!sc.Textured)
		throw std::exception(std::string("ERROR: You cannot call a texture update in a surface that wasn't initialized as textured\nREF: [" + texture + "]").c_str());
	if (id > 1u)
		throw std::exception("ERROR: The given id to update the texture is not valid, id must be 0 or 1");

	changeBind(std::make_unique<Texture>(gfx, texture, id), id + 2);
}

void Surface::updateTexture(Graphics& gfx, UINT id, Texture texture)
{
	if (!sc.Textured)
		throw std::exception("ERROR: You cannot call a texture update in a surface that wasn't initialized as textured");
	if (id > 1u)
		throw std::exception("ERROR: The given id to update the texture is not valid, id must be 0 or 1");

	((Texture*)changeBind(std::make_unique<Texture>(texture), id + 2))->setSlot(id);
}

void Surface::updateTextures(Graphics& gfx, Texture texture0, Texture texture1)
{
	if (!sc.Textured)
		throw std::exception("ERROR: You cannot call a texture update in a surface that wasn't initialized as textured");

	changeBind(std::make_unique<Texture>(texture0, 0u), 4u);
	changeBind(std::make_unique<Texture>(texture1, 1u), 5u);
}

void Surface::updateTextures(Graphics& gfx, std::string texture0, std::string texture1)
{
	if (!sc.Textured)
		throw std::exception("ERROR: You cannot call a texture update in a surface that wasn't initialized as textured");

	changeBind(std::make_unique<Texture>(gfx, texture0, 0u), 4u);
	changeBind(std::make_unique<Texture>(gfx, texture1, 1u), 5u);
}

void Surface::updateLight(Graphics& gfx, UINT id, Vector2f intensity, Color color, Vector3f position)
{
	if(!pPSCB)
		throw std::exception("ERROR: You cannot call a light update in a surface that wasn't initialized as lighted");

	pscBuff.lightsource[id] = { intensity.getVector4() , color.getColor4() , position.getVector4() };
	pPSCB->Update(gfx, pscBuff);
}

void Surface::updateLight(Graphics& gfx, UINT id, _float4vector intensity, _float4color color, _float4vector position)
{
	if (!pPSCB)
		throw std::exception("ERROR: You cannot call a light update in a surface that wasn't initialized as lighted");

	pscBuff.lightsource[id] = { intensity , color , position };
	pPSCB->Update(gfx, pscBuff);
}

void Surface::clearLights(Graphics& gfx)
{
	pscBuff = {};
	pPSCB->Update(gfx, pscBuff);
}

void Surface::updateShape(Graphics& gfx, SURFACE_SHAPE ss)
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
						vertexs.push_back(Vector3f(x, y, F(x, y)),
							-((Vector3f(x + epsilon, y, F(x + epsilon, y)) - Vector3f(x - epsilon, y, F(x - epsilon, y))) *
								(Vector3f(x, y + epsilon, F(x, y + epsilon)) - Vector3f(x, y - epsilon, F(x, y - epsilon)))).normalize(),
							sc.color);
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
						vertexs.push_back(Vector3f(x, y, F(x, y)),
							-((Vector3f(x + epsilon, y, F(x + epsilon, y)) - Vector3f(x - epsilon, y, F(x - epsilon, y))) *
								(Vector3f(x, y + epsilon, F(x, y + epsilon)) - Vector3f(x, y - epsilon, F(x, y - epsilon)))).normalize(),
							Vector2f((float)i / numX, (float)j / numY));
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
						vertexs.push_back(makePolar({ theta,phi, r(theta,phi) }),

							-((makePolar({ theta + epsilon,phi, r(theta + epsilon,phi) }) - makePolar({ theta - epsilon,phi, r(theta - epsilon,phi) })) *
								(makePolar({ theta,phi + epsilon, r(theta,phi + epsilon) }) - makePolar({ theta,phi - epsilon, r(theta,phi - epsilon) })))
							.normalize(),
							sc.color);
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
						vertexs.push_back(makePolar({ theta,phi, r(theta,phi) }),

							-((makePolar({ theta + epsilon,phi, r(theta + epsilon,phi) }) - makePolar({ theta - epsilon,phi, r(theta - epsilon,phi) })) *
								(makePolar({ theta,phi + epsilon, r(theta,phi + epsilon) }) - makePolar({ theta,phi - epsilon, r(theta,phi - epsilon) })))
							.normalize(),

							Vector2f((float)i / numX, (float)j / numY));
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
					-((makePolar({ theta + epsilon, phi, r(theta + epsilon, phi) }) - makePolar({ theta - epsilon, phi, r(theta - epsilon, phi) })) *
						(makePolar({ theta, phi + epsilon, r(theta, phi + epsilon) }) - makePolar({ theta, phi - epsilon, r(theta, phi - epsilon) })))
					.normalize();

				v.vector *= r(theta, phi);
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
						h[i][j][k] = { x , y , z , H(x, y, z) };
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
						h[i][j][k] = { x , y , z , H(x, y, z) };
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
				VertexArr vertexs((numU + 1) * (numV + 1));

				for (UINT i = 0; i < numU + 1; i++) {
					for (UINT j = 0; j < numV + 1; j++) {
						float u = ((numU - i) * minRect.x + i * maxRect.x) / numU;
						float v = ((numV - j) * minRect.y + j * maxRect.y) / numV;
						vertexs.push_back(P(u, v),
							-((P(u + epsilon, v) - P(u - epsilon, v)) *
								(P(u, v + epsilon) - P(u, v - epsilon)))
							.normalize(),
							sc.color);
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
						vertexs.push_back(P(u, v),
							-((P(u + epsilon, v) - P(u - epsilon, v)) *
								(P(u, v + epsilon) - P(u, v - epsilon)))
							.normalize(),

							Vector2f((float)i / numU, (float)j / numV));
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
						vertexs.push_back(Vector3f(x(u, v), y(u, v), z(u, v)),
							-((Vector3f(x(u + epsilon, v), y(u + epsilon, v), z(u + epsilon, v)) - Vector3f(x(u - epsilon, v), y(u - epsilon, v), z(u - epsilon, v))) *
								(Vector3f(x(u, v + epsilon), y(u, v + epsilon), z(u, v + epsilon)) - Vector3f(x(u, v - epsilon), y(u, v - epsilon), z(u, v - epsilon))))
							.normalize(),
							sc.color);
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
						vertexs.push_back(Vector3f(x(u, v), y(u, v), z(u, v)),
							-((Vector3f(x(u + epsilon, v), y(u + epsilon, v), z(u + epsilon, v)) - Vector3f(x(u - epsilon, v), y(u - epsilon, v), z(u - epsilon, v))) *
								(Vector3f(x(u, v + epsilon), y(u, v + epsilon), z(u, v + epsilon)) - Vector3f(x(u, v - epsilon), y(u, v - epsilon), z(u, v - epsilon))))
							.normalize(),

							Vector2f((float)i / numU, (float)j / numV));
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
				VertexArr vertexs((numU + 1) * (numV + 1));

				for (UINT i = 0; i < numU + 1; i++) {
					for (UINT j = 0; j < numV + 1; j++) {
						float u = ((numU - i) * minRect.x + i * maxRect.x) / numU;
						float v = ((numV - j) * minRect.y + j * maxRect.y) / numV;
						vertexs.push_back(makePolar(P(u, v)),

							-((makePolar(P(u + epsilon, v)) - makePolar(P(u - epsilon, v))) *
								(makePolar(P(u, v + epsilon)) - makePolar(P(u, v - epsilon))))
							.normalize(),
							sc.color);
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
						vertexs.push_back(makePolar(P(u, v)),

							-((makePolar(P(u + epsilon, v)) - makePolar(P(u - epsilon, v))) *
								(makePolar(P(u, v + epsilon)) - makePolar(P(u, v - epsilon))))
							.normalize(),

							Vector2f((float)i / numU, (float)j / numV));
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
						vertexs.push_back(makePolar({ theta(u,v),phi(u,v),rad(u,v) }),

							-((makePolar({ theta(u + epsilon,v),phi(u + epsilon,v),rad(u + epsilon,v) }) -
								makePolar({ theta(u - epsilon,v),phi(u - epsilon,v),rad(u - epsilon,v) })) *
								((makePolar({ theta(u,v + epsilon),phi(u,v + epsilon),rad(u,v + epsilon) }) -
									makePolar({ theta(u,v - epsilon),phi(u,v - epsilon),rad(u,v - epsilon) }))))
							.normalize(),
							sc.color);
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
						vertexs.push_back(makePolar({ theta(u,v),phi(u,v),rad(u,v) }),

							-((makePolar({ theta(u + epsilon,v),phi(u + epsilon,v),rad(u + epsilon,v) }) -
								makePolar({ theta(u - epsilon,v),phi(u - epsilon,v),rad(u - epsilon,v) })) *
								((makePolar({ theta(u,v + epsilon),phi(u,v + epsilon),rad(u,v + epsilon) }) -
									makePolar({ theta(u,v - epsilon),phi(u,v - epsilon),rad(u,v - epsilon) }))))
							.normalize(),

							Vector2f((float)i / numU, (float)j / numV));
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

//	Private

void Surface::addOtherBinds(Graphics& gfx)
{
	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	vscBuff = { Matrix::Identity.getMatrix4(), Vector3f().getVector4() };
	pVSCB = (ConstantBuffer<VSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<VSconstBuffer>>(gfx, vscBuff, VERTEX_CONSTANT_BUFFER_TYPE));

	if (sc.Textured)
	{
		AddBind(std::make_unique<Texture>(*sc.texture));

		if (sc.textureNight)
			AddBind(std::make_unique<Texture>(*sc.textureNight, 1u));
		else
			AddBind(std::make_unique<Texture>(*sc.texture, 1u));

		VertexShader* pvs = NULL;
		if (sc.Lighted)
		{
			pvs = (VertexShader*)AddBind(std::move(std::make_unique<VertexShader>(gfx, SHADERS_DIR + std::wstring(L"TexSurfaceVS.cso"))));
			AddBind(std::make_unique<PixelShader>(gfx, SHADERS_DIR + std::wstring(L"TexSurfacePS.cso")));

			float unused = 0.f;
			if (sc.DefaultInitialLights)
				pscBuff = { 32000.f,5000.f,unused,unused,1.f, 1.f, 1.f, 1.f ,160.f, 0.f, 60.f,unused };
			else {
				for (int i = 0; i < 8; i++)
					pscBuff.lightsource[i] = sc.lightsource[i];
			}
			pPSCB = (ConstantBuffer<PSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<PSconstBuffer>>(gfx, pscBuff, PIXEL_CONSTANT_BUFFER_TYPE));
		}
		else
		{
			pvs = (VertexShader*)AddBind(std::move(std::make_unique<VertexShader>(gfx, SHADERS_DIR + std::wstring(L"UnlitSurfaceVS.cso"))));
			AddBind(std::make_unique<PixelShader>(gfx, SHADERS_DIR + std::wstring(L"UnlitSurfacePS.cso")));
		}


		std::vector< D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};

		AddBind(std::make_unique<InputLayout>(gfx, ied, pvs->GetBytecode()));

		AddBind(std::make_unique<Sampler>(gfx, D3D11_FILTER_MIN_MAG_MIP_LINEAR));


	}
	else
	{
		VertexShader* pvs = NULL;
		if (sc.Lighted)
		{
			pvs = (VertexShader*)AddBind(std::move(std::make_unique<VertexShader>(gfx, SHADERS_DIR + std::wstring(L"SurfaceVS.cso"))));
			AddBind(std::make_unique<PixelShader>(gfx, SHADERS_DIR + std::wstring(L"SurfacePS.cso")));

			float unused = 0.f;
			if (sc.DefaultInitialLights)
				pscBuff = {
					60.f,0.f,unused,unused,1.0f, 0.2f, 0.2f, 1.f , 0.f, 8.f, 8.f,unused,
					60.f,0.f,unused,unused,0.0f, 1.0f, 0.0f, 1.f , 0.f,-8.f, 8.f,unused,
					60.f,0.f,unused,unused,0.5f, 0.0f, 1.0f, 1.f ,-8.f, 0.f,-8.f,unused,
					60.f,0.f,unused,unused,1.0f, 1.0f, 0.0f, 1.f , 8.f, 0.f, 8.f,unused,
			};
			else {
				for (int i = 0; i < 8; i++)
					pscBuff.lightsource[i] = sc.lightsource[i];
			}
			pPSCB = (ConstantBuffer<PSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<PSconstBuffer>>(gfx, pscBuff, PIXEL_CONSTANT_BUFFER_TYPE));
		}
		else
		{
			pvs = (VertexShader*)AddBind(std::move(std::make_unique<VertexShader>(gfx, SHADERS_DIR + std::wstring(L"UnlitUntexVS.cso"))));
			AddBind(std::make_unique<PixelShader>(gfx, SHADERS_DIR + std::wstring(L"UnlitUntexPS.cso")));
		}

		std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Color",0,DXGI_FORMAT_B8G8R8A8_UNORM,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};

		AddBind(std::make_unique<InputLayout>(gfx, ied, pvs->GetBytecode()));
	}

}

Vector3f Surface::evalPolar(float r(float, float), float theta, float phi)
{
	return r(theta, phi) * Vector3f(cosf(theta) * cosf(phi), sinf(theta) * cosf(phi), sinf(phi));
}

Vector3f Surface::makePolar(Vector3f other)
{
	return other.z * Vector3f(cosf(other.y) * cosf(other.x), cosf(other.y) * sinf(other.x), sinf(other.y));
}

void Surface::addVertexsCube(_float4vector cube[8], std::vector<Vertex>& vertexs, std::vector<unsigned short>& indexs, bool polar)
{
	struct triangle {
		unsigned short v0;
		unsigned short v1;
		unsigned short v2;

		triangle(UINT x, UINT y, UINT z)
		{
			v0 = unsigned short(x);
			v1 = unsigned short(y);
			v2 = unsigned short(z);
		}
	};

	std::vector<triangle> triangles;

	UINT vi = (UINT)vertexs.size();

	Vector2i cases[] = { {0 , 1} , {0 , 3} , {0 , 4} ,  {1 , 2} ,  {1 , 5} ,  {3 , 2} ,  {3 , 7} ,  {4 , 5} ,  {4 , 7} ,  {2 , 6} ,  {5 , 6} ,  {7 , 6} };

	unsigned char b = 0x0;
	for (int i = 0; i < 8; i++) {
		if (cube[i].w <= 0)
			b |= 0x1 << i;
	}

	for (Vector2i& c : cases) {
		if (((b & (0x1 << c.x)) && !(b & (0x1 << c.y))) || (!(b & (0x1 << c.x)) && (b & (0x1 << c.y))))
		{
			Vector3f p0 = { cube[c.x].x ,cube[c.x].y ,cube[c.x].z };
			Vector3f p1 = { cube[c.y].x ,cube[c.y].y ,cube[c.y].z };
			float w0 = cube[c.x].w;
			float w1 = cube[c.y].w;
			if (polar)
				vertexs.push_back(Vertex(makePolar((p0 * w1 - p1 * w0) / (w1 - w0)), ((p0 * w0 - p1 * w1) / (w1 - w0)).normalize(), Color::White));
			else
				vertexs.push_back(Vertex((p0 * w1 - p1 * w0) / (w1 - w0), ((p0 * w0 - p1 * w1) / (w1 - w0)).normalize(), Color::White));
		}

	}

	constexpr unsigned char p0 = 0x01;
	constexpr unsigned char p1 = 0x02;
	constexpr unsigned char p2 = 0x04;
	constexpr unsigned char p3 = 0x08;
	constexpr unsigned char p4 = 0x10;
	constexpr unsigned char p5 = 0x20;
	constexpr unsigned char p6 = 0x40;
	constexpr unsigned char p7 = 0x80;

#define CASE(p) case (p):case 0xff - (p):
#define T(v0,v1,v2) triangles.push_back(triangle(vi + v0,vi + v1,vi + v2))

	switch (b)
	{
	CASE(0x00)
		break;
	CASE(p0)
	CASE(p1)
	CASE(p2)
	CASE(p3)
	CASE(p4)
	CASE(p5)
	CASE(p6)
	CASE(p7)
		T(0,1,2);
		break;
	CASE(p0 | p1)
	CASE(p1 | p2)
	CASE(p2 | p3)
	CASE(p3 | p0)
	CASE(p4 | p5)
	CASE(p5 | p6)
	CASE(p6 | p7)
	CASE(p7 | p4)
	CASE(p0 | p4)
	CASE(p1 | p5)
	CASE(p2 | p6)
	CASE(p3 | p7)
	CASE(p0 | p1 | p2 | p3)
	CASE(p0 | p1 | p4 | p5)
	CASE(p0 | p3 | p4 | p7)
		T(0, 1, 2);
		T(1, 2, 3);
		break;
	CASE(p0 | p1 | p3)
		T(0, 2, 4);
		T(1, 2, 3);
		T(2, 3, 4);
		break;
	CASE(p0 | p4 | p1)
		T(0, 1, 4);
		T(1, 2, 4);
		T(2, 4, 3);
		break;
	CASE(p0 | p3 | p4)
		T(0, 1, 3);
		T(1, 2, 3);
		T(2, 3, 4);
		break;
	CASE(p1 | p0 | p2)
		T(1, 2, 4);
		T(0, 1, 3);
		T(1, 3, 4);
		break;
	CASE(p1 | p2 | p5)
	CASE(p3 | p2 | p7)
	CASE(p4 | p5 | p7)
		T(0, 1, 2);
		T(1, 2, 3);
		T(2, 3, 4);
		break;
	CASE(p1 | p0 | p5)
	CASE(p2 | p6 | p1)
	CASE(p3 | p7 | p0)
	CASE(p5 | p6 | p1)
	CASE(p7 | p6 | p3)
		T(0, 2, 4);
		T(0, 1, 4);
		T(1, 4, 3);
		break;
	CASE(p2 | p1 | p3)
	CASE(p5 | p1 | p4)
	CASE(p6 | p2 | p5)
	CASE(p7 | p3 | p4)
		T(2, 3, 4);
		T(0, 1, 2);
		T(1, 2, 3);
		break;
	CASE(p2 | p3 | p6)
	CASE(p5 | p4 | p6)
	CASE(p7 | p4 | p6)
		T(0, 1, 3);
		T(0, 2, 3);
		T(2, 3, 4);
		break;
	CASE(p3 | p0 | p2)
	CASE(p4 | p0 | p5)
		T(1, 3, 4);
		T(1, 4, 0);
		T(4, 0, 2);
		break;
	CASE(p4 | p7 | p0)
		T(0, 3, 4);
		T(0, 4, 1);
		T(4, 1, 2);
		break;
	CASE(p6 | p5 | p7)
		T(0, 1, 4);
		T(0, 1, 2);
		T(1, 2, 3);
		break;
	CASE(p6 | p7 | p2)
		T(0, 3, 4);
		T(0, 3, 1);
		T(3, 1, 2);
		break;
	CASE(p0 | p1 | p3 | p4)
		T(0, 1, 4);
		T(0, 4, 2);
		T(4, 2, 5);
		T(2, 5, 3);
		break;
	CASE(p1 | p2 | p0 | p5)
		T(1, 3, 5);
		T(1, 5, 0);
		T(5, 0, 4);
		T(0, 4, 2);
		break;
	CASE(p2 | p3 | p1 | p6)
		T(0, 2, 4);
		T(0, 4, 5);
		T(0, 5, 1);
		T(5, 1, 3);
		break;
	CASE(p3 | p0 | p2 | p7)
		T(0, 2, 4);
		T(0, 4, 5);
		T(0, 5, 1);
		T(5, 1, 3);
		break;

	default:
		throw std::exception("binggoo");
		break;
	}

#undef CASE
#undef T

	for (auto& t : triangles) {
		indexs.push_back(t.v0);
		indexs.push_back(t.v1);
		indexs.push_back(t.v2);
		indexs.push_back(t.v0);
		indexs.push_back(t.v2);
		indexs.push_back(t.v1);
	}
}
