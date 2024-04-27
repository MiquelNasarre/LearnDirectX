#pragma once
#include "Drawable.h"
#include <mutex>

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
		static void saveCoefficients(Coefficient* coef, unsigned int ncoef, const char* filename);
	};

private:
	class Functions
	{
	public:
		static float			Ylm(int l, int m, float phi, float theta);
		static float			Ylm(Vector3f v, unsigned int l, int m);
		static float			Ylm(unsigned int i, unsigned int l, int m);
		static _float4vector	YlmDif(unsigned int i, unsigned int l, int m);
		static Vector3f			Ylmdif(unsigned int i, unsigned int l, int m);

		static float	Legendre(int l, int m, float x);
		static Vector2f LegendreDif(int l, int m, float x);

		static float Tchev(int n, float x);
		static float Uchev(int n, float x);

		static int Factorial(int n);
		static float DFactorial(int n);
		static float DivFactorial(int n, int m);
		static float sqDivFactorial(int n, int m);

		static unsigned int maxL;
		static float** Constants;
		static float Klm(unsigned int l, int m);
		static void generateConstants();
	};

	struct infoVect
	{
		float sinphi;
		float cosphi;
		float sintheta;
		float costheta;

	};

	class Curves : public Drawable
	{
		const unsigned int Npoints = 250u;

		struct Vertex {
			_float4vector position;
			_float4color color;
		};

		struct VSconstBuffer {
			_float4vector translation = { 0.f, 0.f, 0.f, 0.f };
			Quaternion rotation = 1.f;
		}vscBuff;

		ConstantBuffer<VSconstBuffer>* pVSCB;

	public:
		Curves() {}
		void create(Graphics& gfx, Coefficient* coef, const unsigned int ncoef, const float phi, const float theta, std::mutex* mtx);
		void updateShape(Graphics& gfx, Coefficient* coef, const unsigned int ncoef, const float phi, const float theta);
		void updateRotation(Graphics& gfx, Quaternion rotation, bool multiplicative = false);
	} curves;

	Coefficient* Coef;
	unsigned int Ncoef;

	static Vector3f*		vertexsIcosphere;
	static infoVect*		infoIcosphere;
	static unsigned int		depthIcosphere;
	static unsigned short*	trianglesIcosphere;

	static unsigned int ntriangles;
	static unsigned int nvertexs;

	static void generateIcosphere();

public:
	FourierSurface() {}
	FourierSurface(Graphics& gfx, Coefficient* coef, unsigned int ncoef);

	void create(Graphics& gfx, Coefficient* coef, unsigned int ncoef, std::mutex* mtx = NULL);
	void updateShape(Graphics& gfx, Coefficient* coef, unsigned int ncoef);

	void saveCoefficients(const char* filename);
	void updateLight(Graphics& gfx, UINT id, Vector2f intensity, Color color, Vector3f position);
	void updateLight(Graphics& gfx, UINT id, _float4vector intensity, _float4color color, _float4vector position);
	void clearLights(Graphics& gfx);
	void updateRotation(Graphics& gfx, Vector3f axis, float angle, bool multiplicative = false);
	void updateRotation(Graphics& gfx, Quaternion rotation, bool multiplicative = false);
	void updateCurves(Graphics& gfx, float phi, float theta);
	Quaternion getRotation();

	void DrawCurves(Graphics& gfx);

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

void createShape(const char* filename);