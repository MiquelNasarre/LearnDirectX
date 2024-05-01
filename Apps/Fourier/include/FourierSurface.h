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
	private:
		static void calculateCoefficientAsync(Coefficient* coef, const unsigned int l, const Vector3f* centerTriangles, const float* areaTriangles, const float* distanceTriangles, const unsigned int numT, bool* finished);
	public:
		const static void** extractFigureFromFile(const char* filename);
		static Coefficient* calculateCoefficients(const void** extractedFigure, unsigned int maxL);
		static unsigned int ncoefFromFile(const char* filename);
		static Coefficient* loadCoefficientsFromFile(const char* filename);
		static void saveCoefficients(Coefficient* coef, unsigned int ncoef, const char* filename);
	};

	struct Vertex {
		Vector3f vector;
		Vector3f dYlm;
		Color color;
	};

private:
	class Functions
	{
	public:
		static void				generateHarmonicsAsync();
		static void				generateDataAsync(Vector3f*** dataset, unsigned int l, bool* done);
		static Vector3f***		DatasetYlmi;

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
		static const unsigned int Npoints = 250u;

		struct Vertex {
			_float4vector position;
			_float4color color;
		};

		struct VSconstBuffer {
			_float4vector translation = { 0.f, 0.f, 0.f, 0.f };
			Quaternion rotation = 1.f;
			_float4vector screenDisplacement = { 0.f, 0.f, 0.f, 0.f };
		}vscBuff;

		ConstantBuffer<VSconstBuffer>* pVSCB;

		static void generatePhiCurveAsync(const unsigned int t0, const unsigned int t1, const Coefficient* coef, const unsigned int ncoef, const float phi, const float theta, Vertex* V);
		static void generateThetaCurveAsync(const unsigned int t0, const unsigned int t1, const Coefficient* coef, const unsigned int ncoef, const float phi, const float theta, Vertex* V);

	public:
		Curves() {}
		void create(Graphics& gfx, const Coefficient* coef, const unsigned int ncoef, const float phi, const float theta, std::mutex* mtx);
		void updateShape(Graphics& gfx, const Coefficient* coef, const unsigned int ncoef, const float phi, const float theta);
		void updateRotation(Graphics& gfx, Quaternion rotation, bool multiplicative = false);
		void updateScreenPosition(Graphics& gfx, Vector2f screenDisplacement);
	} curves;

	Coefficient* Coef = NULL;
	unsigned int Ncoef = 0u;
	Vertex* Vertexs = NULL;

	static Vector3f*		vertexsIcosphere;
	static infoVect*		infoIcosphere;
	static unsigned int		depthIcosphere;
	static unsigned short*	trianglesIcosphere;

	static unsigned int ntriangles;
	static unsigned int nvertexs;

	static void generateIcosphere();
	static void calculateVertexsAsync(const unsigned int ti, const unsigned int tf, const Coefficient* coef, const unsigned int ncoef, Vertex* V);

public:
	FourierSurface() {}
	~FourierSurface();
	FourierSurface(Graphics& gfx, const Coefficient* coef, const unsigned int ncoef);

	void create(Graphics& gfx, const Coefficient* coef, const unsigned int ncoef, std::mutex* mtx = NULL);
	void updateShape(Graphics& gfx, const Coefficient* coef, const unsigned int ncoef);

	void saveCoefficients(const char* filename);
	void updateLight(Graphics& gfx, UINT id, Vector2f intensity, Color color, Vector3f position);
	void updateLight(Graphics& gfx, UINT id, _float4vector intensity, _float4color color, _float4vector position);
	void clearLights(Graphics& gfx);
	void updateTexture(Graphics& gfx, Color color, bool def = false, bool random = false);
	void updateRotation(Graphics& gfx, Vector3f axis, float angle, bool multiplicative = false);
	void updateRotation(Graphics& gfx, Quaternion rotation, bool multiplicative = false);
	void updateScreenPosition(Graphics& gfx, Vector2f screenDisplacement);
	void updateCurves(Graphics& gfx, float phi, float theta);

	Quaternion getRotation();
	Vertex* getVertexPtr();

	void DrawCurves(Graphics& gfx);

	static void generateDataSet();
	static unsigned int depthDataset();
	static unsigned int getNvertexs();
	static unsigned short* getTrianglesIcosphere();
	static unsigned int getNtriangles();
private:

	struct PSconstBuffer {
		struct {
			_float4vector intensity = { 0.f,0.f,0.f,0.f };
			_float4color  color = { 1.f,1.f,1.f,1.f };
			_float4vector position = { 0.f,0.f,0.f,0.f };
		}lightsource[8];
	}pscBuff;

	struct VSconstBuffer {
		_float4vector translation = { 0.f, 0.f, 0.f, 0.f };
		Quaternion rotation = 1.f;
		_float4vector screenDisplacement = { 0.f, 0.f, 0.f, 0.f };
	}vscBuff;

	ConstantBuffer<VSconstBuffer>* pVSCB = NULL;
	ConstantBuffer<PSconstBuffer>* pPSCB = NULL;

};

void createShape(const char* filename);
void formatFile(const char* srcFile, const char* newFile);