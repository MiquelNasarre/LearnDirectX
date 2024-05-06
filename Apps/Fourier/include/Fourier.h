#pragma once
#include "Window.h"
#include "FourierSurface.h"
#include "Polihedron.h"
#include "Interpolated.h"

struct IG {

	static int L;
	static int M;

	static float theta;
	static float phi;

	static bool UPDATE;
	static bool CURVES;
	static bool UPDATE_CURVES;

	static bool DOUBLE_VIEW;
	static int VIEW1;
	static int VIEW2;
	static Vector2i* PAIRS;
	static unsigned int PAIRS_SIZE;
	static bool ALREADY_EXISTS;
	static int COPY;
	static bool SAVE;
	static char* SAVE_NAME;

	static bool CALCULATE_FIGURE;
	static bool FIGURE_FILE;
	static unsigned int NFIG;
	static unsigned int NPLOT;
	static bool LOADING;
	static char* FILENAME;
	static unsigned int MAXL;
	static unsigned short TDEPTH;
	static bool UPDATE_TEXTURE;
	static _float4color TEXTURE;
	static bool DELETE_VIEW;

	static unsigned int NINT;
	static float TVALUE;
	static bool PLAY;
	static float PLAY_SPEED;
	static int** I_DATA;
	static unsigned int* I_DATA_SIZE;
	static bool I_ADD;
	static int ADD_FIGURE;
	static int DELETE_FIGURE;

	static Vector2i WindowDim;

	static int UPDATE_LIGHT;
	struct lightsource {
		bool is_on;
		_float4vector intensities;
		_float4color color;
		_float4vector position;
	};
	static lightsource* LIGHTS;

	static bool MENU;
	static bool COEF_VIEW;
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

	//	Figures

	Polihedron** DataPlots;

	//	Harmonics

	FourierSurface harmonics;
	Coefficient C = { (unsigned int)IG::L, IG::M, 1 };

	//	Surfaces

	bool Ffigu = false;
	bool Fcoef = false;
	bool Fplot = false;
	const void** extractedFigure = NULL;
	Coefficient* coef;
	std::mutex mtx;
	FourierSurface** Figure;

	//	Interpolations
	
	InterpolatedString** Interpolations = NULL;

	//	Coefficients view

	bool isCoefWindowOpen = false;
	unsigned int Ncoef = 0u;
	Coefficient* linkedCoef = NULL;

	void createCoefWindow();
	void updateCoefWindow();
	void deleteCoefWindow();

	//  Threading utlities

	static void createPlotAsync(Graphics* gfx, Polihedron* dataplot, const void** extractedFigure, bool* done, std::mutex* mtx);
	static void createFigureAsync(Graphics* gfx, FourierSurface* figure, Coefficient** coef, unsigned int ncoef, bool* done, std::mutex* mtx, bool* begin);
	static void calculateCoefficientsAsync(Coefficient** coef, const void** extractedFigure, unsigned int maxL, bool* done);

public:
	Fourier();

	int Run();


	void eventManager();
	void doFrame();
};




