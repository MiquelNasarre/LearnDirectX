#pragma once
#include "Drawable.h"

class FourierSurface : public Drawable
{
public:
	struct Coefficient
	{
		unsigned int L;
		int M;
		float C;
	};

	class FileManager
	{
	public:
		static void** extractFigureFromFile(const char* filename);
		static Coefficient* calculateCoefficients(const char* filename, unsigned int maxL);

	};

	void saveCoefficients(const char* filename = "");

private:
	struct infoVect
	{
		float sinphi;
		float cosphi;
		float sintheta;
		float costheta;

	};

	Coefficient* Coef;
	unsigned int Ncoef;

	static float	Ylm(Vector3f v, unsigned int l, int m);
	float			Ylm(unsigned int i, unsigned int l, int m);
	_float4vector	YlmDif(unsigned int i, unsigned int l, int m);
	Vector3f		Ylmdif(unsigned int i, unsigned int l, int m);

	static Vector3f*		vertexsIcosphere;
	static infoVect*		infoIcosphere;
	static unsigned int		depthIcosphere;
	static unsigned short*	trianglesIcosphere;

	static unsigned int ntriangles;
	static unsigned int nvertexs;

	static unsigned int maxL;
	static float** Constants;
	static float Klm(unsigned int l, int m);

	static void generateConstants();
	static void generateIcosphere();

public:
	FourierSurface() {}
	FourierSurface(Graphics& gfx, Coefficient* coef, unsigned int ncoef);

	void create(Graphics& gfx, Coefficient* coef, unsigned int ncoef);
	void updateShape(Graphics& gfx, Coefficient* coef, unsigned int ncoef);

	void updateLight(Graphics& gfx, UINT id, Vector2f intensity, Color color, Vector3f position);
	void updateLight(Graphics& gfx, UINT id, _float4vector intensity, _float4color color, _float4vector position);
	void clearLights(Graphics& gfx);
	void updateRotation(Graphics& gfx, Vector3f axis, float angle, bool multiplicative = false);
	Quaternion getRotation();

private:

	struct LIGHTSOURCE {
		_float4vector intensity = { 0.f,0.f,0.f,0.f };
		_float4color  color = { 1.f,1.f,1.f,1.f };
		_float4vector position = { 0.f,0.f,0.f,0.f };
	}lightsource[8];

	struct PSconstBuffer {
		LIGHTSOURCE lightsource[8];
	}pscBuff;

	struct VSconstBuffer {
		_float4vector translation = { 0.f, 0.f, 0.f, 0.f };
		Quaternion rotation = 1.f;
	}vscBuff;

	ConstantBuffer<VSconstBuffer>* pVSCB;
	ConstantBuffer<PSconstBuffer>* pPSCB;

	
};