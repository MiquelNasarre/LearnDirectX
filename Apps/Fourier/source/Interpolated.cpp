#include "Interpolated.h"

Interpolated::Interpolated(Graphics& gfx, FourierSurface* surface0, FourierSurface* surface1, float t)
{
	create(gfx, surface0, surface1, t);
}

void Interpolated::create(Graphics& gfx, FourierSurface* surface0, FourierSurface* surface1, float t)
{
	if (isInit)
		throw std::exception("You cannot create a surface over one that is already initialized");
	else
		isInit = true;

	Surface0 = surface0;
	Surface1 = surface1;
	while (t > 1)t--;
	while (t < 0)t++;
	vscBuff.tvalue.x = t;

	FourierSurface::Vertex* V0 = surface0->getVertexPtr();
	FourierSurface::Vertex* V1 = surface1->getVertexPtr();
	unsigned int N = FourierSurface::getNvertexs();

	InterpolatedVertex* vertexs = (InterpolatedVertex*)calloc(N, sizeof(InterpolatedVertex));

	for (unsigned int i = 0; i < N; i++)
	{
		vertexs[i].vector = V0[i].vector;
		vertexs[i].dYlm0 = V0[i].dYlm;
		vertexs[i].dYlm1 = V1[i].dYlm;
		vertexs[i].color = Color::White;
	}

	AddBind(std::make_unique<VertexBuffer>(gfx, vertexs, N));

	AddBind(std::make_unique<IndexBuffer>(gfx, FourierSurface::getTrianglesIcosphere(), 3 * FourierSurface::getNtriangles()));

	auto pvs = (VertexShader*)AddBind(std::move(std::make_unique<VertexShader>(gfx, SHADERS_DIR + std::wstring(L"InterpolatedVS.cso"))));

	AddBind(std::make_unique<PixelShader>(gfx, SHADERS_DIR + std::wstring(L"InterpolatedPS.cso")));

	std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Dylm0a",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Dylm1a",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Color",0,DXGI_FORMAT_B8G8R8A8_UNORM,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	AddBind(std::make_unique<InputLayout>(gfx, ied, pvs->GetBytecode()));

	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_unique<Rasterizer>(gfx, true));

	pVSCB = (ConstantBuffer<VSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<VSconstBuffer>>(gfx, VERTEX_CONSTANT_BUFFER_TYPE));

	pscBuff.lightsource[0] = { { 600.f, 320.f, 0.f, 0.f } , { 1.f, 1.f, 1.f, 1.f } , { 30.f, 10.f, 20.f, 0.f } };

	pPSCB = (ConstantBuffer<PSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<PSconstBuffer>>(gfx, pscBuff, PIXEL_CONSTANT_BUFFER_TYPE));

	free(vertexs);
}

void Interpolated::updateShape(Graphics& gfx, FourierSurface* surface0, FourierSurface* surface1, float t)
{
	if (!isInit)
		throw std::exception("You cannot update shape to an uninitialized surface");

	Surface0 = surface0;
	Surface1 = surface1;
	while (t > 1)t--;
	while (t < 0)t++;
	vscBuff.tvalue.x = t;

	FourierSurface::Vertex* V0 = surface0->getVertexPtr();
	FourierSurface::Vertex* V1 = surface1->getVertexPtr();
	unsigned int N = FourierSurface::getNvertexs();

	InterpolatedVertex* vertexs = (InterpolatedVertex*)calloc(N, sizeof(InterpolatedVertex));

	for (unsigned int i = 0; i < N; i++)
	{
		vertexs[i].vector = V0[i].vector;
		vertexs[i].dYlm0 = V0[i].dYlm;
		vertexs[i].dYlm1 = V1[i].dYlm;
		vertexs[i].color = Color::White;
	}

	changeBind(std::make_unique<VertexBuffer>(gfx, vertexs, N), 0u);

	pVSCB->Update(gfx, vscBuff);
	
	free(vertexs);
}

void Interpolated::updateInterpolation(Graphics& gfx, float t)
{
	if (!isInit)
		throw std::exception("You cannot update screen position to an uninitialized surface");

	while (t > 1)t--;
	while (t < 0)t++;
	vscBuff.tvalue.x = t;

	pVSCB->Update(gfx, vscBuff);
}

void Interpolated::saveCoefficients(const char* filename)
{
}

void Interpolated::saveInterpolation(const char* filename)
{
}

void Interpolated::updateLight(Graphics& gfx, UINT id, Vector2f intensity, Color color, Vector3f position)
{
	if (!isInit)
		throw std::exception("You cannot update light to an uninitialized surface");

	pscBuff.lightsource[id] = { intensity.getVector4() , color.getColor4() , position.getVector4() };
	pPSCB->Update(gfx, pscBuff);
}

void Interpolated::updateLight(Graphics& gfx, UINT id, _float4vector intensity, _float4color color, _float4vector position)
{
	if (!isInit)
		throw std::exception("You cannot update light to an uninitialized surface");

	pscBuff.lightsource[id] = { intensity , color , position };
	pPSCB->Update(gfx, pscBuff);
}

void Interpolated::clearLights(Graphics& gfx)
{
	if (!isInit)
		throw std::exception("You cannot update clear lights to an uninitialized surface");

	pscBuff = {};
	pPSCB->Update(gfx, pscBuff);
}

void Interpolated::updateTexture(Graphics& gfx, Color color, bool def, bool random)
{
}

void Interpolated::updateRotation(Graphics& gfx, Vector3f axis, float angle, bool multiplicative)
{
	if (!isInit)
		throw std::exception("You cannot update rotation to an uninitialized surface");

	if (!multiplicative)
		vscBuff.rotation = rotationQuaternion(axis, angle);
	else
		vscBuff.rotation *= rotationQuaternion(axis, angle);

	vscBuff.rotation.normalize();
	pVSCB->Update(gfx, vscBuff);
}

void Interpolated::updateRotation(Graphics& gfx, Quaternion rotation, bool multiplicative)
{
	if (!isInit)
		throw std::exception("You cannot update rotation to an uninitialized surface");

	if (!multiplicative)
		vscBuff.rotation = rotation;
	else
		vscBuff.rotation *= rotation;

	vscBuff.rotation.normalize();
	pVSCB->Update(gfx, vscBuff);
}

void Interpolated::updateScreenPosition(Graphics& gfx, Vector2f screenDisplacement)
{
	if (!isInit)
		throw std::exception("You cannot update screen position to an uninitialized surface");

	vscBuff.screenDisplacement = screenDisplacement.getVector4();

	pVSCB->Update(gfx, vscBuff);
}

void Interpolated::updateCurves(Graphics& gfx, float phi, float theta)
{
}

Quaternion Interpolated::getRotation()
{
	return vscBuff.rotation;
}

void Interpolated::DrawCurves(Graphics& gfx)
{
}

//	Interpolated String Functions

InterpolatedString::~InterpolatedString()
{
	if (!Nsurfaces)
		return;
	for (unsigned int i = 0; i < Nsurfaces - 1; i++)
		Interpolations[i]->~Interpolated();
	if (Interpolations)
		free(Interpolations);
	if (Surfaces)
		free(Surfaces);
}

InterpolatedString::InterpolatedString(Graphics& gfx, FourierSurface** surfaces, unsigned int nsurfaces, float t)
{
	create(gfx, surfaces, nsurfaces, t);
}

void InterpolatedString::create(Graphics& gfx, FourierSurface** surfaces, unsigned int nsurfaces, float t)
{
	if (Surfaces)
		free(Surfaces);
	if (Interpolations)
		free(Interpolations);
	Nsurfaces = nsurfaces;
	Surfaces = NULL;
	Interpolations = NULL;
	T = 0.f;


	if (!nsurfaces) return;

	Surfaces = (FourierSurface**)calloc(nsurfaces, sizeof(void*));
	for (unsigned int i = 0; i < nsurfaces; i++)
		Surfaces[i] = surfaces[i];

	if (nsurfaces == 1u) return;

	while (t > float(nsurfaces) - 1.f) t -= nsurfaces - 1;
	while (t < 0.f) t += nsurfaces - 1;
	T = t;

	Interpolations = (Interpolated**)calloc(nsurfaces - 1u, sizeof(void*));
	for (unsigned int i = 0; i < nsurfaces - 1u; i++)
	{
		Interpolations[i] = new(Interpolated);
		Interpolations[i]->create(gfx, surfaces[i], surfaces[i + 1]);
		if (t >= i && t < i + 1)
			Interpolations[i]->updateInterpolation(gfx, t - i);
	}
}

void InterpolatedString::addSurface(Graphics& gfx, FourierSurface* surface)
{
	Nsurfaces++;
	FourierSurface** tSurfaces = (FourierSurface**)calloc(Nsurfaces, sizeof(void*));
	for (unsigned int i = 0; i < Nsurfaces - 1; i++)
		tSurfaces[i] = Surfaces[i];
	if (Surfaces)
		free(Surfaces);
	Surfaces = tSurfaces;
	Surfaces[Nsurfaces - 1] = surface;

	if (Nsurfaces == 1u)
		return;

	Interpolated** tInterpolations = (Interpolated**)calloc(Nsurfaces - 1, sizeof(void*));
	for (unsigned int i = 0; i < Nsurfaces - 2; i++)
		tInterpolations[i] = Interpolations[i];
	if (Interpolations)
		free(Interpolations);
	Interpolations = tInterpolations;
	Interpolations[Nsurfaces - 2] = new(Interpolated);
	Interpolations[Nsurfaces - 2]->create(gfx, Surfaces[Nsurfaces - 2], surface);
}

void InterpolatedString::deleteSurface(Graphics& gfx, unsigned int s)
{
	if (Nsurfaces == 1u)
	{
		free(Surfaces);
		Surfaces = NULL;
		Nsurfaces = 0u;
		return;
	}

	if (s != Nsurfaces - 1 && Nsurfaces > 1)
		Interpolations[s]->~Interpolated();

	for (unsigned int i = s; i < Nsurfaces; i++)
	{
		Surfaces[i] = Surfaces[i + 1];
		if (i < Nsurfaces - 1)
			Interpolations[i] = Interpolations[i + 1];
	}

	if (s != 0 && s != Nsurfaces - 1)
	{
		Interpolations[s-1]->~Interpolated();
		Interpolations[s-1] = new(Interpolated);
		Interpolations[s - 1]->create(gfx, Surfaces[s - 1], Surfaces[s]);
	}

	if (--Nsurfaces == 1u)
		Interpolations = NULL;
}

void InterpolatedString::updateInterpolation(Graphics& gfx, float t)
{
	if (!Nsurfaces || Nsurfaces == 1u)
		return;

	while (t > float(Nsurfaces) - 1.f) t -= Nsurfaces - 1;
	while (t < 0.f) t += Nsurfaces - 1;
	T = t;

	for (unsigned int i = 0; i < Nsurfaces - 1u; i++)
	{
		if (t >= i && t <= i + 1)
			Interpolations[i]->updateInterpolation(gfx, t - i);
	}
}

void InterpolatedString::saveCoefficients(const char* filename)
{
}

void InterpolatedString::saveInterpolation(const char* filename)
{
}

void InterpolatedString::updateLight(Graphics& gfx, UINT id, Vector2f intensity, Color color, Vector3f position)
{
	if (!Nsurfaces)return;

	for (unsigned int i = 0; i < Nsurfaces - 1; i++)
		Interpolations[i]->updateLight(gfx, id, intensity, color, position);
}

void InterpolatedString::updateLight(Graphics& gfx, UINT id, _float4vector intensity, _float4color color, _float4vector position)
{
	if (!Nsurfaces)return;

	for (unsigned int i = 0; i < Nsurfaces - 1; i++)
		Interpolations[i]->updateLight(gfx, id, intensity, color, position);
}

void InterpolatedString::clearLights(Graphics& gfx)
{
	if (!Nsurfaces)return;

	for (unsigned int i = 0; i < Nsurfaces - 1; i++)
		Interpolations[i]->clearLights(gfx);
}

void InterpolatedString::updateTexture(Graphics& gfx, Color color, bool def, bool random)
{
	if (!Nsurfaces)return;

	for (unsigned int i = 0; i < Nsurfaces - 1; i++)
		Interpolations[i]->updateTexture(gfx, color, def, random);
}

void InterpolatedString::updateRotation(Graphics& gfx, Vector3f axis, float angle, bool multiplicative)
{
	if (!Nsurfaces)return;

	if (Nsurfaces == 1)
		Surfaces[0]->updateRotation(gfx, axis, angle, multiplicative);

	for (unsigned int i = 0; i < Nsurfaces - 1; i++)
		Interpolations[i]->updateRotation(gfx, axis, angle, multiplicative);
}

void InterpolatedString::updateRotation(Graphics& gfx, Quaternion rotation, bool multiplicative)
{
	if (!Nsurfaces)return;

	if (Nsurfaces == 1)
		Surfaces[0]->updateRotation(gfx, rotation, multiplicative);

	for (unsigned int i = 0; i < Nsurfaces - 1; i++)
		Interpolations[i]->updateRotation(gfx, rotation, multiplicative);
}

void InterpolatedString::updateScreenPosition(Graphics& gfx, Vector2f screenDisplacement)
{
	if (!Nsurfaces)return;

	if (Nsurfaces == 1)
		Surfaces[0]->updateScreenPosition(gfx, screenDisplacement);

	for (unsigned int i = 0; i < Nsurfaces - 1; i++)
		Interpolations[i]->updateScreenPosition(gfx, screenDisplacement);
}

void InterpolatedString::updateCurves(Graphics& gfx, float phi, float theta)
{
	if (!Nsurfaces)return;

	for (unsigned int i = 0; i < Nsurfaces - 1; i++)
		Interpolations[i]->updateCurves(gfx, phi, theta);
}

Quaternion InterpolatedString::getRotation()
{
	if (Nsurfaces)
		return Surfaces[0]->getRotation();
	else
		return 0.f;
}

void InterpolatedString::Draw(Graphics& gfx)
{
	if (!Nsurfaces)
		return;
	if (Nsurfaces == 1)
		return Surfaces[0]->Draw(gfx);

	for (unsigned int i = 0; i < Nsurfaces - 1u; i++)
	{
		if (T >= i && T <= i + 1)
			return Interpolations[i]->Draw(gfx);
	}
}

void InterpolatedString::DrawCurves(Graphics& gfx)
{
	for (unsigned int i = 0; i < Nsurfaces - 1u; i++)
	{
		if (T >= i && T <= i + 1)
			return Interpolations[i]->DrawCurves(gfx);
	}
}
