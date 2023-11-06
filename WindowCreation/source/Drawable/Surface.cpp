#include "Drawable/Surface.h"

//	Constructors

Surface::Surface(Graphics& gfx, SURFACE_TYPE Type, float F0(float, float), bool defaultValues, Vector2f minRect, Vector2f maxRect, UINT numX, UINT numY)
{
	switch (Type)
	{
	case _EXPLICIT:
		if (defaultValues) {
			minRect = { -1.f,-1.f };
			maxRect = {  1.f, 1.f };
		}
		generateExplicit(gfx, F0, minRect, maxRect, numX, numY);
		break;
	case _RADIAL_SPHERICAL:
		if (defaultValues) {
			minRect = { 0.f, pi / 2.f };
			maxRect = { 2.f * pi, -pi / 2.f };
		}
		generatePolarNormal(gfx, F0, minRect, maxRect, numX, numY);
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
	addDefaultBinds(gfx);
}

Surface::Surface(Graphics& gfx, SURFACE_TYPE Type, float F0(float, float), std::string Texture0, std::string Texture1, bool defaultValues, Vector2f minRect, Vector2f maxRect, UINT numX, UINT numY)
{
	switch (Type)
	{
		if (defaultValues) {
			minRect = { -1.f,-1.f };
			maxRect = { 1.f, 1.f };
		}
	case _EXPLICIT:
		generateExplicit(gfx, F0, minRect, maxRect, numX, numY, true);
		break;
	case _RADIAL_SPHERICAL:
		if (defaultValues) {
			minRect = { 0.f, pi / 2.f };
			maxRect = { 2.f * pi, -pi / 2.f };
		}
		generatePolarNormal(gfx, F0, minRect, maxRect, numX, numY, true);
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
	addTexturedBinds(gfx, Texture0, Texture1);
}

Surface::Surface(Graphics& gfx, SURFACE_TYPE Type, float F0(float, float), Texture texture0, Texture texture1, bool defaultValues, Vector2f minRect, Vector2f maxRect, UINT numX, UINT numY)
{
	switch (Type)
	{
		if (defaultValues) {
			minRect = { -1.f,-1.f };
			maxRect = { 1.f, 1.f };
		}
	case _EXPLICIT:
		generateExplicit(gfx, F0, minRect, maxRect, numX, numY, true);
		break;
	case _RADIAL_SPHERICAL:
		if (defaultValues) {
			minRect = { 0.f, pi / 2.f };
			maxRect = { 2.f * pi, -pi / 2.f };
		}
		generatePolarNormal(gfx, F0, minRect, maxRect, numX, numY, true);
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
	addTexturedBinds(gfx, texture0, texture1);
}

Surface::Surface(Graphics& gfx, SURFACE_TYPE Type, float rad(float, float), UINT ICOSPHERE_DEPTH)
{
	switch (Type)
	{
	case _RADIAL_ICOSPHERE:
		generatePolarIcosphere(gfx, rad, ICOSPHERE_DEPTH);
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
	addDefaultBinds(gfx);
}

Surface::Surface(Graphics& gfx, SURFACE_TYPE Type, float rad(float, float), UINT ICOSPHERE_DEPTH, std::string Texture0, std::string Texture1)
{
	switch (Type)
	{
	case _RADIAL_ICOSPHERE:
		generatePolarIcosphere(gfx, rad, ICOSPHERE_DEPTH, true);
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
	addTexturedBinds(gfx, Texture0, Texture1);
}

Surface::Surface(Graphics& gfx, SURFACE_TYPE Type, float rad(float, float), UINT ICOSPHERE_DEPTH, Texture texture0, Texture texture1)
{
	switch (Type)
	{
	case _RADIAL_ICOSPHERE:
		generatePolarIcosphere(gfx, rad, ICOSPHERE_DEPTH, true);
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
	addTexturedBinds(gfx, texture0, texture1);
}

Surface::Surface(Graphics& gfx, SURFACE_TYPE Type, float F0(float, float), float F1(float, float), float F2(float, float), bool defaultValues, Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV)
{
	switch (Type)
	{
	case _PARAMETRIC:
		if (defaultValues) {
			minRect = { -1.f,-1.f };
			maxRect = { 1.f, 1.f };
		}
		generateParametric(gfx, F0, F1, F2, minRect, maxRect, numU, numV);
		break;
	case _PARAMETRIC_SPHERICAL:
		if (defaultValues) {
			minRect = { 0.f, pi / 2.f };
			maxRect = { 2.f * pi, -pi / 2.f };
		}
		generatePolarParametric(gfx, F0, F1, F2, minRect, maxRect, numU, numV);
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
	addDefaultBinds(gfx);
}

Surface::Surface(Graphics& gfx, SURFACE_TYPE Type, float F0(float, float), float F1(float, float), float F2(float, float), std::string Texture0, std::string Texture1, bool defaultValues, Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV)
{
	switch (Type)
	{
	case _PARAMETRIC:
		if (defaultValues) {
			minRect = { -1.f,-1.f };
			maxRect = { 1.f, 1.f };
		}
		generateParametric(gfx, F0, F1, F2, minRect, maxRect, numU, numV, true);
		break;
	case _PARAMETRIC_SPHERICAL:
		if (defaultValues) {
			minRect = { 0.f, pi / 2.f };
			maxRect = { 2.f * pi, -pi / 2.f };
		}
		generatePolarParametric(gfx, F0, F1, F2, minRect, maxRect, numU, numV, true);
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
	addTexturedBinds(gfx, Texture0, Texture1);
}

Surface::Surface(Graphics& gfx, SURFACE_TYPE Type, float F0(float, float), float F1(float, float), float F2(float, float), Texture texture0, Texture texture1, bool defaultValues, Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV)
{
	switch (Type)
	{
	case _PARAMETRIC:
		if (defaultValues) {
			minRect = { -1.f,-1.f };
			maxRect = { 1.f, 1.f };
		}
		generateParametric(gfx, F0, F1, F2, minRect, maxRect, numU, numV, true);
		break;
	case _PARAMETRIC_SPHERICAL:
		if (defaultValues) {
			minRect = { 0.f, pi / 2.f };
			maxRect = { 2.f * pi, -pi / 2.f };
		}
		generatePolarParametric(gfx, F0, F1, F2, minRect, maxRect, numU, numV, true);
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
	addTexturedBinds(gfx, texture0, texture1);
}

Surface::Surface(Graphics& gfx, SURFACE_TYPE Type, float H(float, float, float))
{
	switch (Type)
	{
	case _IMPLICIT:
		generateImplicit(gfx, H);
		break;
	case _IMPLICIT_SPHERICAL:
		generateImplicitPolar(gfx, H);
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
	addDefaultBinds(gfx);
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
	if (!textured)
		throw std::exception(std::string("ERROR: You cannot call a texture update in a surface that wasn't initialized as textured\nREF: [" + texture + "]").c_str());
	if (id > 1u)
		throw std::exception("ERROR: The given id to update the texture is not valid, id must be 0 or 1");

	changeBind(std::make_unique<Texture>(gfx, texture, id), id + 2);
}

void Surface::updateTexture(Graphics& gfx, UINT id, Texture texture)
{
	if (!textured)
		throw std::exception("ERROR: You cannot call a texture update in a surface that wasn't initialized as textured");
	if (id > 1u)
		throw std::exception("ERROR: The given id to update the texture is not valid, id must be 0 or 1");

	((Texture*)changeBind(std::make_unique<Texture>(texture), id + 2))->setSlot(id);
}

void Surface::updateTextures(Graphics& gfx, Texture texture0, Texture texture1)
{
	if (!textured)
		throw std::exception("ERROR: You cannot call a texture update in a surface that wasn't initialized as textured");

	((Texture*)changeBind(std::make_unique<Texture>(texture0), 2u))->setSlot(0u);
	((Texture*)changeBind(std::make_unique<Texture>(texture1), 3u))->setSlot(1u);
}

void Surface::updateTextures(Graphics& gfx, std::string texture0, std::string texture1)
{
	if (!textured)
		throw std::exception("ERROR: You cannot call a texture update in a surface that wasn't initialized as textured");

	changeBind(std::make_unique<Texture>(gfx, texture0, 0u), 2u);
	changeBind(std::make_unique<Texture>(gfx, texture1, 1u), 3u);
}

void Surface::updateLight(Graphics& gfx, UINT id, Vector2f intensity, Color color, Vector3f position)
{
	pscBuff.lightsource[id] = { intensity.getVector4() , color.getColor4() , position.getVector4() };
	pPSCB->Update(gfx, pscBuff);
}

void Surface::updateLight(Graphics& gfx, UINT id, _float4vector intensity, _float4color color, _float4vector position)
{
	pscBuff.lightsource[id] = { intensity , color , position };
	pPSCB->Update(gfx, pscBuff);
}

void Surface::clearLights(Graphics& gfx)
{
	pscBuff = {};
	pPSCB->Update(gfx, pscBuff);
}

//	Private

void Surface::generateExplicit(Graphics& gfx, float F(float, float), Vector2f minRect, Vector2f maxRect, UINT numX, UINT numY, bool Textured)
{
	float epsilon = error_epsilon;
	if (!Textured) {
		std::vector<Vertex> vertexs;
		
		for (UINT i = 0; i < numX + 1; i++) {
			for (UINT j = 0; j < numY + 1; j++) {
				float x = ((numX - i) * minRect.x + i * maxRect.x) / numX;
				float y = ((numY - j) * minRect.y + j * maxRect.y) / numY;
				vertexs.push_back({ Vector3f(x, y, F(x,y)) ,
					-((Vector3f(x + epsilon, y, F(x + epsilon,y)) - Vector3f(x - epsilon, y, F(x - epsilon,y))) * 
					(Vector3f(x, y + epsilon, F(x,y + epsilon)) - Vector3f(x, y - epsilon, F(x,y - epsilon)))).normalize() });
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

void Surface::generatePolarNormal(Graphics& gfx, float r(float, float), Vector2f minRect, Vector2f maxRect, UINT numX, UINT numY, bool Textured)
{
	float epsilon = error_epsilon;

	if (!Textured) {
		std::vector<Vertex> vertexs;

		for (UINT i = 0; i < numX + 1; i++) {
			for (UINT j = 0; j < numY + 1; j++) {
				float theta = ((numX - i) * minRect.x + i * maxRect.x) / numX;
				float phi = ((numY - j) * minRect.y + j * maxRect.y) / numY;
				vertexs.push_back({ evalPolar(r,theta,phi) ,

					-((evalPolar(r,theta + epsilon,phi) - evalPolar(r,theta - epsilon,phi)) *
					((evalPolar(r,theta,phi + epsilon) - evalPolar(r,theta,phi - epsilon))))
					.normalize() });
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

void Surface::generatePolarIcosphere(Graphics& gfx, float r(float, float), UINT depth, bool Textured)
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
			-((evalPolar(r, theta + epsilon, phi) - evalPolar(r, theta - epsilon, phi)) *
			((evalPolar(r, theta, phi + epsilon) - evalPolar(r, theta, phi - epsilon))))
			.normalize();

		v.vector *= r(theta, phi);
		texCoord.push_back({ theta / 2.f / pi,(pi / 2.f - phi) / pi });
	}

	if (Textured) {
		std::vector<TexVertex> texvertexs;
		for (UINT i = 0; i < vertexs.size(); i++)
			texvertexs.push_back({ vertexs[i].vector,vertexs[i].norm,texCoord[i] });
		AddBind(std::make_unique<VertexBuffer>(gfx, texvertexs));
	}
	else
		AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));

	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));

}

void Surface::generateParametric(Graphics& gfx, float x(float, float), float y(float, float), float z(float, float), Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV, bool Textured)
{
	float epsilon = error_epsilon;

	if (!Textured) {
		std::vector<Vertex> vertexs;

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

void Surface::generatePolarParametric(Graphics& gfx, float theta(float, float), float phi(float, float), float rad(float, float), Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV, bool Textured)
{
	float epsilon = error_epsilon;

	if (!Textured) {
		std::vector<Vertex> vertexs;

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
		AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));
	}
	else {
		std::vector<TexVertex> vertexs;

		for (UINT i = 0; i < numU + 1; i++) {
			for (UINT j = 0; j < numV + 1; j++) {
				float u = ((numU - i) * minRect.x + i * maxRect.x) / numU;
				float v = ((numV - j) * minRect.y + j * maxRect.y) / numV;
				vertexs.push_back({ rad(u,v) * Vector3f(cosf(theta(u,v)) * cosf(phi(u,v)), sinf(theta(u,v)) * cosf(phi(u,v)), sinf(phi(u,v))) ,

					-((rad(u + epsilon,v) * Vector3f(cosf(theta(u + epsilon,v)) * cosf(phi(u + epsilon,v)), sinf(theta(u + epsilon,v)) * cosf(phi(u + epsilon,v)), sinf(phi(u + epsilon,v))) -
					rad(u + epsilon,v) * Vector3f(cosf(theta(u - epsilon,v)) * cosf(phi(u - epsilon,v)), sinf(theta(u - epsilon,v)) * cosf(phi(u - epsilon,v)), sinf(phi(u - epsilon,v)))) *
					((rad(u,v + epsilon) * Vector3f(cosf(theta(u,v + epsilon)) * cosf(phi(u,v + epsilon)), sinf(theta(u,v + epsilon)) * cosf(phi(u,v + epsilon)), sinf(phi(u,v + epsilon))) -
					rad(u,v + epsilon) * Vector3f(cosf(theta(u,v - epsilon)) * cosf(phi(u,v - epsilon)), sinf(theta(u,v - epsilon)) * cosf(phi(u,v - epsilon)), sinf(phi(u,v - epsilon))))))
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

void Surface::generateImplicit(Graphics& gfx, float H(float, float, float))
{
	throw std::exception("This surface type hasn't been implemented yet");
}

void Surface::generateImplicitPolar(Graphics& gfx, float H(float, float, float))
{
	throw std::exception("This surface type hasn't been implemented yet");
}

void Surface::addDefaultBinds(Graphics& gfx)
{
	auto pvs = (VertexShader*)AddBind(std::move(std::make_unique<VertexShader>(gfx, SHADERS_DIR + std::wstring(L"SurfaceVS.cso"))));

	AddBind(std::make_unique<PixelShader>(gfx, SHADERS_DIR + std::wstring(L"SurfacePS.cso")));

	std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	AddBind(std::make_unique<InputLayout>(gfx, ied, pvs->GetBytecode()));

	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));


	float unused = 0.f;
	vscBuff = { Matrix::Identity.getMatrix4(), Vector3f().getVector4() };
	pscBuff = { 
		60.f,0.f,unused,unused,1.0f, 0.2f, 0.2f, 1.f , 0.f, 8.f, 8.f,unused,
		60.f,0.f,unused,unused,0.0f, 1.0f, 0.0f, 1.f , 0.f,-8.f, 8.f,unused,
		60.f,0.f,unused,unused,0.5f, 0.0f, 1.0f, 1.f ,-8.f, 0.f,-8.f,unused,
		60.f,0.f,unused,unused,1.0f, 1.0f, 0.0f, 1.f , 8.f, 0.f, 8.f,unused,
	};

	pVSCB = (ConstantBuffer<VSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<VSconstBuffer>>(gfx, vscBuff, VERTEX_CONSTANT_BUFFER_TYPE));
	pPSCB = (ConstantBuffer<PSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<PSconstBuffer>>(gfx, pscBuff, PIXEL_CONSTANT_BUFFER_TYPE));
}

void Surface::addTexturedBinds(Graphics& gfx, std::string texture0, std::string texture1)
{
	textured = true;
	AddBind(std::make_unique<Texture>(gfx, texture0));

	if(texture1.size())
		AddBind(std::make_unique<Texture>(gfx, texture1, 1u));
	else
		AddBind(std::make_unique<Texture>(gfx, texture0, 1u));

	auto pvs = (VertexShader*)AddBind(std::move(std::make_unique<VertexShader>(gfx, SHADERS_DIR + std::wstring(L"TexSurfaceVS.cso"))));

	AddBind(std::make_unique<PixelShader>(gfx, SHADERS_DIR + std::wstring(L"TexSurfacePS.cso")));

	std::vector< D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	AddBind(std::make_unique<InputLayout>(gfx, ied, pvs->GetBytecode()));

	AddBind(std::make_unique<Sampler>(gfx, D3D11_FILTER_MIN_MAG_MIP_LINEAR));

	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	float unused = 0.f;
	vscBuff = { Matrix::Identity.getMatrix4(), Vector3f().getVector4() };
	pscBuff = { 32000.f,5000.f,unused,unused,1.f, 1.f, 1.f, 1.f ,160.f, 0.f, 60.f,unused };

	pVSCB = (ConstantBuffer<VSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<VSconstBuffer>>(gfx, vscBuff, VERTEX_CONSTANT_BUFFER_TYPE));
	pPSCB = (ConstantBuffer<PSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<PSconstBuffer>>(gfx, pscBuff, PIXEL_CONSTANT_BUFFER_TYPE));
}

void Surface::addTexturedBinds(Graphics& gfx, Texture texture0, Texture texture1)
{
	textured = true;
	((Texture*)AddBind(std::make_unique<Texture>(texture0)))->setSlot(0u);

	((Texture*)AddBind(std::make_unique<Texture>(texture1)))->setSlot(1u);

	auto pvs = (VertexShader*)AddBind(std::move(std::make_unique<VertexShader>(gfx, SHADERS_DIR + std::wstring(L"TexSurfaceVS.cso"))));

	AddBind(std::make_unique<PixelShader>(gfx, SHADERS_DIR + std::wstring(L"TexSurfacePS.cso")));

	std::vector< D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	AddBind(std::make_unique<InputLayout>(gfx, ied, pvs->GetBytecode()));

	AddBind(std::make_unique<Sampler>(gfx, D3D11_FILTER_MIN_MAG_MIP_LINEAR));

	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	float unused = 0.f;
	vscBuff = { Matrix::Identity.getMatrix4(), Vector3f().getVector4() };
	pscBuff = { 32000.f,5000.f,unused,unused,1.f, 1.f, 1.f, 1.f ,160.f, 0.f, 60.f,unused };

	pVSCB = (ConstantBuffer<VSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<VSconstBuffer>>(gfx, vscBuff, VERTEX_CONSTANT_BUFFER_TYPE));
	pPSCB = (ConstantBuffer<PSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<PSconstBuffer>>(gfx, pscBuff, PIXEL_CONSTANT_BUFFER_TYPE));
}

Vector3f Surface::evalPolar(float r(float, float), float theta, float phi)
{
	return r(theta, phi) * Vector3f(cosf(theta) * cosf(phi), sinf(theta) * cosf(phi), sinf(phi));
}
