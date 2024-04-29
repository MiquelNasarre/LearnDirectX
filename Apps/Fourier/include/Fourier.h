#pragma once
#include "Window.h"
#include "FourierSurface.h"
#include "Polihedron.h"

struct IG_DATA {

	static int L;
	static int M;

	static float theta;
	static float phi;

	static bool UPDATE;
	static bool CURVES;
	static bool UPDATE_CURVES;

	static bool DOUBLE_VIEW;
	static int FIGURE_VIEW;
	static int SECOND_VIEW;
	static Vector2i* PAIRS;
	static unsigned int PAIRS_SIZE;
	static bool ALREADY_EXISTS;
	static int COPY;

	static bool CALCULATE_FIGURE;
	static bool FIGURE_FILE;
	static unsigned int NFIG;
	static unsigned int NPLOT;
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
	Vector3f observer = { -1.f, 0.f, 0.f };

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
	void drag_dynamic_plane();
	void magneticReturn();
	void strictReturn();

	// Figures

	Polihedron** DataPlots;

	// Surfaces

	FourierSurface harmonics;
	FourierSurface::Coefficient C = { (unsigned int)IG_DATA::L, IG_DATA::M, 1 };

	bool Ffigu = false;
	bool Fcoef = false;
	bool Fplot = false;
	const void** extractedFigure = NULL;
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

void createPlotAsync(Graphics* gfx, Polihedron* dataplot, const void** extractedFigure, bool* done, std::mutex* mtx);
void createFigureAsync(Graphics* gfx, FourierSurface* figure, FourierSurface::Coefficient** coef, unsigned int ncoef, bool* done, std::mutex* mtx, bool* begin);
void calculateCoefficientsAsync(FourierSurface::Coefficient** coef, const void** extractedFigure, unsigned int maxL, bool* done);
