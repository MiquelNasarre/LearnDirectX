#include "FourierSurface.h"
#include "Bindable/BindableBase.h"
#include "Functions.h"

Vector3f*					FourierSurface::vertexsIcosphere = NULL;
unsigned int				FourierSurface::depthIcosphere = 6u;
unsigned short*				FourierSurface::trianglesIcosphere = NULL;
FourierSurface::infoVect*	FourierSurface::infoIcosphere = NULL;

unsigned int	FourierSurface::ntriangles = 0;
unsigned int	FourierSurface::nvertexs = 0;

unsigned int	FourierSurface::maxL = MAX_L;
float**			FourierSurface::Constants = NULL;

float FourierSurface::Ylm(Vector3f v, unsigned int l, int m)
{
	v.normalize();
	float costheta = v.z;
	float sintheta = sqrtf(1 - v.z * v.z);
	float cosphi = v.x / sintheta;
	float sinphi = v.y / sintheta;

	if (m > 0)
		return Klm(l, m) * Functions::Tchev(m, cosphi) * Functions::Legendre(l, m, costheta);
	else if (m == 0)
		return Klm(l, m) * Functions::Legendre(l, m, costheta);
	else
		return Klm(l, m) * Functions::Uchev(-m - 1, cosphi) * sinphi * Functions::Legendre(l, -m, costheta);
}

float FourierSurface::Ylm(unsigned int i, unsigned int l, int m)
{

	if (m > 0)
		return Klm(l, m) * Functions::Tchev(m, infoIcosphere[i].cosphi) * Functions::Legendre(l, m, vertexsIcosphere[i].z);
	else if (m == 0)
		return Klm(l, m) * Functions::Legendre(l, m, vertexsIcosphere[i].z);
	else
		return Klm(l, m) * Functions::Uchev(-m - 1, infoIcosphere[i].cosphi) * infoIcosphere[i].sinphi * Functions::Legendre(l, -m, vertexsIcosphere[i].z);

}

_float4vector FourierSurface::YlmDif(unsigned int i, unsigned int l, int m)
{
	_float4vector ylm;

	float minus_sint_dty;
	float minus_dpy;

	if (m > 0)
	{
		Vector2f leg = Functions::LegendreDif(l, m, infoIcosphere[i].costheta);
		float cosmp = Functions::Tchev(m, infoIcosphere[i].cosphi);
		float sinmp = Functions::Uchev(m - 1, infoIcosphere[i].cosphi) * infoIcosphere[i].sinphi;
		float K = Klm(l, m);

		ylm.w = K * leg.x * cosmp;

		minus_sint_dty = infoIcosphere[i].sintheta * infoIcosphere[i].sintheta * K * leg.y * cosmp;
		minus_dpy = K * leg.x * m * sinmp;
	}
	if (m == 0)
	{
		Vector2f leg = Functions::LegendreDif(l, m, infoIcosphere[i].costheta);
		float K = Klm(l, m);

		ylm.w = K * leg.x;

		minus_sint_dty = infoIcosphere[i].sintheta * infoIcosphere[i].sintheta * K * leg.y;
		minus_dpy = 0.f;
	}
	if (m < 0)
	{
		Vector2f leg = Functions::LegendreDif(l, -m, infoIcosphere[i].costheta);
		float cosmp = Functions::Tchev(-m, infoIcosphere[i].cosphi);
		float sinmp = Functions::Uchev(-m - 1, infoIcosphere[i].cosphi) * infoIcosphere[i].sinphi;
		float K = Klm(l, -m);

		ylm.w = K * leg.x * sinmp;

		minus_sint_dty = infoIcosphere[i].sintheta * infoIcosphere[i].sintheta * K * leg.y * sinmp;
		minus_dpy = K * leg.x * m * cosmp;
	}

	// Vectors normals

	if (i == 12)
	{
		ylm.x = 0.f;
		ylm.x = 0.f;
		ylm.z = 1.f;
		return ylm;
	}
	if (i == 17)
	{
		ylm.x = 0.f;
		ylm.x = 0.f;
		ylm.z = -1.f;
		return ylm;
	}

	ylm.x = ylm.w * (minus_sint_dty * infoIcosphere[i].costheta * infoIcosphere[i].cosphi - minus_dpy * infoIcosphere[i].sinphi + ylm.w * infoIcosphere[i].sintheta * vertexsIcosphere[i].x);
	ylm.y = ylm.w * (minus_sint_dty * infoIcosphere[i].costheta * infoIcosphere[i].sinphi + minus_dpy * infoIcosphere[i].cosphi + ylm.w * infoIcosphere[i].sintheta * vertexsIcosphere[i].y);
	ylm.z = ylm.w * infoIcosphere[i].sintheta * (-minus_sint_dty + ylm.w * vertexsIcosphere[i].z);

	return ylm;
}

Vector3f FourierSurface::Ylmdif(unsigned int i, unsigned int l, int m)
{
	Vector3f ylm;

	if (m > 0)
	{
		Vector2f leg = Functions::LegendreDif(l, m, infoIcosphere[i].costheta);
		float cosmp = Functions::Tchev(m, infoIcosphere[i].cosphi);
		float sinmp = Functions::Uchev(m - 1, infoIcosphere[i].cosphi) * infoIcosphere[i].sinphi;
		float K = Klm(l, m);

		ylm.x = K * leg.x * cosmp;

		ylm.y = -infoIcosphere[i].sintheta * K * leg.y * cosmp;
		ylm.z = -K * leg.x * m * sinmp;

		return ylm;
	}
	if (m == 0)
	{
		Vector2f leg = Functions::LegendreDif(l, m, infoIcosphere[i].costheta);
		float K = Klm(l, m);

		ylm.x = K * leg.x;

		ylm.y = -infoIcosphere[i].sintheta * K * leg.y;
		ylm.z = 0.f;

		return ylm;
	}
	else
	{
		Vector2f leg = Functions::LegendreDif(l, -m, infoIcosphere[i].costheta);
		float cosmp = Functions::Tchev(-m, infoIcosphere[i].cosphi);
		float sinmp = Functions::Uchev(-m - 1, infoIcosphere[i].cosphi) * infoIcosphere[i].sinphi;
		float K = Klm(l, -m);

		ylm.x = K * leg.x * sinmp;

		ylm.y = -infoIcosphere[i].sintheta * K * leg.y * sinmp;
		ylm.z = -K * leg.x * m * cosmp;

		return ylm;
	}
}

float FourierSurface::Klm(unsigned int l, int m)
{
	if (!Constants)
		generateConstants();
	
	return m>=0 ? Constants[l][m] : Constants[l][-m];
}

void FourierSurface::generateConstants()
{
	Constants = (float**)calloc(maxL + 1, sizeof(void*));
	for (unsigned int l = 0; l <= maxL; l++)
	{
		Constants[l] = (float*)calloc(l + 1, sizeof(float));

		Constants[l][0] = sqrtf(float(2 * l + 1));
		for (unsigned int m = 1; m <= l; m++)
			Constants[l][m] = sqrtf(float(2 * (2 * l + 1))) / Functions::sqDivFactorial(l + m, l - m);
	}
}

void FourierSurface::generateIcosphere()
{
	unsigned int depth = depthIcosphere;

	unsigned int currentdepth = 0;
	unsigned int V = 12u;
	unsigned int C = 20u;
	unsigned int A = 30u;

	struct arista {
		unsigned int v0;
		unsigned int v1;
	};

	struct triangle {
		int a0;
		int a1;
		int a2;
	};

	Vector3f* vertexs = (Vector3f*)calloc(V, sizeof(Vector3f));
	arista* aristas = (arista*)calloc(A, sizeof(arista));
	triangle* triangles = (triangle*)calloc(C,sizeof(triangle));

	float gold = (1.f + sqrtf(5) / 2.f);

	vertexs[ 0] = { 0.f, 1.f, gold };
	vertexs[ 1] = { 0.f, 1.f,-gold };
	vertexs[ 2] = { 0.f,-1.f, gold };
	vertexs[ 3] = { 0.f,-1.f,-gold };
	vertexs[ 4] = { 1.f, gold, 0.f };
	vertexs[ 5] = { 1.f,-gold, 0.f };
	vertexs[ 6] = {-1.f, gold, 0.f };
	vertexs[ 7] = {-1.f,-gold, 0.f };
	vertexs[ 8] = { gold, 0.f, 1.f };
	vertexs[ 9] = {-gold, 0.f, 1.f };
	vertexs[10] = { gold, 0.f,-1.f };
	vertexs[11] = {-gold, 0.f,-1.f };


	aristas[ 0] = { 0, 2 };
	aristas[ 1] = { 0, 4 };
	aristas[ 2] = { 0, 6 };
	aristas[ 3] = { 0, 8 };
	aristas[ 4] = { 0, 9 };
	aristas[ 5] = { 1, 3 };
	aristas[ 6] = { 1, 4 };
	aristas[ 7] = { 1, 6 };
	aristas[ 8] = { 1,10 };
	aristas[ 9] = { 1,11 };
	aristas[10] = { 2, 5 };
	aristas[11] = { 2, 7 };
	aristas[12] = { 2, 8 };
	aristas[13] = { 2, 9 };
	aristas[14] = { 3, 5 };
	aristas[15] = { 3, 7 };
	aristas[16] = { 3,10 };
	aristas[17] = { 3,11 };
	aristas[18] = { 4, 6 };
	aristas[19] = { 4, 8 };
	aristas[20] = { 4,10 };
	aristas[21] = { 5, 7 };
	aristas[22] = { 5, 8 };
	aristas[23] = { 5,10 };
	aristas[24] = { 6, 9 };
	aristas[25] = { 6,11 };
	aristas[26] = { 7, 9 };
	aristas[27] = { 7,11 };
	aristas[28] = { 8,10 };
	aristas[29] = { 9,11 };


	triangles[ 0] = {  3,-19, -2 };
	triangles[ 1] = {  7, 19, -8 };
	triangles[ 2] = { 11, 22,-12 };
	triangles[ 3] = { 16,-22,-15 };
	triangles[ 4] = { 21,-29,-20 };
	triangles[ 5] = { 23, 29,-24 };
	triangles[ 6] = { 25, 30,-26 };
	triangles[ 7] = { 28,-30,-27 };
	triangles[ 8] = {-13, -1,  4 };
	triangles[ 9] = { -5,  1, 14 };
	triangles[10] = { -9,  6, 17 };
	triangles[11] = {-18, -6, 10 };
	triangles[12] = {  2, 20, -4 };
	triangles[13] = {  5,-25, -3 };
	triangles[14] = { -7,  9,-21 };
	triangles[15] = { 26,-10,  8 };
	triangles[16] = { 18,-28,-16 };
	triangles[17] = { 15, 24,-17 };
	triangles[18] = { 27,-14, 12 };
	triangles[19] = {-11, 13,-23 };

	unsigned int pV;
	unsigned int pC;
	unsigned int pA;
	triangle* ptriangles = NULL;
	arista* paristas = NULL;
	Vector3f* pvertexs = NULL;

	while (currentdepth < depth)
	{
		if (ptriangles) free(ptriangles);
		if (paristas)	free(paristas);
		if (pvertexs)	free(pvertexs);

		pV = V;
		pC = C;
		pA = A;
		ptriangles = triangles;
		pvertexs = vertexs;
		paristas = aristas;

		currentdepth++;
		V += A;
		A *= 4;
		C *= 4;

		vertexs =	(Vector3f*)calloc(V, sizeof(Vector3f));
		aristas =	(arista*)calloc(A, sizeof(arista));
		triangles =	(triangle*)calloc(C, sizeof(triangle));

		for (unsigned int i = 0; i < pV; i++)
			vertexs[i] = pvertexs[i];

		for (unsigned int i = 0; i < pA; i++)
		{
			vertexs[pV + i] = (vertexs[paristas[i].v0] + vertexs[paristas[i].v1]) / 2.f;
			aristas[2 * i] = { paristas[i].v0, pV + i };
			aristas[2 * i + 1] = { pV + i, paristas[i].v1 };
		}

		for (unsigned int i = 0; i < pC; i++)
		{
			unsigned int aris0 = abs(ptriangles[i].a0) - 1;
			bool ornt0 = (ptriangles[i].a0 > 0) ? true : false;
			unsigned int aris1 = abs(ptriangles[i].a1) - 1;
			bool ornt1 = (ptriangles[i].a1 > 0) ? true : false;
			unsigned int aris2 = abs(ptriangles[i].a2) - 1;
			bool ornt2 = (ptriangles[i].a2 > 0) ? true : false;

			aristas[2 * pA + 3 * i] = { aristas[2 * aris0].v1, aristas[2 * aris1].v1 };
			aristas[2 * pA + 3 * i + 1] = { aristas[2 * aris1].v1, aristas[2 * aris2].v1 };
			aristas[2 * pA + 3 * i + 2] = { aristas[2 * aris2].v1, aristas[2 * aris0].v1 };

			triangles[4 * i] = { int(2 * pA + 3 * i + 1) , int(2 * pA + 3 * i + 2) , int(2 * pA + 3 * i + 3) };

			if (ornt0 && ornt1)
				triangles[4 * i + 1] = { int(2 * aris0 + 2), int(2 * aris1 + 1), -int(2 * pA + 3 * i + 1) };
			else if (ornt0 && !ornt1)
				triangles[4 * i + 1] = { int(2 * aris0 + 2), -int(2 * aris1 + 2), -int(2 * pA + 3 * i + 1) };
			else if (!ornt0 && ornt1)
				triangles[4 * i + 1] = { -int(2 * aris0 + 1), int(2 * aris1 + 1), -int(2 * pA + 3 * i + 1) };
			else if (!ornt0 && !ornt1)
				triangles[4 * i + 1] = { -int(2 * aris0 + 1), -int(2 * aris1 + 2), -int(2 * pA + 3 * i + 1) };

			if (ornt1 && ornt2)
				triangles[4 * i + 2] = { int(2 * aris1 + 2), int(2 * aris2 + 1), -int(2 * pA + 3 * i + 2) };
			else if (ornt1 && !ornt2)
				triangles[4 * i + 2] = { int(2 * aris1 + 2), -int(2 * aris2 + 2), -int(2 * pA + 3 * i + 2) };
			else if (!ornt1 && ornt2)
				triangles[4 * i + 2] = { -int(2 * aris1 + 1), int(2 * aris2 + 1), -int(2 * pA + 3 * i + 2) };
			else if (!ornt1 && !ornt2)
				triangles[4 * i + 2] = { -int(2 * aris1 + 1), -int(2 * aris2 + 2), -int(2 * pA + 3 * i + 2) };

			if (ornt2 && ornt0)
				triangles[4 * i + 3] = { int(2 * aris2 + 2), int(2 * aris0 + 1), -int(2 * pA + 3 * i + 3) };
			else if (ornt2 && !ornt0)
				triangles[4 * i + 3] = { int(2 * aris2 + 2), -int(2 * aris0 + 2), -int(2 * pA + 3 * i + 3) };
			else if (!ornt2 && ornt0)
				triangles[4 * i + 3] = { -int(2 * aris2 + 1), int(2 * aris0 + 1), -int(2 * pA + 3 * i + 3) };
			else if (!ornt2 && !ornt0)
				triangles[4 * i + 3] = { -int(2 * aris2 + 1), -int(2 * aris0 + 2), -int(2 * pA + 3 * i + 3) };

		}
	}

	if (ptriangles) free(ptriangles);
	if (paristas)	free(paristas);
	if (pvertexs)	free(pvertexs);

	infoIcosphere = (infoVect*)calloc(V, sizeof(infoVect));

	trianglesIcosphere = (unsigned short*)calloc(3 * C, sizeof(unsigned short));
	for (unsigned int i = 0; i < C; i++)
	{
		unsigned int aris0 = abs(triangles[i].a0) - 1;
		bool ornt0 = (triangles[i].a0 > 0) ? true : false;
		unsigned int aris1 = abs(triangles[i].a1) - 1;
		bool ornt1 = (triangles[i].a1 > 0) ? true : false;
		unsigned int aris2 = abs(triangles[i].a2) - 1;
		bool ornt2 = (triangles[i].a2 > 0) ? true : false;
		
		trianglesIcosphere[3 * i]		= ornt0 ? aristas[aris0].v0 : aristas[aris0].v1;
		trianglesIcosphere[3 * i + 1]	= ornt1 ? aristas[aris1].v0 : aristas[aris1].v1;
		trianglesIcosphere[3 * i + 2]	= ornt2 ? aristas[aris2].v0 : aristas[aris2].v1;

	}
	vertexsIcosphere = vertexs;

	nvertexs = V;
	ntriangles = C;

	for (unsigned int i = 0; i < nvertexs; i++)
	{
		vertexsIcosphere[i].normalize();
		infoIcosphere[i].costheta = vertexsIcosphere[i].z;
		infoIcosphere[i].sintheta = sqrtf(1 - vertexsIcosphere[i].z * vertexsIcosphere[i].z);
		infoIcosphere[i].cosphi = vertexsIcosphere[i].x / infoIcosphere[i].sintheta;
		infoIcosphere[i].sinphi = vertexsIcosphere[i].y / infoIcosphere[i].sintheta;
	}

	free(aristas);
	free(triangles);
}

FourierSurface::FourierSurface(Graphics& gfx, Coefficient* coef, unsigned int ncoef)
{
	create(gfx, coef, ncoef);
}

void FourierSurface::create(Graphics& gfx, Coefficient* coef, unsigned int ncoef)
{

	if (isInit)
		throw std::exception("You cannot create a surface over one that is already initialized");
	else
		isInit = true;

	Coef = coef;
	Ncoef = ncoef;

	if (!vertexsIcosphere)
		generateIcosphere();

	struct Vertex {
		Vector3f vector;
		Vector3f norm;
		Color color;
	};

	Vertex* V = (Vertex*)calloc(nvertexs, sizeof(Vertex));
	for (unsigned int i = 0; i < nvertexs; i++)
	{
		if (ncoef == 1)
		{
			_float4vector ylm = YlmDif(i, coef[0].L, coef[0].M);
			if (ylm.w > 0)
			{
				V[i].vector = ylm.w * vertexsIcosphere[i];
				V[i].color = Color::Blue;
				V[i].norm = Vector3f(ylm.x, ylm.y, ylm.z).normalize();
			}
			else
			{
				V[i].vector = -ylm.w * vertexsIcosphere[i];
				V[i].color = Color::Yellow;
				V[i].norm = Vector3f(ylm.x, ylm.y, ylm.z).normalize();
			}
		}
		else
		{
			Vector3f ylm(0.f, 0.f, 0.f);
			for (unsigned int j = 0; j < ncoef; j++)
				ylm += coef[j].C * Ylmdif(i, coef[j].L, coef[j].M);

			V[i].vector = ylm.x * vertexsIcosphere[i];

			V[i].norm = (ylm.x * Vector3f(
				-infoIcosphere[i].sintheta * infoIcosphere[i].costheta * infoIcosphere[i].cosphi * ylm.y + infoIcosphere[i].sinphi * ylm.z + infoIcosphere[i].sintheta * infoIcosphere[i].sintheta * infoIcosphere[i].cosphi * ylm.x,
				-infoIcosphere[i].sintheta * infoIcosphere[i].costheta * infoIcosphere[i].sinphi * ylm.y - infoIcosphere[i].cosphi * ylm.z + infoIcosphere[i].sintheta * infoIcosphere[i].sintheta * infoIcosphere[i].sinphi * ylm.x,
				infoIcosphere[i].sintheta * (infoIcosphere[i].sintheta * ylm.y + infoIcosphere[i].costheta * ylm.x)
			)).normalize();



			V[i].color = Color(rand() % 256, rand() % 256, rand() % 256);
		}
	}

	AddBind(std::make_unique<VertexBuffer>(gfx, V, nvertexs));
	free(V);

	AddBind(std::make_unique<IndexBuffer>(gfx, trianglesIcosphere, 3 * ntriangles));

	auto pvs = (VertexShader*)AddBind(std::move(std::make_unique<VertexShader>(gfx, SHADERS_DIR + std::wstring(L"VertexShader.cso"))));

	AddBind(std::make_unique<PixelShader>(gfx, SHADERS_DIR + std::wstring(L"PixelShader.cso")));

	std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Color",0,DXGI_FORMAT_B8G8R8A8_UNORM,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	AddBind(std::make_unique<InputLayout>(gfx, ied, pvs->GetBytecode()));

	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_unique<Rasterizer>(gfx, true));

	pVSCB = (ConstantBuffer<VSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<VSconstBuffer>>(gfx, VERTEX_CONSTANT_BUFFER_TYPE));

	pPSCB = (ConstantBuffer<PSconstBuffer>*)AddBind(std::make_unique<ConstantBuffer<PSconstBuffer>>(gfx, pscBuff, PIXEL_CONSTANT_BUFFER_TYPE));
}

void FourierSurface::updateShape(Graphics& gfx, Coefficient* coef, unsigned int ncoef)
{
	if (!isInit)
		throw std::exception("You cannot update shape to an uninitialized surface");

	Coef = coef;
	Ncoef = ncoef;

	if (!vertexsIcosphere)
		generateIcosphere();

	struct Vertex {
		Vector3f vector;
		Vector3f norm;
		Color color;
	};

	Vertex* V = (Vertex*)calloc(nvertexs, sizeof(Vertex));
	for (unsigned int i = 0; i < nvertexs; i++)
	{
		if (ncoef == 1)
		{
			_float4vector ylm = YlmDif(i, coef[0].L, coef[0].M);
			if (ylm.w > 0)
			{
				V[i].vector = ylm.w * vertexsIcosphere[i];
				V[i].color = Color::Blue;
				V[i].norm = Vector3f(ylm.x, ylm.y, ylm.z).normalize();
			}
			else
			{
				V[i].vector = -ylm.w * vertexsIcosphere[i];
				V[i].color = Color::Yellow;
				V[i].norm = Vector3f(ylm.x, ylm.y, ylm.z).normalize();
			}
		}
		else
		{
			Vector3f ylm(0.f, 0.f, 0.f);
			for (unsigned int j = 0; j < ncoef; j++)
				ylm += coef[j].C * Ylmdif(i, coef[j].L, coef[j].M);

			V[i].vector = ylm.x * vertexsIcosphere[i];

			V[i].norm = (ylm.x * Vector3f(
				-infoIcosphere[i].sintheta * infoIcosphere[i].costheta * infoIcosphere[i].cosphi * ylm.y + infoIcosphere[i].sinphi * ylm.z + infoIcosphere[i].sintheta * infoIcosphere[i].sintheta * infoIcosphere[i].cosphi * ylm.x,
				-infoIcosphere[i].sintheta * infoIcosphere[i].costheta * infoIcosphere[i].sinphi * ylm.y - infoIcosphere[i].cosphi * ylm.z + infoIcosphere[i].sintheta * infoIcosphere[i].sintheta * infoIcosphere[i].sinphi * ylm.x,
				infoIcosphere[i].sintheta * (infoIcosphere[i].sintheta * ylm.y + infoIcosphere[i].costheta * ylm.x)
			)).normalize();



			V[i].color = Color(rand() % 256, rand() % 256, rand() % 256);
		}
	}

	changeBind(std::make_unique<VertexBuffer>(gfx, V, nvertexs), 0u);

	free(V);
}

void FourierSurface::updateLight(Graphics& gfx, UINT id, Vector2f intensity, Color color, Vector3f position)
{
	pscBuff.lightsource[id] = { intensity.getVector4() , color.getColor4() , position.getVector4() };
	pPSCB->Update(gfx, pscBuff);
}

void FourierSurface::updateLight(Graphics& gfx, UINT id, _float4vector intensity, _float4color color, _float4vector position)
{
	pscBuff.lightsource[id] = { intensity , color , position };
	pPSCB->Update(gfx, pscBuff);
}

void FourierSurface::clearLights(Graphics& gfx)
{
	pscBuff = {};
	pPSCB->Update(gfx, pscBuff);
}

void FourierSurface::updateRotation(Graphics& gfx, Vector3f axis, float angle, bool multiplicative)
{
	if (!multiplicative)
		vscBuff.rotation = rotationQuaternion(axis, angle);
	else
		vscBuff.rotation *= rotationQuaternion(axis, angle);

	vscBuff.rotation.normalize();
	pVSCB->Update(gfx, vscBuff);
}

Quaternion FourierSurface::getRotation()
{
	return vscBuff.rotation;
}

// File manager functions

void** FourierSurface::FileManager::extractFigureFromFile(const char* filename)
{
	FILE* file = fopen((FIGURES_DIR + std::string(filename) + ".txt").c_str(), "r");
	if (!file)
		throw std::exception(("Unable to find or open file: " + std::string(filename)).c_str());

	fscanf(file, "Vertexs:\n");
	char s[100];
	int nV = 0;
	while (fgets(s, 100, file)[0] != 'T')nV++;

	rewind(file);
	fscanf(file, "Vertexs:\n");

	Vector3f* vertexs = (Vector3f*)calloc(nV, sizeof(Vector3f));
	for (int i = 0; i < nV; i++)
		fscanf(file, "(%f,%f,%f)\n", &vertexs[i].x, &vertexs[i].y, &vertexs[i].z);

	fscanf(file, "Triangles:\n");

	int nT = 0;
	while (fscanf(file, "%s\n", s) != EOF)nT++;

	rewind(file);
	fscanf(file, "Vertexs:\n");
	while (fgets(s, 100, file)[0] != 'T');

	Vector3i* triangles = (Vector3i*)calloc(nT, sizeof(Vector3i));

	for (int i = 0; i < nT; i++)
		fscanf(file, "%i,%i,%i", &triangles[i].x, &triangles[i].y, &triangles[i].z);

	fclose(file);

	void** figure = (void**)calloc(3, sizeof(void*));
	int* nt = (int*)calloc(2, sizeof(int));
	nt[0] = nT;
	nt[1] = nV;
	figure[0] = vertexs;
	figure[1] = triangles;
	figure[2] = nt;

	return figure;
}

FourierSurface::Coefficient* FourierSurface::FileManager::calculateCoefficients(const char* filename, unsigned int maxL)
{
	void** figure = extractFigureFromFile(filename);

	Vector3f* vertexs = (Vector3f*)figure[0];
	Vector3i* triangles = (Vector3i*)figure[1];
	int numT = ((int*)figure[2])[0];
	int numV = ((int*)figure[2])[1];
	free(figure[2]);
	free(figure);

	// Now we have the figure
	// Lets calculate the coefficients

	Vector3f* centerTriangles = (Vector3f*)calloc(numT, sizeof(Vector3f));
	float* areaTriangles = (float*)calloc(numT, sizeof(float));
	float* distanceTriangles = (float*)calloc(numT, sizeof(float));

	for (int i = 0; i < numT; i++)
	{
		Vector3f V1 = vertexs[triangles[i].x];
		Vector3f V2 = vertexs[triangles[i].y];
		Vector3f V3 = vertexs[triangles[i].z];

		Vector3f V12 = (V1 * V2 * V1).normalize();
		Vector3f V13 = (V1 * V3 * V1).normalize();

		float angle1 = acosf(V13 ^ V12);

		Vector3f V21 = (V2 * V1 * V2).normalize();
		Vector3f V23 = (V2 * V3 * V2).normalize();

		float angle2 = acosf(V23 ^ V21);

		Vector3f V32 = (V3 * V2 * V3).normalize();
		Vector3f V31 = (V3 * V1 * V3).normalize();

		float angle3 = acosf(V31 ^ V32);

		centerTriangles[i] = (V1 + V2 + V3) / 3.f;
		areaTriangles[i] = angle1 + angle2 + angle3 - pi;
		distanceTriangles[i] = centerTriangles[i].abs();
		centerTriangles[i].normalize();
	}

	Coefficient* Coef = (Coefficient*)calloc((maxL + 1) * (maxL + 1), sizeof(Coefficient));
	for (unsigned int l = 0; l <= maxL; l++)
	{
		for (int m = -int(l); m <= int(l); m++)
		{
			Coef[l * l + m + l].L = l;
			Coef[l * l + m + l].M = m;
			Coef[l * l + m + l].C = 0.f;

			for (int i = 0; i < numT; i++)
			{
				Coef[l * l + m + l].C += areaTriangles[i] * distanceTriangles[i] * Ylm(centerTriangles[i], l, m);
			}
			Coef[l * l + m + l].C *= 0.25f / pi;
		}
	}

	free(vertexs);
	free(triangles);
	return Coef;

}

void FourierSurface::saveCoefficients(const char* filename)
{
	int n = 21;
	Vector3f* vertexs = (Vector3f*)calloc(n * n * 6, sizeof(Vector3f));

	for (int m = 0; m < 6; m++)
	{
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				float x = -1.f + 2.f * i / (n - 1.f);
				float y = -1.f + 2.f * j / (n - 1.f);
				switch (m)
				{
				case 0:
					vertexs[n * n * m + n * i + j] = Vector3f(1.f, x, y);
					break;
				case 1:
					vertexs[n * n * m + n * i + j] = Vector3f(-1.f, x, y);
					break;
				case 2:
					vertexs[n * n * m + n * i + j] = Vector3f(x, 1.f, y);
					break;
				case 3:
					vertexs[n * n * m + n * i + j] = Vector3f(x, -1.f, y);
					break;
				case 4:
					vertexs[n * n * m + n * i + j] = Vector3f(x, y, 1.f);
					break;
				case 5:
					vertexs[n * n * m + n * i + j] = Vector3f(x, y, -1.f);
					break;
				default:
					break;
				}
			}
		}
	}

	Vector3i* triangles = (Vector3i*)calloc((n - 1) * (n - 1) * 12, sizeof(Vector3i));

	for (int m = 0; m < 6; m++)
	{
		for (int i = 0; i < n-1; i++)
		{
			for (int j = 0; j < n-1; j++)
			{
				triangles[2 * m * (n - 1) * (n - 1) + 2 * i * (n - 1) + 2 * j] = Vector3i(n * n * m + n * i + j, n * n * m + n * (i + 1) + j, n * n * m + n * i + (j + 1));
				triangles[2 * m * (n - 1) * (n - 1) + 2 * i * (n - 1) + 2 * j + 1] = Vector3i(n * n * m + n * (i + 1) + (j + 1), n * n * m + n * (i + 1) + j, n * n * m + n * i + (j + 1));
			}
		}
	}

	FILE* file = fopen(FIGURES_DIR "Cube.txt", "w");
	if (!file)
		throw std::exception("Couldn't create the file");

	fprintf(file, "Vertexs:\n");

	for (int i = 0; i < n * n * 6; i++)
		fprintf(file, "(%f,%f,%f)\n", vertexs[i].x, vertexs[i].y, vertexs[i].z);

	fprintf(file, "Triangles:");
	for (int i = 0; i < (n - 1) * (n - 1) * 12; i++)
		fprintf(file, "\n%i,%i,%i", triangles[i].x, triangles[i].y, triangles[i].z);

	fclose(file);
}
