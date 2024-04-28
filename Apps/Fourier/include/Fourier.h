#pragma once
#include "Window.h"
#include "Drawable/Curve.h"
#include "Drawable/Point.h"
#include "FourierSurface.h"
#include <thread>

struct IG_DATA {
	static float THETA;
	static float PHI;
	static float SPEED;

	static int L;
	static int M;

	static float theta;
	static float phi;

	static bool UPDATE;
	static bool CURVES;
	static bool UPDATE_CURVES;

	static bool CALCULATE_FIGURE;
	static bool FIGURE_FILE;
	static int FIGURE_VIEW;
	static unsigned int NFIG;
	static bool LOADING;
	static const char* FILENAME;
	static unsigned int MAXL;
	static bool UPDATE_TEXTURE;
	static _float4color TEXTURE;

	static Vector2i WindowDim;

	static int UPDATE_LIGHT;
	struct lightsource {
		bool is_on;
		_float4vector intensities;
		_float4color color;
		_float4vector position;
	};

	static lightsource* LIGHTS;
};

class Fourier {

private:

	Window window;

	float scale = 200.f;
	Vector3f center = { 0.f, 0.f, 0.f };
	Vector3f observer = { 0.f,-1.f, 0.f };

	Vector3f axis = Vector3f(sinf(0.5f), 0.f, cosf(0.5f));
	float dangle = 0.005f;

	Vector2i lastPos;
	bool dragging;

	Quaternion rotation = Quaternion(cosf(0.25f),0.f,sinf(0.25f),0.f);

	//	Forced return

	bool returning = false;
	bool strict = false;

	//	Magnet motion

	bool magnetized = false;
	Vector3f magnetPos = { 1.f, 0.f, 0.f };

	void drag_dynamic_space();
	void magneticReturn();
	void strictReturn();

	// Surfaces

	FourierSurface harmonics;
	FourierSurface::Coefficient C = { (unsigned int)IG_DATA::L, IG_DATA::M, 1 };

	// Multithread stuff

	bool Ffigu = false;
	bool Fcoef = false;
	FourierSurface::Coefficient* coef;
	std::mutex mtx;
	FourierSurface** Figure;

public:
	Fourier();

	int Run();


	void eventManager();
	void doFrame();
};

//  Threading utlities

void createFigureAsync(Graphics* gfx, FourierSurface* figure, FourierSurface::Coefficient** coef, unsigned int ncoef, bool* done, std::mutex* mtx, bool* begin);
void calculateCoefficientsAsync(FourierSurface::Coefficient** coef, const char* filename, unsigned int maxL, bool* done);
