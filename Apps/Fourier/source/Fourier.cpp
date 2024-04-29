#include "Fourier.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "IG_Fourier.h"

float			IG_DATA::THETA = 0.f;
float			IG_DATA::PHI   = 0.f;
float			IG_DATA::SPEED = 0.f;
int				IG_DATA::L = 7;
int				IG_DATA::M = -4;
float			IG_DATA::theta = 0.f;
float			IG_DATA::phi = 0.f;
bool			IG_DATA::UPDATE = false;
bool			IG_DATA::UPDATE_CURVES = false;
bool			IG_DATA::CURVES = false;

bool			IG_DATA::CALCULATE_FIGURE = false;
bool			IG_DATA::FIGURE_FILE = true;
int				IG_DATA::FIGURE_VIEW = -1;
bool			IG_DATA::LOADING = false;
const char*		IG_DATA::FILENAME = (const char*)calloc(100, sizeof(char));
unsigned int	IG_DATA::MAXL = 0u;
unsigned int	IG_DATA::NFIG = 0u;
Vector2i		IG_DATA::WindowDim = { 0, 0 };

bool			IG_DATA::UPDATE_TEXTURE = false;
_float4color	IG_DATA::TEXTURE = { -1.f,0.f,0.f,0.f };

int						IG_DATA::UPDATE_LIGHT = -1;
IG_DATA::lightsource*	IG_DATA::LIGHTS = (IG_DATA::lightsource*)calloc(sizeof(IG_DATA::lightsource), 8);

Fourier::Fourier()
	: window(960, 720, "Fourier", "", true)
{
	window.setFramerateLimit(60);

	IG_DATA::LIGHTS[0].is_on = true;
	IG_DATA::LIGHTS[0].color = { 1.f, 1.f, 1.f, 1.f };
	IG_DATA::LIGHTS[0].intensities = { 600.f, 320.f, 0.f, 0.f };
	IG_DATA::LIGHTS[0].position = { 30.f, 10.f, 20.f, 0.f };

	harmonics.create(window.graphics, &C, 1u);
}

int Fourier::Run()
{
	while (window.processEvents())
		doFrame();
	return 0;
}

void Fourier::drag_dynamic_space()
{
	Vector3f obs = window.graphics.getObserver();
	Vector3f ex = -(obs * Vector3f(0.f, 0.f, 1.f)).normalize();
	Vector3f ey = ex * obs;

	Vector3f lastMouse = (-obs + (ex * (lastPos.x - window.getDimensions().x / 2) + ey * (lastPos.y - window.getDimensions().y / 2)) / scale).normalize();
	lastPos = Mouse::getPosition();
	Vector3f newMouse = (-obs + (ex * (lastPos.x - window.getDimensions().x / 2) + ey * (lastPos.y - window.getDimensions().y / 2)) / scale).normalize();

	Vector3f axis0 = lastMouse * newMouse;
	if (!axis0) axis0 = newMouse;
	axis0.normalize();
	float dangle0 = -acosf(lastMouse ^ newMouse);
	if ((lastMouse ^ newMouse) > 1.f)dangle0 = 0.f;
	if (lastMouse == newMouse) dangle0 = 0.f;

	constexpr float s = 1.f / 1.5f;

	Quaternion newRot = rotationQuaternion(newMouse, Mouse::getWheel() / 18000.f) * rotationQuaternion(axis0, dangle0) * rotationQuaternion(axis, (dangle * (1 - s + fabs(axis ^ newMouse) * s)));

	dangle = 2 * acosf(newRot.r);
	axis = newRot.getVector();
	if (!axis)axis = newMouse;
	axis.normalize();
}

void Fourier::magneticReturn()
{
	if (rotation.r < 0)rotation = -rotation;
	constexpr float pull = 0.03f;

	float angle = 2 * acosf(rotation.r);
	Vector3f axisRot = rotation.getVector();

	Quaternion newRot;
	if (angle < pull)
		newRot = rotationQuaternion(axisRot, -pull / 40.f) * rotationQuaternion(axis, dangle);
	else
		newRot = rotationQuaternion(axisRot, -angle / 40.f) * rotationQuaternion(axis, dangle);
	
	axis = newRot.getVector();
	dangle = 2 * acos(newRot.r);

	dangle *= 0.95f;

	if ((fabs(angle) < 0.01f && fabs(dangle) < 0.005f))
	{
		rotation = 1.f;
		axis = { 1.f,0.f,0.f };
		dangle = 0.f;
		returning = false;

	}
}

void Fourier::strictReturn()
{
	dangle = 0.f;

	if (rotation.r < 0)rotation = -rotation;

	float angle = 2 * acosf(rotation.r);
	Vector3f axisRot = rotation.getVector();

	axis = axisRot;
	dangle = -powf(angle, 0.65f) / 40.f;

	if (fabs(angle) < 0.01f)
	{
		rotation = 1.f;
		strict = false;
		dangle = 0.f;
	}
}

void Fourier::eventManager()
{
	// Full screen

	if (Keyboard::isKeyPressed(VK_F11))
		window.setFullScreen(true);

	if (Keyboard::isKeyPressed(VK_ESCAPE))
		window.setFullScreen(false);

	// returning motion

	if (Keyboard::isKeyPressed('R'))
		magneticReturn();

	if (Keyboard::isKeyPressed('T'))
		strictReturn();

	if (Keyboard::isKeyPressed('S'))
		dangle = 0.f;

	// Quaternion motion

	if (Mouse::isButtonPressed(Mouse::Left) && !dragging)
	{
		dragging = true;
		lastPos = Mouse::getPosition();
	}

	if (dragging && !Mouse::isButtonPressed(Mouse::Left))
		dragging = false;

	if (dragging)
			drag_dynamic_space();

	else
		scale *= powf(1.1f, Mouse::getWheel() / 120.f);

	rotation *= rotationQuaternion(axis, dangle * window.getFrameTime() * 60.f);

	IG_DATA::WindowDim = window.getDimensions();

	if (!(rotation.abs() < 10.f))
	{
		rotation = 1.f;
		axis = { 1.f,1.f,1.f };
		dangle = 0.f;
	}


	//	Calculate observer vector

	observer = {
			-cosf(IG_DATA::PHI) * cosf(IG_DATA::THETA) ,
			-cosf(IG_DATA::PHI) * sinf(IG_DATA::THETA) ,
			-sinf(IG_DATA::PHI)
	};

	// Multithread

	if (IG_DATA::CALCULATE_FIGURE)
	{
		IG_DATA::CALCULATE_FIGURE = false;
		IG_DATA::LOADING = true;

		FourierSurface** tFigure = (FourierSurface**)calloc(IG_DATA::NFIG + 1, sizeof(void*));
		for (unsigned int i = 0; i < IG_DATA::NFIG; i++)
			tFigure[i] = Figure[i];
		if(IG_DATA::NFIG)
			free(Figure);
		Figure = tFigure;
		Figure[IG_DATA::NFIG] = new(FourierSurface);

		if (IG_DATA::FIGURE_FILE)
		{
			std::thread(calculateCoefficientsAsync, &coef, IG_DATA::FILENAME, IG_DATA::MAXL, &Fcoef).detach();
			std::thread(createFigureAsync, &window.graphics, Figure[IG_DATA::NFIG], &coef, (IG_DATA::MAXL + 1) * (IG_DATA::MAXL + 1), &Ffigu, &mtx, &Fcoef).detach();
		}
		else
		{
			coef = FourierSurface::FileManager::loadCoefficientsFromFile(IG_DATA::FILENAME);
			Fcoef = true;
			std::thread(createFigureAsync, &window.graphics, Figure[IG_DATA::NFIG], &coef, FourierSurface::FileManager::ncoefFromFile(IG_DATA::FILENAME), &Ffigu, &mtx, &Fcoef).detach();
		}
	}

	if (Ffigu)
	{
		Ffigu = false;
		IG_DATA::NFIG++;
		IG_DATA::LOADING = false;
		IG_DATA::FIGURE_VIEW = IG_DATA::NFIG - 1;
		IG_DATA::UPDATE_CURVES = true;
		IG_DATA::UPDATE_LIGHT = -2;
	}

	//	Shape updates

	if (IG_DATA::UPDATE)
	{
		IG_DATA::UPDATE = false;

		C.L = (unsigned int)IG_DATA::L;
		C.M = IG_DATA::M;

		harmonics.updateShape(window.graphics, &C, 1u);

		if (IG_DATA::CURVES)
		{
			if (IG_DATA::FIGURE_VIEW == -1)
				harmonics.updateCurves(window.graphics, IG_DATA::phi, IG_DATA::theta);
			else
				Figure[IG_DATA::FIGURE_VIEW]->updateCurves(window.graphics, IG_DATA::phi, IG_DATA::theta);
		}
	}

	if (IG_DATA::UPDATE_CURVES)
	{
		IG_DATA::UPDATE_CURVES = false;

		if (IG_DATA::FIGURE_VIEW == -1)
			harmonics.updateCurves(window.graphics, IG_DATA::phi, IG_DATA::theta);
		else
			Figure[IG_DATA::FIGURE_VIEW]->updateCurves(window.graphics, IG_DATA::phi, IG_DATA::theta);
	}

	//	Set lights & textures

	int l = IG_DATA::UPDATE_LIGHT;
	if (l == -2) {
		for (int i = 0; i < 8; i++) {
			harmonics.updateLight(window.graphics, i, IG_DATA::LIGHTS[i].intensities, IG_DATA::LIGHTS[i].color, IG_DATA::LIGHTS[i].position);
			for (unsigned int j = 0; j < IG_DATA::NFIG; j++)
				Figure[j]->updateLight(window.graphics, i, IG_DATA::LIGHTS[i].intensities, IG_DATA::LIGHTS[i].color, IG_DATA::LIGHTS[i].position);
		}
	}
	if (l > -1) {
		harmonics.updateLight(window.graphics, l, IG_DATA::LIGHTS[l].intensities, IG_DATA::LIGHTS[l].color, IG_DATA::LIGHTS[l].position);
		for (unsigned int j = 0; j < IG_DATA::NFIG; j++)
			Figure[j]->updateLight(window.graphics, l, IG_DATA::LIGHTS[l].intensities, IG_DATA::LIGHTS[l].color, IG_DATA::LIGHTS[l].position);
		IG_DATA::UPDATE_LIGHT = -1;
	}

	if (IG_DATA::UPDATE_TEXTURE)
	{
		IG_DATA::UPDATE_TEXTURE = false;
		if (IG_DATA::TEXTURE.r == -1.f)
		{
			if (IG_DATA::FIGURE_VIEW > -1)
				Figure[IG_DATA::FIGURE_VIEW]->updateTexture(window.graphics, Color::White, true);
			else
				harmonics.updateTexture(window.graphics, Color::White, true);
		}
		else if (IG_DATA::TEXTURE.g == -1.f)
		{
			if (IG_DATA::FIGURE_VIEW > -1)
				Figure[IG_DATA::FIGURE_VIEW]->updateTexture(window.graphics, Color::White, false, true);
			else
				harmonics.updateTexture(window.graphics, Color::White, false, true);
		}
		else
		{
			if (IG_DATA::FIGURE_VIEW > -1)
				Figure[IG_DATA::FIGURE_VIEW]->updateTexture(window.graphics, Color((float*)&IG_DATA::TEXTURE));
			else
				harmonics.updateTexture(window.graphics, Color((float*)&IG_DATA::TEXTURE));
		}
	}
}

void Fourier::doFrame()
{
	eventManager();

	//	Update objects

	window.graphics.updatePerspective(observer, center, scale);

	if (IG_DATA::FIGURE_VIEW == -1)
		harmonics.updateRotation(window.graphics, rotation);
	else
		Figure[IG_DATA::FIGURE_VIEW]->updateRotation(window.graphics, rotation);

	window.setTitle("Fourier  -  " + std::to_string(int(std::round(window.getFramerate()))) + "fps");

	//	Rendering

	window.graphics.clearBuffer(Color::Black);

	if (IG_DATA::FIGURE_VIEW == -1)
		harmonics.Draw(window.graphics);
	else
		Figure[IG_DATA::FIGURE_VIEW]->Draw(window.graphics);

	if (IG_DATA::CURVES)
	{
		if (IG_DATA::FIGURE_VIEW == -1)
			harmonics.DrawCurves(window.graphics);
		else
			Figure[IG_DATA::FIGURE_VIEW]->DrawCurves(window.graphics);
	}

	//	ImGui crap

	IG_Fourier::render();

	//	Push the frame to the scriin

	window.graphics.pushFrame();
}

//  Threading utilities

void createFigureAsync(Graphics* gfx, FourierSurface* figure, FourierSurface::Coefficient** coef, unsigned int ncoef, bool* done, std::mutex* mtx, bool* begin)
{
	while (begin && !(*begin))
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	*begin = false;

	// Crea la figura
	figure->create(*gfx, *coef, ncoef, mtx);
	*done = true;
}

void calculateCoefficientsAsync(FourierSurface::Coefficient** coef, const char* filename, unsigned int maxL, bool* done)
{
	*coef = FourierSurface::FileManager::calculateCoefficients(filename, maxL);
	*done = true;
}
